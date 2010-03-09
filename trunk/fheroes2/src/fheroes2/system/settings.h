/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
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

#ifndef H2SETTINGS_H
#define H2SETTINGS_H

#include <string>
#include <list>
#include <iostream>
#include <iomanip>
#include "gamedefs.h"
#include "difficulty.h"
#include "race.h"
#include "color.h"
#include "maps_fileinfo.h"
#include "game.h"
#include "game_io.h"
#include "bitmodes.h"

#define ListMapsDirectory std::list<std::string>

enum
{
    DBG_WARN	= 0x0001,
    DBG_INFO	= 0x0002,
    DBG_TRACE	= 0x0004,

    DBG_ENGINE	= 0x0010,
    DBG_GAME	= 0x0020,
    DBG_BATTLE	= 0x0040,
    DBG_AI	= 0x0080,
    DBG_NETWORK	= 0x0100,


    DBG_DEVEL	= 0x8000,
};

#define VERBOSE(x) std::cout << x << std::endl
#define IS_DEVEL() (DBG_DEVEL & Settings::Get().Debug())
#define IS_DEBUG(x, y) (((x) & Settings::Get().Debug()) && ((0x000F & (y)) <= (0x000F & Settings::Get().Debug())))
#define DEBUG(x, y, z) if(IS_DEBUG((x), (y))) VERBOSE(z)

class Settings
{
public:
    // ns: battle
    enum
    {
        BATTLEAUTO	  = 0x00000001,
        BATTLEGRID        = 0x00000002,
        BATTLEMOUSESHADOW = 0x00000004,
        BATTLEMOVESHADOW  = 0x00000008,
    };

    // ns: pocketpc
    enum
    {
	POCKETPC          = 0x00000001,
	LOWMEMORY         = 0x00000002,
	TAPMODE           = 0x00000004,
    };

    // ns: global
    enum
    {
	LOADGAME          = 0x00000001,
        ORIGINAL          = 0x00000002,
        PRICELOYALTY      = 0x00000004,

	AUTOSAVE          = 0x00000010,

	DEDICATEDSERVER   = 0x00000020,
	LOCALCLIENT       = 0x00000040,

	HIDEAIMOVE        = 0x00000080,

	SHOWCPANEL        = 0x00000100,
	SHOWRADAR         = 0x00000200,
	SHOWICONS         = 0x00000400,
	SHOWBUTTONS       = 0x00000800,
	SHOWSTATUS        = 0x00001000,
	SHOWSYSTEM        = 0x00002000,
        HIDEINTERFACE     = 0x00004000,

	FADE              = 0x00008000,
        EVILINTERFACE     = 0x00010000,
        FONTRENDERBLENDED = 0x00020000,

        FULLSCREEN        = 0x00400000,
        LOGO              = 0x00800000,

        SOUND             = 0x01000000,
        MUSIC_EXT         = 0x02000000,
        MUSIC_CD          = 0x04000000,
        MUSIC_MIDI        = 0x08000000,

        EDITOR            = 0x20000000,
        USEUNICODE        = 0x40000000,
	ALTRESOURCE       = 0x80000000,

	MUSIC		= MUSIC_CD | MUSIC_EXT | MUSIC_MIDI,

    };

    static Settings & Get(void);

    bool Read(const std::string & filename);
    bool Save(const std::string & filename) const;

    void Dump(std::ostream & stream = std::cout) const;
    bool LoadFileMapsMP2(const std::string & file);
    Maps::FileInfo & CurrentFileInfo(void);

    u8 MajorVersion(void) const;
    u8 MinorVersion(void) const;
    u16 Debug(void) const;
    u8 Animation(void) const;

    const std::string & BuildVersion(void) const;
    const std::string & DataDirectory(void) const;
    const std::string & LocalPrefix(void) const;
    const std::string & PlayMusCommand(void) const;

    const ListMapsDirectory & GetListMapsDirectory(void) const;

    Difficulty::difficulty_t GameDifficulty(void) const;

    const std::string & ForceLang(void) const;
    const std::string & FontsNormal(void) const;
    const std::string & FontsSmall(void) const;
    u8 FontsNormalSize(void) const;
    u8 FontsSmallSize(void) const;
    bool FontsRenderBlended(void) const;

