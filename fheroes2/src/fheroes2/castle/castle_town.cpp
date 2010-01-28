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

#include <string>
#include <vector>
#include <bitset>
#include "agg.h"
#include "world.h"
#include "button.h"
#include "cursor.h"
#include "settings.h"
#include "castle.h"
#include "dialog.h"
#include "heroes.h"
#include "text.h"
#include "statusbar.h"
#include "payment.h"
#include "buildinginfo.h"
#include "kingdom.h"
#include "localclient.h"

void ShowBuildMessage(StatusBar & bar, bool isBuilt, const std::string & message, const Castle & castle, const u32 building)
{
    std::string str;
    
    if(isBuilt)
    {
        str = _("%{name} is already built");
        String::Replace(str, "%{name}", message);
    }
    else
    {
        const PaymentConditions::BuyBuilding paymentBuild(castle.GetRace(), static_cast<Castle::building_t>(building));

        if(!castle.AllowBuild())
	{
	    str = _("Cannot build. Already built here this turn.");
        }
        else
        if(castle.AllowBuild() && ! world.GetMyKingdom().AllowPayment(paymentBuild))
	{
	    str = _("Cannot afford %{name}");
    	    String::Replace(str, "%{name}", message);
        }
        else
        if(Castle::BUILD_SHIPYARD == building && !castle.HaveNearlySea())
        {
            str = _("Cannot build %{name} because castle is too far from water.");
    	    String::Replace(str, "%{name}", message);
        }
        else
        if(!castle.AllowBuyBuilding(static_cast<Castle::building_t>(building)))
        {
            str = _("Cannot build %{name}");
    	    String::Replace(str, "%{name}", message);
        }
        else
	{
	    str = _("Build %{name}");
    	    String::Replace(str, "%{name}", message);
        }
    }

    bar.ShowMessage(str);
}

Dialog::answer_t Castle::DialogBuyHero(const Heroes* hero)
{
    if(!hero) return Dialog::CANCEL;

    const ICN::icn_t system = (Settings::Get().EvilInterface() ? ICN::SYSTEME : ICN::SYSTEM);

    Display & display = Display::Get();
    Cursor & cursor = Cursor::Get();

    cursor.Hide();

    const u8 spacer = Settings::Get().PocketPC() ? 5 : 10;
    const Sprite & portrait_frame = AGG::GetICN(ICN::SURRENDR, 4);

    Text text(_("Recruit Hero"), Font::BIG);

    u8 count = hero->GetCountArtifacts();
    if(hero->HasArtifact(Artifact::MAGIC_BOOK)) count--;

    std::string str = _("%{name} is a level %{value} %{race}");

    // FIXME: It is necessary to consider locale features for numerals (with getext).
    if(count)
    {
	str += " ";
	str += count > 1 ? _(" with %{count} artifacts") : _(" with one artifact");
    }

    String::Replace(str, "%{name}", hero->GetName());
    String::Replace(str, "%{value}", hero->GetLevel());
    String::Replace(str, "%{race}", Race::String(hero->GetRace()));
    String::Replace(str, "%{count}", count);

    TextBox box2(str, Font::BIG, BOXAREA_WIDTH);

    Resource::funds_t paymentCosts(Resource::GOLD, RECRUIT_HEROES_GOLD);
    Resource::BoxSprite rbs(paymentCosts, BOXAREA_WIDTH);

    Dialog::Box box(text.h() + spacer + portrait_frame.h() + spacer + box2.h() + spacer + rbs.GetArea().h, true);
    const Rect & box_rt = box.GetArea();
    LocalEvent & le = LocalEvent::Get();
    Point dst_pt;

    dst_pt.x = box_rt.x + (box_rt.w - text.w()) / 2;
    dst_pt.y = box_rt.y;
    text.Blit(dst_pt);

    //portrait and frame
    dst_pt.x = box_rt.x + (box_rt.w - portrait_frame.w()) / 2;
    dst_pt.y = dst_pt.y + text.h() + spacer;
    display.Blit(portrait_frame, dst_pt);

    dst_pt.x = dst_pt.x + 5;
    dst_pt.y = dst_pt.y + 5;
    display.Blit(hero->GetPortrait101x93(), dst_pt);

    dst_pt.x = box_rt.x;
    dst_pt.y = dst_pt.y + portrait_frame.h() + spacer;
    box2.Blit(dst_pt);

    rbs.SetPos(dst_pt.x, dst_pt.y + box2.h() + spacer);
    rbs.Redraw();

    dst_pt.x = box_rt.x;
    dst_pt.y = box_rt.y + box_rt.h - AGG::GetICN(system, 1).h();
    Button button1(dst_pt, system, 1, 2);

    if(! AllowBuyHero())
    {
	button1.Press();
	button1.SetDisable(true);
    }

    dst_pt.x = box_rt.x + box_rt.w - AGG::GetICN(system, 3).w();
    dst_pt.y = box_rt.y + box_rt.h - AGG::GetICN(system, 3).h();
    Button button2(dst_pt, system, 3, 4);

    button1.Draw();
    button2.Draw();

    cursor.Show();
    display.Flip();

    // message loop
    while(le.HandleEvents())
    {
        le.MousePressLeft(button1) ? button1.PressDraw() : button1.ReleaseDraw();
        le.MousePressLeft(button2) ? button2.PressDraw() : button2.ReleaseDraw();

        if(button1.isEnable() &&
    	    (le.MouseClickLeft(button1) ||
    	    le.KeyPress(KEY_RETURN))) return Dialog::OK;

        if(le.MouseClickLeft(button2) ||
    	    le.KeyPress(KEY_ESCAPE)) break;
    }

    return Dialog::CANCEL;
}

