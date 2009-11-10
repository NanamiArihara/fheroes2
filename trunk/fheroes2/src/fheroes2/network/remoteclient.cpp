/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov                               *
 *   afletdinov@mail.dc.baikal.ru                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef WITH_NET

#include <sstream>
#include <algorithm>
#include <functional>
#include "world.h"
#include "settings.h"
#include "server.h"
#include "client.h"
#include "kingdom.h"
#include "remoteclient.h"
#include "zzlib.h"

int FH2RemoteClient::callbackCreateThread(void *data)
{
    return data ? reinterpret_cast<FH2RemoteClient *>(data)->Main() : -1;
}

FH2RemoteClient::FH2RemoteClient()
{
}

void FH2RemoteClient::RunThread(void)
{
    if(!thread.IsRun())
	thread.Create(callbackCreateThread, this);
}

void FH2RemoteClient::ShutdownThread(void)
{
    SetModes(ST_SHUTDOWN);
    DELAY(100);

    if(thread.IsRun()) thread.Kill();

    if(sd)
    {
        packet.Reset();
	packet.SetID(MSG_SHUTDOWN);
	packet.Send(*this);
        Close();
    }
    SetModes(0);
}

void FH2RemoteClient::Logout(const std::string & str)
{
    packet.Reset();
    packet.SetID(MSG_LOGOUT);
    packet.Push(std::string("FH2RemoteClient::Logout: " + str));

    FH2Server & server = FH2Server::Get();
    server.Lock();
    server.PrepareSending(packet, player_id);
    server.Unlock();

    Close();

    modes = 0;
    player_id = 0;
    player_color = 0;
}

int FH2RemoteClient::Main(void)
{
    std::string error;

    if(ConnectionChat())
    {
	if(StartGame())
	{
	    // may be also
	}
	else
	    error = "StartGame: lost connection";
    }
    else
	error = "ConnectionChat: lost connection";

    Logout(error);

    return 1;
}

