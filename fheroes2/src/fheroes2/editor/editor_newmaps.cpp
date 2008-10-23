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

#include "agg.h"
#include "dialog.h"
#include "cursor.h"
#include "sprite.h"
#include "localevent.h"
#include "button.h"
#include "display.h"
#include "config.h"
#include "maps.h"
#include "game.h"
#include "world.h"

#include "error.h"

Game::menu_t Game::Editor::NewMaps(void)
{
    Display & display = Display::Get();

    // preload
    AGG::PreloadObject(ICN::BTNESIZE);
    AGG::PreloadObject(ICN::REDBACK);

    // cursor
    Cursor & cursor = Cursor::Get();
    cursor.Hide();
    cursor.SetThemes(cursor.POINTER);

    Display::SetVideoMode(640, 480);

    // image background
    const Sprite &back = AGG::GetICN(ICN::EDITOR, 0);
    display.Blit(back);

    const Sprite &panel = AGG::GetICN(ICN::REDBACK, 0);
    display.Blit(panel, 405, 5);

    LocalEvent & le = LocalEvent::GetLocalEvent();

    Button buttonSmall(455, 45, ICN::BTNESIZE, 0, 1);
    Button buttonMedium(455, 110, ICN::BTNESIZE, 2, 3);
    Button buttonLarge(455, 175, ICN::BTNESIZE, 4, 5);
    Button buttonXLarge(455, 240, ICN::BTNESIZE, 6, 7);
    Button buttonCancel(455, 375, ICN::BTNESIZE, 8, 9);

    buttonSmall.Draw();
    buttonMedium.Draw();
    buttonLarge.Draw();
    buttonXLarge.Draw();
    buttonCancel.Draw();

    cursor.Show();
    display.Flip();

    // NewMap loop
    while(le.HandleEvents())
    {
	le.MousePressLeft(buttonSmall) ? buttonSmall.PressDraw() : buttonSmall.ReleaseDraw();
	le.MousePressLeft(buttonMedium) ? buttonMedium.PressDraw() : buttonMedium.ReleaseDraw();
	le.MousePressLeft(buttonLarge) ? buttonLarge.PressDraw() : buttonLarge.ReleaseDraw();
	le.MousePressLeft(buttonXLarge) ? buttonXLarge.PressDraw() : buttonXLarge.ReleaseDraw();
	le.MousePressLeft(buttonCancel) ? buttonCancel.PressDraw() : buttonCancel.ReleaseDraw();

	if(le.MouseClickLeft(buttonSmall)) {
		world.NewMaps(Maps::SMALL, Maps::SMALL);
		return EDITSTART;
	}
	if(le.MouseClickLeft(buttonMedium)) {
		world.NewMaps(Maps::MEDIUM, Maps::MEDIUM);
		return EDITSTART;
	}
	if(le.MouseClickLeft(buttonLarge)) {
		world.NewMaps(Maps::LARGE, Maps::LARGE);
		return EDITSTART;
	}
	if(le.MouseClickLeft(buttonXLarge)) {
		world.NewMaps(Maps::XLARGE, Maps::XLARGE);
		return EDITSTART;
	}
	if(le.MouseClickLeft(buttonCancel) || le.KeyPress(KEY_ESCAPE)) return EDITMAINMENU;

        // right info
	if(le.MousePressRight(buttonSmall)) Dialog::Message("", "Create a map that is 36 squares wide by 36 squares high. (For reference, all the maps in Heroes where 72 x 72)", Font::BIG);
	else
	if(le.MousePressRight(buttonMedium)) Dialog::Message("", "Create a map that is 72 squares wide by 72 squares high. (For reference, all the maps in Heroes where 72 x 72)", Font::BIG);
	else
	if(le.MousePressRight(buttonLarge)) Dialog::Message("", "Create a map that is 108 squares wide by 108 squares high. (For reference, all the maps in Heroes where 72 x 72)", Font::BIG);
	else
	if(le.MousePressRight(buttonXLarge)) Dialog::Message("", "Create a map that is 144 squares wide by 144 squares high. (For reference, all the maps in Heroes where 72 x 72)", Font::BIG);
	else
	if(le.MousePressRight(buttonCancel)) Dialog::Message("", "Cancel back to the main menu.", Font::BIG);
    }

    return EDITMAINMENU;
}