Dialog::answer_t Castle::DialogBuyCastle(bool buttons) const
{
    BuildingInfo info(*this, BUILD_CASTLE);
    return info.DialogBuyBuilding(buttons) ? Dialog::OK : Dialog::CANCEL;
}

Dialog::answer_t Castle::DialogBuyCaptain(bool buttons) const
{
    BuildingInfo info(*this, BUILD_CAPTAIN);
    return info.DialogBuyBuilding(buttons) ? Dialog::OK : Dialog::CANCEL;
}

void RedrawInfoDwelling(const Point & pt, const Castle & castle, const Castle::building_t & build)
{
    Display & display = Display::Get();

    const Sprite & sprite_allow = AGG::GetICN(ICN::TOWNWIND, 11);
    const Sprite & sprite_deny  = AGG::GetICN(ICN::TOWNWIND, 12);
    const Sprite & sprite_money = AGG::GetICN(ICN::TOWNWIND, 13);
    
    Point dst_pt(pt);

    bool allowBuyBuilding = castle.AllowBuyBuilding(build);

    PaymentConditions::BuyBuilding paymentBuild(castle.GetRace(), build);

    if(Castle::BUILD_CAPTAIN & build)
    {
	// indicator
	dst_pt.x = pt.x + 65;
	dst_pt.y = pt.y + 60;
	if(castle.isBuild(build)) display.Blit(sprite_allow, dst_pt);
	else
	if(! allowBuyBuilding)
	    (1 == paymentBuild.GetValidItems() && paymentBuild.gold && castle.AllowBuild()) ? display.Blit(sprite_money, dst_pt) : display.Blit(sprite_deny, dst_pt);
    }
    else
    {
	// indicator
	dst_pt.x = pt.x + 115;
	dst_pt.y = pt.y + 40;
	if(castle.isBuild(build)) display.Blit(sprite_allow, dst_pt);
	else
	if(! allowBuyBuilding)
	    (1 == paymentBuild.GetValidItems() && paymentBuild.gold && castle.AllowBuild()) ? display.Blit(sprite_money, dst_pt) : display.Blit(sprite_deny, dst_pt);

	// status bar
	if(!castle.isBuild(build))
	{
	    dst_pt.x = pt.x - 1;
	    dst_pt.y = pt.y + 57;
	    display.Blit(AGG::GetICN(ICN::CASLXTRA, allowBuyBuilding ? 1 : 2), dst_pt);
	}

	// name
	Text text(Castle::GetStringBuilding(build, castle.GetRace()), Font::SMALL);
        dst_pt.x = pt.x + 68 - text.w() / 2;
	dst_pt.y = pt.y + 58;
	text.Blit(dst_pt);
    }
}

