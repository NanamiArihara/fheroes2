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

#include "agg.h"
#include "settings.h"
#include "cursor.h"
#include "button.h"
#include "dialog.h"

void Dialog::SkillInfo(const Skill::Secondary::skill_t skill, const Skill::Level::type_t level, const bool ok_button)
{
    std::string header(Skill::Level::String(level));
    header.append(" ");
    header.append(Skill::Secondary::String(skill));
    const std::string description(Skill::Secondary::Description(skill, level));
    SkillInfo(header, description, skill, level, ok_button);
}

void Dialog::SkillInfo(const std::string &header, const std::string &message, const Skill::Secondary::skill_t skill, const Skill::Level::type_t level, const bool ok_button)
{
    Display & display = Display::Get();
    const ICN::icn_t system = Settings::Get().EvilInterface() ? ICN::SYSTEME : ICN::SYSTEM;

    // preload
    AGG::PreloadObject(system);

    // cursor
    Cursor & cursor = Cursor::Get();

    cursor.Hide();
    cursor.SetThemes(cursor.POINTER);

    TextBox box1(header, Font::YELLOW_BIG, BOXAREA_WIDTH);
    TextBox box2(message, Font::BIG, BOXAREA_WIDTH);
    const Sprite & border = AGG::GetICN(ICN::SECSKILL, 15);
    const u8 spacer = Settings::Get().PocketPC() ? 5 : 10;

    Box box(box1.h() + spacer + box2.h() + spacer + border.h(), ok_button);
    Rect pos = box.GetArea();

    if(header.size()) box1.Blit(pos);
    pos.y += box1.h() + spacer;

    if(message.size()) box2.Blit(pos);
    pos.y += box2.h() + spacer;

    // blit sprite
    pos.x = box.GetArea().x + (pos.w - border.w()) / 2;
    display.Blit(border, pos.x, pos.y);
    const Sprite & sprite = AGG::GetICN(ICN::SECSKILL, Skill::Secondary::GetIndexSprite1(skill));
    pos.x = box.GetArea().x + (pos.w - sprite.w()) / 2;
    display.Blit(sprite, pos.x, pos.y + 3);

    Text text;

    // small text
    text.Set(Skill::Secondary::String(skill), Font::SMALL);
    pos.x = box.GetArea().x + (pos.w - text.w()) / 2;
    text.Blit(pos.x, pos.y + 3);

    text.Set(Skill::Level::String(level));
    pos.x = box.GetArea().x + (pos.w - text.w()) / 2;
    text.Blit(pos.x, pos.y + 55);

    LocalEvent & le = LocalEvent::Get();

    Button *button = NULL;
    Point pt;

    if(ok_button)
    {
        pt.x = box.GetArea().x + (box.GetArea().w - AGG::GetICN(system, 1).w()) / 2;
        pt.y = box.GetArea().y + box.GetArea().h - AGG::GetICN(system, 1).h();
	button = new Button(pt, system, 1, 2);
    }

    if(button) (*button).Draw();

    cursor.Show();
    display.Flip();

    // message loop
    while(le.HandleEvents())
    {
        if(!ok_button && !le.MousePressRight()) break;

	if(button) le.MousePressLeft(*button) ? button->PressDraw() : button->ReleaseDraw();

        if(button && le.MouseClickLeft(*button)){ break; }

	if(le.KeyPress(KEY_RETURN) || le.KeyPress(KEY_ESCAPE)){ break; }
    }

    cursor.Hide();
    if(button) delete button;
}

void Dialog::SkillInfo(const std::string &header, const std::string &message, const Skill::Primary::skill_t skill)
{
    Display & display = Display::Get();
    const ICN::icn_t system = Settings::Get().EvilInterface() ? ICN::SYSTEME : ICN::SYSTEM;

    // preload
    AGG::PreloadObject(system);

    // cursor
    Cursor & cursor = Cursor::Get();

    cursor.Hide();
    cursor.SetThemes(cursor.POINTER);

    u8 index = 0;
    std::string skill_name;

    switch(skill)
    {
	case Skill::Primary::ATTACK:
	    index = 0;
	    skill_name = _("Attack Skill");
	    break;

	case Skill::Primary::DEFENCE:
	    index = 1;
	    skill_name = _("Defense Skill");
	    break;

	case Skill::Primary::POWER:
	    index = 2;
	    skill_name = _("Spell Power");
	    break;

	case Skill::Primary::KNOWLEDGE:
	    index = 3;
	    skill_name = _("Knowledge");
	    break;

	default: break;
    }

    TextBox box1(header, Font::BIG, BOXAREA_WIDTH);
    TextBox box2(message, Font::BIG, BOXAREA_WIDTH);
    const Sprite & border = AGG::GetICN(ICN::PRIMSKIL, 4);
    const u8 spacer = Settings::Get().PocketPC() ? 5 : 10;

    Box box(box1.h() + spacer + box2.h() + spacer + border.h(), Dialog::OK);
    Rect pos = box.GetArea();

    if(header.size()) box1.Blit(pos);
    pos.y += box1.h() + spacer;

    if(message.size()) box2.Blit(pos);
    pos.y += box2.h() + spacer;

    // blit sprite
    pos.x = box.GetArea().x + (pos.w - border.w()) / 2;
    display.Blit(border, pos.x, pos.y);
    const Sprite & sprite = AGG::GetICN(ICN::PRIMSKIL, index);
    pos.x = box.GetArea().x + (pos.w - sprite.w()) / 2;
    display.Blit(sprite, pos.x, pos.y + 6);

    Text text;

    text.Set(skill_name, Font::SMALL);
    pos.x = box.GetArea().x + (pos.w - text.w()) / 2;
    text.Blit(pos.x, pos.y + 8);

    text.Set("+1", Font::BIG);
    pos.x = box.GetArea().x + (pos.w - text.w()) / 2;
    text.Blit(pos.x, pos.y + 80);

    LocalEvent & le = LocalEvent::Get();

    Point pt;
    
    pt.x = box.GetArea().x + (box.GetArea().w - AGG::GetICN(system, 1).w()) / 2;
    pt.y = box.GetArea().y + box.GetArea().h - AGG::GetICN(system, 1).h();
    Button button(pt, system, 1, 2);

    button.Draw();

    cursor.Show();
    display.Flip();

    // message loop
    while(le.HandleEvents())
    {
	le.MousePressLeft(button) ? button.PressDraw() : button.ReleaseDraw();

        if(le.MouseClickLeft(button)){ break; }

	if(le.KeyPress(KEY_RETURN) || le.KeyPress(KEY_ESCAPE)){ break; }
    }

    cursor.Hide();
}