bool FH2RemoteClient::ConnectionChat(void)
{
    Settings & conf = Settings::Get();
    bool extdebug = 2 < conf.Debug();

    player_color = 0;
    player_race = Race::RAND;
    player_name.clear();

    // wait thread id
    if(extdebug) std::cerr << "FH2RemoteClient::ConnectionChat: wait start thread...";
    while(0 == thread.GetID()){ DELAY(10); };
    player_id = thread.GetID();
    if(extdebug) std::cerr << "ok" << std::endl;

    SetModes(ST_CONNECT);

    // send banner
    std::ostringstream banner;
    banner << "Free Heroes II Server, version: " << static_cast<int>(conf.MajorVersion()) << "." << static_cast<int>(conf.MinorVersion()) << std::endl;

    packet.Reset();
    packet.SetID(MSG_READY);
    packet.Push(banner.str());

    // send ready
    if(extdebug) std::cerr << "FH2RemoteClient::ConnectionChat: id: 0x" << std::hex << player_id << ", send ready...";
    if(!Send(packet, extdebug)) return false;
    if(extdebug) std::cerr << "ok" << std::endl;

    // recv hello
    if(extdebug) std::cerr << "FH2RemoteClient::ConnectionChat: id: 0x" << std::hex << player_id << ", recv hello...";
    if(!Wait(packet, MSG_HELLO, extdebug)) return false;
    if(extdebug) std::cerr << "ok" << std::endl;

    packet.Pop(player_name);
    if(conf.Debug())
        std::cerr << "FH2RemoteClient::ConnectionChat: id: 0x" << std::hex << player_id << ", connected " << " player: " << player_name << ", host 0x" << std::hex << Host() << ":0x" << Port() << std::endl;

    FH2Server & server = FH2Server::Get();

    // check color
    server.Lock();
    player_color = Color::GetFirst(conf.CurrentFileInfo().allow_colors & (~conf.PlayersColors()));
    conf.SetPlayersColors(server.GetPlayersColors());
    server.Unlock();
    if(0 == player_color)
    {
	if(extdebug) std::cerr << "FH2RemoteClient::ConnectionChat: id: 0x" << std::hex << player_id << ", player_color = 0, logout" << std::endl;
	return false;
    }
    // send update players
    packet.Reset();
    packet.SetID(MSG_PLAYERS);
    server.Lock();
    server.PushPlayersInfo(packet);
    server.PrepareSending(packet, player_id);
    server.Unlock();

    // send hello, modes, id, color
    packet.Reset();
    packet.SetID(MSG_HELLO);
    packet.Push(modes);
    packet.Push(player_id);
    packet.Push(player_color);
    if(extdebug) std::cerr << "FH2RemoteClient::ConnectionChat: id: 0x" << std::hex << player_id << ", send hello...";
    if(!Send(packet, extdebug)) return false;
    if(extdebug) std::cerr << "ok" << std::endl;
    if(extdebug) std::cerr << "FH2RemoteClient::ConnectionChat: " << (Modes(ST_ADMIN) ? "admin" : "client") << " mode" << std::endl;

    if(Modes(ST_ADMIN)) SetModes(ST_ALLOWPLAYERS);

    if(extdebug) std::cerr << "FH2RemoteClient::ConnectionChat start queue" << std::endl;

    bool exit = false;

    while(!exit)
    {
        if(Modes(ST_SHUTDOWN)) return false;

	if(Ready())
	{
            if(extdebug) std::cerr << "FH2RemoteClient::ConnectionChat: recv: ";
	    if(!Recv(packet, extdebug)) return false;
            if(extdebug) std::cerr << Network::GetMsgString(packet.GetID()) << std::endl;

	    // check broadcast
	    MsgBroadcast();

    	    // msg processing
    	    switch(Network::GetMsg(packet.GetID()))
    	    {
    		case MSG_PING:
		    MsgPing(extdebug);
		    break;

        	case MSG_LOGOUT:
		    MsgLogout(extdebug);
	    	    return false;

    		case MSG_MAPS_INFO_SET:
		    if(!MsgMapsInfoSet(extdebug)) return false;
		    break;

    		case MSG_MAPS_INFO_GET:
		    if(!MsgMapsInfoGet(extdebug)) return false;
		    break;

    		case MSG_MAPS_LIST_GET:
		    if(!MsgMapsListGet(extdebug)) return false;
		    break;

    		case MSG_MAPS_LOAD:
		    if(Modes(ST_ADMIN))
		    {
			server.Lock();
			server.SetStartGame();
			server.Unlock();
			exit = true;
		    }
    		    break;

    		case MSG_PLAYERS_GET:
		    if(!MsgPlayersGet(extdebug)) return false;
		    break;

    		default:
    		    break;
    	    }
	}

        DELAY(100);
    }

    return true;
}

bool FH2RemoteClient::StartGame(void)
{
    Settings & conf = Settings::Get();
    //FH2Server & server = FH2Server::Get();
    bool extdebug = 2 < conf.Debug();
    bool exit = false;

    while(!exit)
    {
        if(Modes(ST_SHUTDOWN)) return false;

	if(Ready())
	{
            if(extdebug) std::cerr << "FH2RemoteClient::StartGame: recv: ";
	    if(!Recv(packet, extdebug)) return false;
            if(extdebug) std::cerr << Network::GetMsgString(packet.GetID()) << std::endl;

	    // check broadcast
	    MsgBroadcast();

    	    // msg processing
    	    switch(Network::GetMsg(packet.GetID()))
    	    {
    		case MSG_PING:
		    MsgPing(extdebug);
		    break;

        	case MSG_LOGOUT:
		    MsgLogout(extdebug);
	    	    return false;

    		default:
    		    break;
    	    }
	}

        DELAY(100);
    }

    return true;
}

void FH2RemoteClient::MsgBroadcast(void)
{
    if(MSG_UNKNOWN != Network::GetMsg(packet.GetID()) && Network::MsgIsBroadcast(packet.GetID()))
    {
	FH2Server & server = FH2Server::Get();

	server.Lock();
	server.PrepareSending(packet, player_id);
	server.Unlock();
    }
}