u32 Castle::OpenTown(void)
{
    Display & display = Display::Get();

    // cursor
    Cursor & cursor = Cursor::Get();
    cursor.Hide();

    Dialog::FrameBorder background;
    background.SetPosition((display.w() - 640 - BORDERWIDTH * 2) / 2, (display.h() - 480 - BORDERWIDTH * 2) / 2, 640, 480);
    background.Redraw();
    
    const Point cur_pt(background.GetArea().x, background.GetArea().y);
    Point dst_pt(cur_pt);

    display.Blit(AGG::GetICN(ICN::CASLWIND, 0), dst_pt);

    // hide captain options
    if(! (building & BUILD_CAPTAIN))
    {
	dst_pt.x = 530;
	dst_pt.y = 163;
	const Rect rect(dst_pt, 110, 84);
	dst_pt.x += cur_pt.x;
	dst_pt.y += cur_pt.y;
		
	display.Blit(AGG::GetICN(ICN::STONEBAK, 0), rect, dst_pt);
    }

    // draw castle sprite
    dst_pt.x = cur_pt.x + 460;
    dst_pt.y = cur_pt.y + 0;
    DrawImageCastle(dst_pt);

    //
    ICN::icn_t icn = ICN::UNKNOWN;
    
    switch(race)
    {
        case Race::BARB: icn = ICN::CSTLBARB; break;
        case Race::KNGT: icn = ICN::CSTLKNGT; break;
        case Race::NECR: icn = ICN::CSTLNECR; break;
        case Race::SORC: icn = ICN::CSTLSORC; break;
        case Race::WRLK: icn = ICN::CSTLWRLK; break;
        case Race::WZRD: icn = ICN::CSTLWZRD; break;
	default: return BUILD_NOTHING;
    }

    BuildingInfo dwelling1(*this, DWELLING_MONSTER1);
    dwelling1.SetPos(cur_pt.x + 5, cur_pt.y + 2);
    dwelling1.Redraw();

    BuildingInfo dwelling2(*this, DWELLING_MONSTER2);
    dwelling2.SetPos(cur_pt.x + 149, cur_pt.y + 2);
    dwelling2.Redraw();

    BuildingInfo dwelling3(*this, DWELLING_MONSTER3);
    dwelling3.SetPos(cur_pt.x + 293, cur_pt.y + 2);
    dwelling3.Redraw();

    BuildingInfo dwelling4(*this, DWELLING_MONSTER4);
    dwelling4.SetPos(cur_pt.x + 5, cur_pt.y + 77);
    dwelling4.Redraw();

    BuildingInfo dwelling5(*this, DWELLING_MONSTER5);
    dwelling5.SetPos(cur_pt.x + 149, cur_pt.y + 77);
    dwelling5.Redraw();

    BuildingInfo dwelling6(*this, DWELLING_MONSTER6);
    dwelling6.SetPos(cur_pt.x + 293, cur_pt.y + 77);
    dwelling6.Redraw();

    // mage guild
    building_t level = BUILD_NOTHING;
    switch(GetLevelMageGuild())
    {
	case 0: level = BUILD_MAGEGUILD1; break;
	case 1: level = BUILD_MAGEGUILD2; break;
	case 2: level = BUILD_MAGEGUILD3; break;
	case 3: level = BUILD_MAGEGUILD4; break;
	default:level = BUILD_MAGEGUILD5; break;
    }
    BuildingInfo buildingMageGuild(*this, level);
    buildingMageGuild.SetPos(cur_pt.x + 5, cur_pt.y + 157);
    buildingMageGuild.Redraw();

    // tavern
    BuildingInfo buildingTavern(*this, BUILD_TAVERN);
    buildingTavern.SetPos(cur_pt.x + 149, cur_pt.y + 157);
    buildingTavern.Redraw();

    // thieves guild
    BuildingInfo buildingThievesGuild(*this, BUILD_THIEVESGUILD);
    buildingThievesGuild.SetPos(cur_pt.x + 293, cur_pt.y + 157);
    buildingThievesGuild.Redraw();

    // shipyard
    BuildingInfo buildingShipyard(*this, BUILD_SHIPYARD);
    buildingShipyard.SetPos(cur_pt.x + 5, cur_pt.y + 232);
    buildingShipyard.Redraw();

    // statue
    BuildingInfo buildingStatue(*this, BUILD_STATUE);
    buildingStatue.SetPos(cur_pt.x + 149, cur_pt.y + 232);
    buildingStatue.Redraw();

    // marketplace
    BuildingInfo buildingMarketplace(*this, BUILD_MARKETPLACE);
    buildingMarketplace.SetPos(cur_pt.x + 293, cur_pt.y + 232);
    buildingMarketplace.Redraw();

    // well
    BuildingInfo buildingWell(*this, BUILD_WELL);
    buildingWell.SetPos(cur_pt.x + 5, cur_pt.y + 307);
    buildingWell.Redraw();

    // wel2
    BuildingInfo buildingWel2(*this, BUILD_WEL2);
    buildingWel2.SetPos(cur_pt.x + 149, cur_pt.y + 307);
    buildingWel2.Redraw();

    // spec
    BuildingInfo buildingSpec(*this, BUILD_SPEC);
    buildingSpec.SetPos(cur_pt.x + 293, cur_pt.y + 307);
    buildingSpec.Redraw();

    // left turret
    BuildingInfo buildingLTurret(*this, BUILD_LEFTTURRET);
    buildingLTurret.SetPos(cur_pt.x + 5, cur_pt.y + 387);
    buildingLTurret.Redraw();

    // right turret
    BuildingInfo buildingRTurret(*this, BUILD_RIGHTTURRET);
    buildingRTurret.SetPos(cur_pt.x + 149, cur_pt.y + 387);
    buildingRTurret.Redraw();

    // moat
    BuildingInfo buildingMoat(*this, BUILD_MOAT);
    buildingMoat.SetPos(cur_pt.x + 293, cur_pt.y + 387);
    buildingMoat.Redraw();

    // captain
    switch(race)
    {
        case Race::BARB: icn = ICN::CSTLCAPB; break;
        case Race::KNGT: icn = ICN::CSTLCAPK; break;
        case Race::NECR: icn = ICN::CSTLCAPN; break;
        case Race::SORC: icn = ICN::CSTLCAPS; break;
        case Race::WRLK: icn = ICN::CSTLCAPW; break;
        case Race::WZRD: icn = ICN::CSTLCAPZ; break;
	default: return BUILD_NOTHING;
    }
    dst_pt.x = cur_pt.x + 444;
    dst_pt.y = cur_pt.y + 165;
    u8 index = building & BUILD_CAPTAIN ? 1 : 0;
    display.Blit(AGG::GetICN(icn, index), dst_pt);
    const Rect rectCaptain(dst_pt, AGG::GetICN(icn, index).w(), AGG::GetICN(icn, index).h());
    bool allowBuyBuildCaptain = AllowBuyBuilding(BUILD_CAPTAIN);
    const std::string & stringCaptain = GetStringBuilding(BUILD_CAPTAIN, race);
    RedrawInfoDwelling(dst_pt, *this, BUILD_CAPTAIN);

    // combat format
    const bool combat_format = (Army::FORMAT_SPREAD == army.GetCombatFormat());
    const Sprite & spriteSpreadArmyFormat = AGG::GetICN(ICN::HSICONS, 9);
    const Sprite & spriteGroupedArmyFormat = AGG::GetICN(ICN::HSICONS, 10);
    const Rect rectSpreadArmyFormat(cur_pt.x + 550, cur_pt.y + 220, spriteSpreadArmyFormat.w(), spriteSpreadArmyFormat.h());
    const Rect rectGroupedArmyFormat(cur_pt.x + 585, cur_pt.y + 220, spriteGroupedArmyFormat.w(), spriteGroupedArmyFormat.h());
    const std::string descriptionSpreadArmyFormat(_("'Spread' combat formation spreads your armies from the top to the bottom of the battlefield, with at least one empty space between each army."));
    const std::string descriptionGroupedArmyFormat(_("'Grouped' combat formation bunches your army toget her in the center of your side of the battlefield."));
    const Point pointSpreadArmyFormat(rectSpreadArmyFormat.x - 1, rectSpreadArmyFormat.y - 1);
    const Point pointGroupedArmyFormat(rectGroupedArmyFormat.x - 1, rectGroupedArmyFormat.y - 1);
    SpriteCursor cursorFormat(AGG::GetICN(ICN::HSICONS, 11), combat_format ? pointSpreadArmyFormat : pointGroupedArmyFormat);
    if(BUILD_CAPTAIN &building)
    {
	Text text(_("Attack Skill") + std::string(" "), Font::SMALL);
	dst_pt.x = cur_pt.x + 535;
	dst_pt.y = cur_pt.y + 168;
	text.Blit(dst_pt);

	std::string message;
	String::AddInt(message, captain.GetAttack());
	text.Set(message);
	dst_pt.x += 90;
	text.Blit(dst_pt);
	
	text.Set(_("Defense Skill") + std::string(" "));
	dst_pt.x = cur_pt.x + 535;
	dst_pt.y += 12;
	text.Blit(dst_pt);

	message.clear();
	String::AddInt(message, captain.GetDefense());
	text.Set(message);
	dst_pt.x += 90;
	text.Blit(dst_pt);

	text.Set(_("Spell Power") + std::string(" "));
	dst_pt.x = cur_pt.x + 535;
	dst_pt.y += 12;
	text.Blit(dst_pt);

	message.clear();
	String::AddInt(message, captain.GetPower());
	text.Set(message);
	dst_pt.x += 90;
	text.Blit(dst_pt);

	text.Set(_("Knowledge") + std::string(" "));
	dst_pt.x = cur_pt.x + 535;
	dst_pt.y += 12;
	text.Blit(dst_pt);

	message.clear();
	String::AddInt(message, captain.GetKnowledge());
	text.Set(message);
	dst_pt.x += 90;
	text.Blit(dst_pt);
	
	display.Blit(spriteSpreadArmyFormat, rectSpreadArmyFormat.x, rectSpreadArmyFormat.y);
	display.Blit(spriteGroupedArmyFormat, rectGroupedArmyFormat.x, rectGroupedArmyFormat.y);

	cursorFormat.Show(combat_format ? pointSpreadArmyFormat : pointGroupedArmyFormat);
    }

    Kingdom & kingdom = world.GetMyKingdom();

    const bool many_hero = kingdom.GetHeroes().size() == KINGDOMMAXHEROES;
    const bool allow_buy_hero = AllowBuyHero();

    Heroes* hero1 = kingdom.GetRecruits().GetHero1();
    Heroes* hero2 = kingdom.GetRecruits().GetHero2();

    // first hero
    dst_pt.x = cur_pt.x + 443;
    dst_pt.y = cur_pt.y + 260;
    const Rect rectHero1(dst_pt, 102, 93);
    if(hero1)
    {
	display.Blit(hero1->GetPortrait101x93(), dst_pt);
    }
    else
	display.FillRect(0, 0, 0, rectHero1);
    // indicator
    if(many_hero || !allow_buy_hero)
    {
	dst_pt.x += 83;
	dst_pt.y += 75;
	display.Blit(AGG::GetICN(ICN::TOWNWIND, 12), dst_pt);
    }

    // second hero
    dst_pt.x = cur_pt.x + 443;
    dst_pt.y = cur_pt.y + 362;
    const Rect rectHero2(dst_pt, 102, 94);
    if(hero2)
    {
	display.Blit(hero2->GetPortrait101x93(), dst_pt);
    }
    else
	display.FillRect(0, 0, 0, rectHero2);
    // indicator
    if(many_hero || !allow_buy_hero)
    {
	dst_pt.x += 83;
	dst_pt.y += 75;
	display.Blit(AGG::GetICN(ICN::TOWNWIND, 12), dst_pt);
    }

    // bottom bar
    dst_pt.x = cur_pt.x;
    dst_pt.y = cur_pt.y + 461;
    const Sprite & bar = AGG::GetICN(ICN::CASLBAR, 0);
    display.Blit(bar, dst_pt);

    StatusBar statusBar;
    statusBar.SetCenter(dst_pt.x + bar.w() / 2, dst_pt.y + 11);

    // redraw resource panel
    RedrawResourcePanel(cur_pt);

    // button exit
    dst_pt.x = cur_pt.x + 554;
    dst_pt.y = cur_pt.y + 428;
    Button buttonExit(dst_pt, ICN::SWAPBTN, 0, 1);

    buttonExit.Draw();

    cursor.Show();
    display.Flip();

    LocalEvent & le = LocalEvent::Get();

    // message loop
    while(le.HandleEvents())
    {
        le.MousePressLeft(buttonExit) ? buttonExit.PressDraw() : buttonExit.ReleaseDraw();

        if(le.MouseClickLeft(buttonExit) || le.KeyPress(KEY_RETURN) || le.KeyPress(KEY_ESCAPE)) break;

	// click left
        if(le.MouseCursor(dwelling1.GetArea()) && dwelling1.QueueEventProcessing()) { return dwelling1(); }
        else
        if(le.MouseCursor(dwelling2.GetArea()) && dwelling2.QueueEventProcessing()) { return dwelling2(); }
        else
        if(le.MouseCursor(dwelling3.GetArea()) && dwelling3.QueueEventProcessing()) { return dwelling3(); }
        else
        if(le.MouseCursor(dwelling4.GetArea()) && dwelling4.QueueEventProcessing()) { return dwelling4(); }
        else
        if(le.MouseCursor(dwelling5.GetArea()) && dwelling5.QueueEventProcessing()) { return dwelling5(); }
        else
        if(le.MouseCursor(dwelling6.GetArea()) && dwelling6.QueueEventProcessing()) { return dwelling6(); }
	else
        if(le.MouseCursor(buildingMageGuild.GetArea()) && buildingMageGuild.QueueEventProcessing()) { return buildingMageGuild(); }
	else
        if(le.MouseCursor(buildingTavern.GetArea()) && buildingTavern.QueueEventProcessing()) { return BUILD_TAVERN; }
	else
        if(le.MouseCursor(buildingThievesGuild.GetArea()) && buildingThievesGuild.QueueEventProcessing()) { return BUILD_THIEVESGUILD; }
	else
        if(le.MouseCursor(buildingShipyard.GetArea()) && buildingShipyard.QueueEventProcessing()) { return BUILD_SHIPYARD; }
	else
        if(le.MouseCursor(buildingStatue.GetArea()) && buildingStatue.QueueEventProcessing()) { return BUILD_STATUE; }
	else
        if(le.MouseCursor(buildingMarketplace.GetArea()) && buildingMarketplace.QueueEventProcessing()) { return BUILD_MARKETPLACE; }
	else
        if(le.MouseCursor(buildingWell.GetArea()) && buildingWell.QueueEventProcessing()) { return BUILD_WELL; }
	else
        if(le.MouseCursor(buildingWel2.GetArea()) && buildingWel2.QueueEventProcessing()) { return BUILD_WEL2; }
	else
        if(le.MouseCursor(buildingSpec.GetArea()) && buildingSpec.QueueEventProcessing()) { return BUILD_SPEC; }
	else
        if(le.MouseCursor(buildingLTurret.GetArea()) && buildingLTurret.QueueEventProcessing()) { return BUILD_LEFTTURRET; }
	else
        if(le.MouseCursor(buildingRTurret.GetArea()) && buildingRTurret.QueueEventProcessing()) { return BUILD_RIGHTTURRET; }
	else
        if(le.MouseCursor(buildingMoat.GetArea()) && buildingMoat.QueueEventProcessing()) { return BUILD_MOAT; }
	else
	if(!(BUILD_CAPTAIN & building) && le.MouseClickLeft(rectCaptain) && allowBuyBuildCaptain && Dialog::OK == DialogBuyCaptain(true))
	{
		return BUILD_CAPTAIN;
	}
        else
	if((BUILD_CAPTAIN && building) && le.MouseClickLeft(rectSpreadArmyFormat) && !combat_format)
        {
            cursor.Hide();
            cursorFormat.Move(pointSpreadArmyFormat);
            cursor.Show();
            display.Flip();
            army.SetCombatFormat(Army::FORMAT_SPREAD);
#ifdef WITH_NET
            FH2LocalClient::SendArmyCombatFormation(army);
#endif
        }
	else
        if((BUILD_CAPTAIN && building) && le.MouseClickLeft(rectGroupedArmyFormat) && combat_format)
        {
            cursor.Hide();
            cursorFormat.Move(pointGroupedArmyFormat);
            cursor.Show();
            display.Flip();
            army.SetCombatFormat(Army::FORMAT_GROUPED);
#ifdef WITH_NET
            FH2LocalClient::SendArmyCombatFormation(army);
#endif
        }
	else
	if(hero1 && le.MouseClickLeft(rectHero1) &&
	    Dialog::OK == DialogBuyHero(hero1))
        {
    	    RecruitHero(hero1);

    	    return BUILD_NOTHING;
        }
	else
	if(hero2 && le.MouseClickLeft(rectHero2) &&
	    Dialog::OK == DialogBuyHero(hero2))
        {
    	    RecruitHero(hero2);

	    return BUILD_NOTHING;
        }


	// right
	if(le.MousePressRight(rectCaptain))
	{
	    if(BUILD_CAPTAIN & building)
		Dialog::Message(GetStringBuilding(BUILD_CAPTAIN, race), GetDescriptionBuilding(BUILD_CAPTAIN, race), Font::BIG);
	    else
		DialogBuyCaptain(false);
	}
        else
	if(le.MousePressRight(rectSpreadArmyFormat)) Dialog::Message(_("Spread Formation"), descriptionSpreadArmyFormat, Font::BIG);
        else
	if(le.MousePressRight(rectGroupedArmyFormat)) Dialog::Message(_("Grouped Formation"), descriptionGroupedArmyFormat, Font::BIG);
	else
	if(hero1 && le.MousePressRight(rectHero1)){ hero1->OpenDialog(true); cursor.Show(); display.Flip(); }
	else
	if(hero2 && le.MousePressRight(rectHero2)){ hero2->OpenDialog(true); cursor.Show(); display.Flip(); }

        // status info
	if(le.MouseCursor(dwelling1.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(dwelling1()), dwelling1.GetName(), *this, dwelling1());
	else
	if(le.MouseCursor(dwelling2.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(dwelling2()), dwelling2.GetName(), *this, dwelling2());
	else
	if(le.MouseCursor(dwelling3.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(dwelling3()), dwelling3.GetName(), *this, dwelling3());
	else
	if(le.MouseCursor(dwelling4.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(dwelling4()), dwelling4.GetName(), *this, dwelling4());
	else
	if(le.MouseCursor(dwelling5.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(dwelling5()), dwelling5.GetName(), *this, dwelling5());
	else
	if(le.MouseCursor(dwelling6.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(dwelling6()), dwelling6.GetName(), *this, dwelling6());
	else
	if(le.MouseCursor(buildingMageGuild.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(buildingMageGuild()), buildingMageGuild.GetName(), *this, buildingMageGuild());
	else
	if(le.MouseCursor(buildingTavern.GetArea()) && !buildingTavern.IsDisable())
	    ShowBuildMessage(statusBar, isBuild(BUILD_TAVERN), buildingTavern.GetName(), *this, BUILD_TAVERN);
	else
	if(le.MouseCursor(buildingThievesGuild.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_THIEVESGUILD), buildingThievesGuild.GetName(), *this, BUILD_THIEVESGUILD);
	else
	if(le.MouseCursor(buildingShipyard.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_SHIPYARD), buildingTavern.GetName(), *this, BUILD_SHIPYARD);
	else
	if(le.MouseCursor(buildingStatue.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_STATUE), buildingStatue.GetName(), *this, BUILD_STATUE);
	else
	if(le.MouseCursor(buildingMarketplace.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_MARKETPLACE), buildingMarketplace.GetName(), *this, BUILD_MARKETPLACE);
	else
	if(le.MouseCursor(buildingWell.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_WELL), buildingWell.GetName(), *this, BUILD_WELL);
	else
	if(le.MouseCursor(buildingWel2.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_WELL), buildingWel2.GetName(), *this, BUILD_WEL2);
	else
	if(le.MouseCursor(buildingSpec.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_SPEC), buildingSpec.GetName(), *this, BUILD_SPEC);
	else
	if(le.MouseCursor(buildingLTurret.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_LEFTTURRET), buildingLTurret.GetName(), *this, BUILD_LEFTTURRET);
	else
	if(le.MouseCursor(buildingRTurret.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_RIGHTTURRET), buildingRTurret.GetName(), *this, BUILD_RIGHTTURRET);
	else
	if(le.MouseCursor(buildingMoat.GetArea()))
	    ShowBuildMessage(statusBar, isBuild(BUILD_MOAT), buildingMoat.GetName(), *this, BUILD_MOAT);
	else
	if(le.MouseCursor(rectCaptain))
	    ShowBuildMessage(statusBar, BUILD_CAPTAIN & building, stringCaptain, *this, BUILD_CAPTAIN);
	else
	if((hero1 && le.MouseCursor(rectHero1)) ||
	   (hero2 && le.MouseCursor(rectHero2)))
	{
	    if(many_hero)
		statusBar.ShowMessage(_("Cannot recruit - you have too many Heroes."));
	    else
	    if(castle_heroes)
		statusBar.ShowMessage(_("Cannot recruit - you already have a Hero in this town."));
	    else
	    if(! allow_buy_hero)
		statusBar.ShowMessage(_("Cannot afford a Hero"));
	    else
	    if(le.MouseCursor(rectHero1))
	    {
		std::string str = _("Recruit %{name} the %{race}");
		String::Replace(str, "%{name}", hero1->GetName());
		String::Replace(str, "%{race}", Race::String(hero1->GetRace()));
	    	statusBar.ShowMessage(str);
	    }
	    else
	    if(le.MouseCursor(rectHero2))
	    {
		std::string str = _("Recruit %{name} the %{race}");
		String::Replace(str, "%{name}", hero2->GetName());
		String::Replace(str, "%{race}", Race::String(hero2->GetRace()));
	    	statusBar.ShowMessage(str);
	    }
	}
	else
	if(le.MouseCursor(rectSpreadArmyFormat))
	    statusBar.ShowMessage(_("Set garrison combat formation to 'Spread'"));
	else
	if(le.MouseCursor(rectGroupedArmyFormat))
	    statusBar.ShowMessage(_("Set garrison combat formation to 'Grouped'"));
	else
	// clear all
	    statusBar.ShowMessage(_("Castle Options"));
    }

    return BUILD_NOTHING;
}