    bool Editor(void) const;
    bool Sound(void) const;
    bool Music(void) const;
    bool CDMusic(void) const;
    bool FullScreen(void) const;
    bool HideInterface(void) const;
    bool ShowControlPanel(void) const;
    bool ShowRadar(void) const;
    bool ShowIcons(void) const;
    bool ShowButtons(void) const;
    bool ShowStatus(void) const;
    bool ShowSystem(void) const;
    bool EvilInterface(void) const;
    bool Logo(void) const;
    bool BattleAuto(void) const;
    bool BattleGrid(void) const;
    bool BattleMovementShaded(void) const;
    bool BattleMouseShaded(void) const;
    bool Unicode(void) const;
    bool PocketPC(void) const;
    bool LowMemory(void) const;
    bool TapMode(void) const;
    bool HideAIMove(void) const;
    bool UseAltResource(void) const;
    bool OriginalVersion(void) const;
    bool PriceLoyaltyVersion(void) const;
    bool LoadedGameVersion(void) const;
    bool UseFade(void) const;
    bool AutoSave(void) const;

    bool NetworkDedicatedServer(void) const;
    bool NetworkLocalClient(void) const;

    const Size & VideoMode(void) const;

    u8   SoundVolume(void) const;
    u8   MusicVolume(void) const;
    u32  GetMusicType(void) const;

    void SetDebug(const u16 d);
    void SetEditor(void);
    void SetUnicode(bool);
    void SetOriginalVersion(void);
    void SetPriceLoyaltyVersion(void);
    void SetGameDifficulty(const Difficulty::difficulty_t d);
    void SetEvilInterface(bool);
    void SetBattleAuto(bool);
    void SetBattleGrid(bool);
    void SetBattleMovementShaded(bool);
    void SetBattleMouseShaded(bool);
    void SetLoadedGameVersion(bool);
    void SetShowPanel(bool);
    void SetShowRadar(bool);
    void SetShowIcons(bool);
    void SetShowButtons(bool);
    void SetShowStatus(bool);
    void SetNetworkLocalClient(bool);
    void SetNetworkDedicatedServer(bool);

    void SetSoundVolume(const u8 v);
    void SetMusicVolume(const u8 v);
    void ResetSound(void);
    void ResetMusic(void);
    void SetAnimation(const u8 s);
    
    u8   GameType(void) const;
    void SetGameType(const Game::type_t type);

    Color::color_t CurrentColor(void) const;
    Color::color_t MyColor(void) const;
    void SetCurrentColor(const Color::color_t c);
    void SetMyColor(const Color::color_t c);
    u8   PlayersColors(void) const;
    void SetPlayersColors(u8 c);
    u8   PreferablyCountPlayers(void) const;
    void SetPreferablyCountPlayers(u8 c);

    void SetLocalPrefix(const char*);

    u16	GetPort(void) const;

    // from maps info
    Race::race_t KingdomRace(u8) const;
    void SetKingdomRace(u8, u8);
    void FixKingdomRandomRace(void);
    const std::string & MapsFile(void) const;
    const std::string & MapsName(void) const;
    const std::string & MapsDescription(void) const;
    Difficulty::difficulty_t MapsDifficulty(void) const;
    u8 MapsWidth(void) const;
    bool AllowColors(u8) const;
    Color::color_t FirstAllowColor(void) const;
    bool KingdomColors(u8) const;
    u8 KingdomColors(void) const;
    void SetKingdomColors(u8);
    bool AllowChangeRace(u8) const;
    u8 AllowColorsCount(void) const;
    u8 KingdomColorsCount(void) const;
    bool MapsWithHeroes(void) const;
    GameOver::conditions_t ConditionWins(void) const;
    GameOver::conditions_t ConditionLoss(void) const;
    bool WinsCompAlsoWins(void) const;
    bool WinsAllowNormalVictory(void) const;
    Artifact::artifact_t WinsFindArtifact(void) const;
    bool WinsFindUltimateArtifact(void) const;
    u16 WinsSidePart(void) const;
    u32 WinsAccumulateGold(void) const;
    u32 WinsMapsIndexObject(void) const;
    u32 LossMapsIndexObject(void) const;
    u16 LossCountDays(void) const;

protected:
    void Parse(const std::string & left, const std::string & right);
    void SetStrModes(const std::string & key);
    void ResetStrModes(const std::string & key);

private:
    friend class Game::IO;

    Settings();

    BitModes opt_global;
    BitModes opt_battle;
    BitModes opt_pocket;

    const u8 major_version;
    const u8 minor_version;
    const std::string svn_version;
    u16 debug;

    Size video_mode;

    Difficulty::difficulty_t game_difficulty;
    Color::color_t my_color;
    Color::color_t cur_color;

    std::string build_version;
    std::string path_data_directory;
    std::string local_prefix;

    ListMapsDirectory list_maps_directory;

    std::string font_normal;
    std::string font_small;
    std::string force_lang;
    u8 size_normal;
    u8 size_small;

    Maps::FileInfo current_maps_file;

    u8 sound_volume;
    u8 music_volume;
    u8 animation;

    u8 game_type;
    u8 players_colors;
    u8 preferably_count_players;

    std::string playmus_command;

    u16 port;
};

#endif