void FH2RemoteClient::MsgPing(bool extdebug)
{
    if(extdebug) Error::Verbose("FH2RemoteClient::MsgPing:");
    packet.Reset();
    packet.SetID(MSG_PING);
    packet.Send(*this);
}

void FH2RemoteClient::MsgLogout(bool extdebug)
{
    // send message
    packet.Reset();
    packet.SetID(MSG_MESSAGE);
    std::string str = "logout player: " + player_name;
    packet.Push(str);

    FH2Server & server = FH2Server::Get();
    Settings & conf = Settings::Get();

    server.Lock();
    server.PrepareSending(packet, player_id);
    //
    conf.SetPlayersColors(server.GetPlayersColors() & (~player_color));
    world.GetKingdom(player_color).SetControl(Game::AI);
    // send players
    packet.Reset();
    packet.SetID(MSG_PLAYERS);
    server.PushPlayersInfo(packet, player_id);
    server.PrepareSending(packet, player_id);
    server.Unlock();
    if(extdebug) Error::Verbose("FH2RemoteClient::MsgLogout: ", str.c_str());
}

bool FH2RemoteClient::MsgMapsInfoSet(bool extdebug)
{
    FH2Server & server = FH2Server::Get();
    Settings & conf = Settings::Get();
    std::string str;

    packet.Pop(str);
    if(Modes(ST_ADMIN) && Settings::Get().LoadFileMapsMP2(str))
    {
	packet.Reset();
        packet.SetID(MSG_MAPS_INFO);
	server.Lock();
	Network::PacketPushMapsFileInfo(packet, conf.CurrentFileInfo());
	server.PrepareSending(packet, 0);
	server.Unlock();
	if(extdebug) std::cerr << "FH2RemoteClient::MsgMapsInfoSet: ";
	if(!Send(packet, extdebug)) return false;
	if(extdebug) std::cerr << "ok" << std::endl;

	// reset players
	server.Lock();
	server.ResetPlayers(player_id);
	server.Unlock();

	// send players
	packet.Reset();
	packet.SetID(MSG_PLAYERS);
	server.Lock();
        server.PushPlayersInfo(packet);
	server.PrepareSending(packet, 0);
	server.Unlock();
    }
    return true;
}

bool FH2RemoteClient::MsgMapsInfoGet(bool extdebug)
{
    Settings & conf = Settings::Get();

    packet.Reset();
    packet.SetID(MSG_MAPS_INFO);
    Network::PacketPushMapsFileInfo(packet, conf.CurrentFileInfo());
    if(extdebug) std::cerr << "FH2RemoteClient::MsgMapsInfoGet: ";
    if(!Send(packet, extdebug)) return false;
    if(extdebug) std::cerr << "ok" << std::endl;
    return true;
}

bool FH2RemoteClient::MsgMapsListGet(bool extdebug)
{
    FH2Server & server = FH2Server::Get();
    //Settings & conf = Settings::Get();

    packet.Reset();
    packet.SetID(MSG_MAPS_LIST);
    server.Lock();
    server.PushMapsFileInfoList(packet);
    server.Unlock();
    if(extdebug) std::cerr << "FH2RemoteClient::MsgMapsListGet: ";
    if(!Send(packet, extdebug)) return false;
    if(extdebug) std::cerr << "ok" << std::endl;
    return true;
}

bool FH2RemoteClient::MsgPlayersGet(bool extdebug)
{
    FH2Server & server = FH2Server::Get();

    packet.Reset();
    packet.SetID(MSG_PLAYERS);
    server.Lock();
    server.PushPlayersInfo(packet);
    server.Unlock();
    if(extdebug) std::cerr << "FH2RemoteClient::MsgPlayersGet: ";
    if(!Send(packet, extdebug)) return false;
    if(extdebug) std::cerr << "ok" << std::endl;
    return true;
}

#endif
