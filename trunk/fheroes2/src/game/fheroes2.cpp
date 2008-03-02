/***************************************************************************
 *   Copyright (C) 2006 by Andrey Afletdinov                               *
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

#include <iostream>
#include <string>
#include <unistd.h>
#include <libgen.h>

#include "gamedefs.h"
#include "cmdline.h"
#include "tools.h"
#include "config.h"
#include "error.h"
#include "localevent.h"
#include "rand.h"
#include "dir.h"
#include "agg.h"
#include "cursor.h"
#include "game.h"
#include "engine.h"
#include "image_logo.h"
#include "image_icons.h"

int main(int argc, char **argv)
{
	chdir(dirname(argv[0]));

	Settings & conf = Settings::Get();

	std::string caption("Free Heroes II, version: ");
	String::AddInt(caption, conf.MajorVersion());
	caption += ".";
	String::AddInt(caption, conf.MinorVersion());

#ifndef BUILD_RELEASE
	caption += ", build: ";
	String::AddInt(caption, conf.DateBuild());
#endif

	std::cout << caption << std::endl;

	// load fheroes2.cfg
	const std::string & fheroes2_cfg = "fheroes2.cfg";
	std::cout << "config: " << fheroes2_cfg << (conf.Read(fheroes2_cfg) ? " load" : " not found") << std::endl;

	{
	    // parse cmd params
	    CmdLine cmd((const char **) argv);

	    if(cmd.Exists('h'))
	    {
		std::cout << "Usage: " << argv[0] << " [OPTIONS]\n" \
		    << "  -e\teditors mode\n" \
		    << "  -d\tdebug mode\n" \
		    << "  -c\tpath to config file (default fheroes2.cfg)\n" \
		    << "  -h\tprint this help and exit" << std::endl;

		return EXIT_SUCCESS;
	    }

	    // load cmd config
	    if(cmd.Exists('c'))
	    {
		const std::string & cmd_config = cmd.GetValue('c');

		std::cout << "config: " << cmd_config << (conf.Read(cmd_config) ? " load" : " not found") << std::endl;
	    }

	    // set debug
	    if(cmd.Exists('d')) conf.SetDebug(String::ToInt(cmd.GetValue('d')));

	    // editor mode
	    if(cmd.Exists('e'))
	    {
		conf.SetModes(Settings::EDITOR);

		if(conf.Debug()) std::cout << "start: editor mode." << std::endl;

		caption = "Free Heroes II (Editor), version: ";
		String::AddInt(caption, conf.MajorVersion());
		caption += ".";
		String::AddInt(caption, conf.MinorVersion());

#ifndef BUILD_RELEASE
		caption += ", build: ";
		String::AddInt(caption, conf.DateBuild());
#endif
	    }
	}

	// random init
	Rand::Init();
	
	const u32 subsystem = conf.Sound() || conf.Music() ? INIT_VIDEO | INIT_AUDIO : INIT_VIDEO;

	if(SDL::Init(subsystem))
	try
	{
	    Display::SetVideoMode(Display::SMALL);
	    Display::HideCursor();
	    Display::SetCaption(caption);

    	    Surface icons(image_icons.pixel_data, image_icons.width, image_icons.height, image_icons.bytes_per_pixel, true);
	    Display::SetIcons(icons);

	    AGG::Cache & cache = AGG::Cache::Get();

	    // read data directory
    	    Dir dir;
    	    
    	    dir.Read(conf.DataDirectory(), "agg");
    	    dir.Read(conf.DataDirectory(), "Agg");
            dir.Read(conf.DataDirectory(), "AGG");

	    const u16 count_agg = dir.size();

	    // not found agg, exit
	    if(0 == count_agg) Error::Except("AGG data files not found.");

    	    std::string agg_heroes2;
    	    std::string agg_heroes2x;

    	    // attach agg files
    	    for(u16 ii = 0; ii < count_agg; ++ii)
    	    {
    		std::string filename(basename(const_cast<char *>(dir[ii].c_str())));

    		// skip original version heroes2.agg
    		if(0 == strncasecmp("heroes2.agg", filename.c_str(), 11))
    		    agg_heroes2 = dir[ii];
    		else
    		// skip price loyality version heroes2x.agg
    		if(0 == strncasecmp("heroes2x.agg", filename.c_str(), 12))
    		{
    		    agg_heroes2x = dir[ii];
    		    
    		    conf.SetModes(Settings::PRICELOYALTY);
    		}
    		else
    		    cache.AttachFile(dir[ii]);
	    }

    	    // loyality version heroes2x.agg attach
	    if(agg_heroes2x.size()) cache.AttachFile(agg_heroes2x);

	    // original version heroes2.agg need to last attach
	    if(agg_heroes2.size()) cache.AttachFile(agg_heroes2);

	    // load palette
	    cache.LoadPAL();

#ifdef BUILD_RELEASE
	    // SDL logo
	    if(H2Config::Logo())
	    {
		Display & display = Display::Get();

    		Surface logo(image_logo.pixel_data, image_logo.width, image_logo.height, image_logo.bytes_per_pixel, false);
    		
    		logo.SetDisplayFormat();

		const u32 black = logo.MapRGB(0, 0, 0);

		u8 ii = 0;

		while(ii < 250)
		{
		    logo.SetAlpha(ii);
		    display.Blit(logo);
		    display.Flip();
		    display.Fill(black);
		    ii += 10;
		}
		
		SDL_Delay(500);

		while(ii > 0)
		{
		    logo.SetAlpha(ii);
		    display.Blit(logo);
		    display.Flip();
		    display.Fill(black);
		    ii -= 10;
		}
	    }
#endif

	    // init cursor
	    AGG::PreloadObject(ICN::ADVMCO);
	    Cursor::Get().SetThemes(Cursor::POINTER);

	    // default events
	    LocalEvent::SetStateDefaults();

	    // set global events
	    LocalEvent::SetGlobalFilterEvents(Cursor::Redraw);

	    // goto main menu
	    Game::menu_t rs = Game::MAINMENU;

	    while(rs != Game::QUITGAME)
	    {
		if(conf.Editor())
		{
		    switch(rs)
		    {
	    		case Game::MAINMENU:       rs = Game::Editor::MainMenu();	break;
	    		case Game::EDITNEWMAP:     rs = Game::Editor::NewMaps();	break;
	    		case Game::EDITLOADMAP:    rs = Game::Editor::LoadMaps();       break;
	    		case Game::EDITSTART:      rs = Game::Editor::StartGame();      break;

			default: break;
		    }
		}
		else
		{
		    switch(rs)
		    {
	    		case Game::MAINMENU:       rs = Game::MainMenu();		break;
	    		case Game::NEWGAME:        rs = Game::NewGame();		break;
	    		case Game::LOADGAME:       rs = Game::LoadGame();		break;
	    		case Game::HIGHSCORES:     rs = Game::HighScores();		break;
	    		case Game::CREDITS:        rs = Game::Credits();		break;
	    		case Game::NEWSTANDARD:    rs = Game::NewStandard();		break;
	    		case Game::NEWCAMPAIN:     rs = Game::NewCampain();		break;
	    		case Game::NEWMULTI:       rs = Game::NewMulti();		break;
	    		case Game::LOADSTANDARD:   rs = Game::LoadStandard();		break;
	    		case Game::LOADCAMPAIN:    rs = Game::LoadCampain();		break;
	    		case Game::LOADMULTI:      rs = Game::LoadMulti();		break;
	    		case Game::SCENARIOINFO:   rs = Game::ScenarioInfo();		break;
			case Game::STARTGAME:      rs = Game::StartGame();      	break;

	    		default: break;
		    }
		}
	    }

	    Display::ShowCursor();

	} catch(std::bad_alloc)
	{
	} catch(Error::Exception)
	{
	    conf.Dump();
	}

	SDL::Quit();

	std::cout << "Bye." << std::endl;
	
	return EXIT_SUCCESS;
}
