/*************************************************************************** 
 *   Copyright (C) 2008 by Andrey Afletdinov                               * 
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
#include "audio.h"
#include "mp2.h"
#include "world.h"
#include "config.h"
#include "castle.h"
#include "error.h"
#include "monster.h"
#include "heroes.h"

// action to next cell
void Heroes::Action(void)
{
    if(path.empty()) return;

    const u16 dst_index = path.front().to_index;
    const MP2::object_t object = world.GetTiles(dst_index).GetObject();

    switch(object)
    {
	case MP2::OBJ_ZERO:	MoveNext(); return;

	case MP2::OBJ_MONSTER:	ActionToMonster(dst_index); break;

        case MP2::OBJ_CASTLE:	ActionToCastle(dst_index); break;
        case MP2::OBJ_HEROES:	ActionToHeroes(dst_index); break;

        case MP2::OBJ_BOAT:	ActionToBoat(); break;
	case MP2::OBJ_COAST:	ActionToCoast(); break;

        // resource
        case MP2::OBJ_RESOURCE:	ActionToResource(dst_index, MP2::OBJ_RESOURCE); break;
        case MP2::OBJ_CAMPFIRE: ActionToResource(dst_index, MP2::OBJ_CAMPFIRE); break;

        case MP2::OBJ_ARTIFACT:
        case MP2::OBJ_ANCIENTLAMP:
        case MP2::OBJ_TREASURECHEST:
        case MP2::OBJ_SHIPWRECKSURVIROR:
        case MP2::OBJ_FLOTSAM:
    	    path.Hide();
    	    path.Reset();
	    if(H2Config::Debug()) Error::Verbose("Heroes::Action: " + std::string(MP2::StringObject(object)));
	    break;

        // shrine circle
	case MP2::OBJ_SHRINE1:
	case MP2::OBJ_SHRINE2:
        case MP2::OBJ_SHRINE3:	ActionToShrine(dst_index); break;

        // witchs hut
        case MP2::OBJ_WITCHSHUT: ActionToWitchsHut(dst_index); break;

        // info message
        case MP2::OBJ_SIGN:	ActionToSign(dst_index); break;
        case MP2::OBJ_BOTTLE:	ActionToBottle(dst_index); break;

        // luck modification
        case MP2::OBJ_FOUNTAIN: ActionToLuckObject(dst_index, MP2::OBJ_FOUNTAIN); break;
        case MP2::OBJ_FAERIERING: ActionToLuckObject(dst_index, MP2::OBJ_FAERIERING); break;
        case MP2::OBJ_IDOL:	ActionToLuckObject(dst_index, MP2::OBJ_IDOL); break;

        case MP2::OBJ_MAGICWELL: ActionToMagicWell(dst_index); break;

        case MP2::OBJ_TRADINGPOST: ActionToTradingPost(dst_index); break;

        // primary skill modification
        case MP2::OBJ_FORT:		ActionToPrimarySkillObject(dst_index, MP2::OBJ_FORT); break;
        case MP2::OBJ_MERCENARYCAMP:	ActionToPrimarySkillObject(dst_index, MP2::OBJ_MERCENARYCAMP); break;
        case MP2::OBJ_DOCTORHUT:	ActionToPrimarySkillObject(dst_index, MP2::OBJ_DOCTORHUT); break;
        case MP2::OBJ_STANDINGSTONES:	ActionToPrimarySkillObject(dst_index, MP2::OBJ_STANDINGSTONES); break;

        // morale modification
        case MP2::OBJ_OASIS:		ActionToMoraleObject(dst_index, MP2::OBJ_OASIS); break;
        case MP2::OBJ_TEMPLE:		ActionToMoraleObject(dst_index, MP2::OBJ_TEMPLE); break;
        case MP2::OBJ_BUOY:		ActionToMoraleObject(dst_index, MP2::OBJ_BUOY); break;

        // experience modification
        case MP2::OBJ_GAZEBO:		ActionToExperienceObject(dst_index, MP2::OBJ_GAZEBO); break;

        // object
        case MP2::OBJ_ALCHEMYTOWER:
        case MP2::OBJ_MINES:
	case MP2::OBJ_SAWMILL:
        case MP2::OBJ_LIGHTHOUSE:

        case MP2::OBJ_WATERMILL:
        case MP2::OBJ_WINDMILL:


	case MP2::OBJ_ARCHERHOUSE:
        case MP2::OBJ_GOBLINHUNT:
        case MP2::OBJ_DWARFCOTT:
	case MP2::OBJ_PEASANTHUNT:
        case MP2::OBJ_PEASANTHUNT2:
	case MP2::OBJ_DESERTTENT:

        case MP2::OBJ_SKELETON:
        case MP2::OBJ_DAEMONCAVE:
        case MP2::OBJ_GRAVEYARD:
        case MP2::OBJ_DRAGONCITY:
        case MP2::OBJ_OBELISK:
	case MP2::OBJ_ORACLE:
	case MP2::OBJ_DERELICTSHIP:
	case MP2::OBJ_STONELIGHTS:
        case MP2::OBJ_WAGONCAMP:
        case MP2::OBJ_WATCHTOWER:
        case MP2::OBJ_TREEHOUSE:
        case MP2::OBJ_TREECITY:
        case MP2::OBJ_RUINS:
        case MP2::OBJ_ABANDONEDMINE:
        case MP2::OBJ_TREEKNOWLEDGE:
        case MP2::OBJ_HILLFORT:
        case MP2::OBJ_HALFLINGHOLE:
        case MP2::OBJ_CRAKEDLAKE:
	case MP2::OBJ_PIRAMID:
        case MP2::OBJ_CITYDEAD:
        case MP2::OBJ_EXCAVATION:
        case MP2::OBJ_SPHINX:
        case MP2::OBJ_WAGON:
        case MP2::OBJ_ARTESIANSPRING:
        case MP2::OBJ_TROLLBRIDGE:
        case MP2::OBJ_XANADU:
        case MP2::OBJ_CAVE:
        case MP2::OBJ_LEANTO:
        case MP2::OBJ_MAGICGARDEN:
	case MP2::OBJ_OBSERVATIONTOWER:
        case MP2::OBJ_FREEMANFOUNDRY:
    	    MoveNext();
	    if(H2Config::Debug()) Error::Verbose("Heroes::Action: FIXME: " + std::string(MP2::StringObject(object)));
	    break;

	default:
	    if(H2Config::Debug()) Error::Verbose("Heroes::Action: FIXME: unknown: " + std::string(MP2::StringObject(object)));
	    break;
    }

    return;
}

void Heroes::ActionToMonster(const u16 dst_index)
{
    const Maps::Tiles & tile = world.GetTiles(dst_index);
    const Monster::monster_t monster = Monster::Monster(tile);
    //const u16 count = Monster::GetSize(tile);

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToMonster: " + GetName() + " attack monster " + Monster::String(monster));

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToMonster: FIXME: attack monster");
}

void Heroes::ActionToHeroes(const u16 dst_index)
{
    const Heroes *other_hero = world.GetHeroes(dst_index);

    if(! other_hero) return;

    if(color == other_hero->GetColor())
    {
	if(H2Config::Debug()) Error::Verbose("Heroes::ActionToHeroes: " + GetName() + " meeting " + other_hero->GetName());

	MeetingDialog(const_cast<Heroes &>(*other_hero));
    }
    else
    {
	if(H2Config::Debug()) Error::Verbose("Heroes::ActionToHeroes: " + GetName() + " attack enemy hero " + other_hero->GetName());

	if(H2Config::Debug()) Error::Verbose("Heroes::ActionToHeroes: FIXME: attack enemy hero");
    }
}

void Heroes::ActionToCastle(const u16 dst_index)
{
    const Castle *castle = world.GetCastle(dst_index);

    if(! castle) return;
    
    if(color == castle->GetColor())
    {
	MoveNext();

	if(H2Config::Debug()) Error::Verbose("Heroes::ActionToCastle: " + GetName() + " goto castle " + castle->GetName());

	Audio::Mixer::Get().Reduce();

	const_cast<Castle *>(castle)->OpenDialog();

	Audio::Mixer::Get().Enhance();
    }
    else
    {
	if(H2Config::Debug()) Error::Verbose("Heroes::ActionToCastle: " + GetName() + " attack enemy castle " + castle->GetName());

	if(H2Config::Debug()) Error::Verbose("Heroes::ActiontoCastle: FIXME: attack enemy castle");
    }
}

void Heroes::ActionToBoat(void)
{
    if(isShipMaster()) return;

    MoveNext();
    SetShipMaster(true);
    move_point = 0;

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToBoat: " + GetName() + " to boat");
}

void Heroes::ActionToCoast(void)
{
    if(! isShipMaster()) return;

    MoveNext();
    SetShipMaster(false);
    move_point = 0;

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToCoast: " + GetName() + " to coast");
}

void Heroes::ActionToResource(const u16 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    const Maps::TilesAddon *addon = tile.FindResource();

    path.Hide();
    path.Reset();

    if(addon)
    {
	Resource::funds_t resource;
	u16 count = 0;
	const u32 uniq = addon->uniq;

	switch(addon->index)
        {
    	    case 1:	count = Resource::RandCount(Resource::WOOD); resource.wood += count;  break;
    	    case 3:	count = Resource::RandCount(Resource::MERCURY); resource.mercury += count; break;
    	    case 5:	count = Resource::RandCount(Resource::ORE); resource.ore += count; break;
    	    case 7:	count = Resource::RandCount(Resource::SULFUR); resource.sulfur += count; break;
    	    case 9:	count = Resource::RandCount(Resource::CRYSTAL); resource.crystal += count; break;
    	    case 11:	count = Resource::RandCount(Resource::GEMS); resource.gems += count; break;
    	    case 13:	count = Resource::RandCount(Resource::GOLD); resource.gold += count; break;

	    default:
		if(H2Config::Debug()) Error::Warning("Heroes::ActionToResource: unknown resource, from tile: ", dst_index);
		return;
        }

	PlayPickupSound();

	world.GetKingdom(GetColor()).AddFundsResource(resource);

	tile.Remove(uniq);
	tile.SetObject(MP2::OBJ_ZERO);

	// remove shadow from left cell
	if(Maps::isValidDirection(dst_index, Direction::LEFT))
	{
	    Maps::Tiles & left_tile = world.GetTiles(Maps::GetDirectionIndex(dst_index, Direction::LEFT));

	    left_tile.Remove(uniq);
	    left_tile.Redraw();
        }

	tile.Redraw();

	if(H2Config::Debug()) Error::Verbose("Heroes::ActionToResource: " + GetName() + " pickup small resource");
    }
}

void Heroes::ActionToResource(const u16 dst_index, const MP2::object_t obj)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);

    const Maps::TilesAddon *addon = NULL;

    std::string header;
    std::string body;

    switch(obj)
    {
	case MP2::OBJ_RESOURCE:
	    ActionToResource(dst_index);
	    return;

	case MP2::OBJ_CAMPFIRE:
	    addon = tile.FindCampFire();
	    header = std::string(MP2::StringObject(obj));
	    body = "Ransacking an enemy camp, you discover a hidden cache of treasures.";
	    break;

	default: Error::Warning("Heroes::ActionToResource: unknown object: ", dst_index); return;
    }

    path.Hide();
    path.Reset();

    if(addon)
    {
	Resource::funds_t resource(obj);
	const u32 uniq = addon->uniq;

	PlayPickupSound();

	world.GetKingdom(GetColor()).AddFundsResource(resource);

	if(H2Config::Debug()) Dialog::ResourceInfo(header, body, resource);

	tile.Remove(uniq);
	tile.SetObject(MP2::OBJ_ZERO);

	// remove shadow from left cell
	if(Maps::isValidDirection(dst_index, Direction::LEFT))
	{
	    Maps::Tiles & left_tile = world.GetTiles(Maps::GetDirectionIndex(dst_index, Direction::LEFT));

	    left_tile.Remove(uniq);
	    left_tile.Redraw();
        }

	tile.Redraw();

	if(H2Config::Debug()) Error::Verbose("Heroes::ActionToResource: "  + GetName() + ": " + MP2::StringObject(obj));
    }
}

void Heroes::ActionToShrine(const u16 dst_index)
{
    MoveNext();
    Display::Get().Flip();

    const Spell::spell_t spell = world.SpellFromShrine(dst_index);

    const std::string & spell_name = Spell::String(spell);
    const u8 spell_level = Spell::Level(spell);

    std::string head;
    std::string body("You come across");

    switch(spell_level)
    {
	case 1:
	    head = "Shrine of the 1st Circle";
	    body += " a small shrine attended by a group of novice acolytes. In exchange for your protection, they agree to teach you a simple spell -";
	    break;
	case 2:
	    head = "Shrine of the 2st Circle";
	    body += " an ornate shrine attended by a group of rotund friars. In exchange for your protection, they agree to teach you a spell -";
	    break;
	case 3:
	    head = "Shrine of the 3st Circle";
	    body += " a lavish shrine attended by a group of high priests. In exchange for your protection, they agree to teach you a sophisticated spell -";
	    break;
	default: return;
    }
    
    body += "'" + spell_name + "'.";

    // check spell book
    if(!HasArtifact(Artifact::MAGIC_BOOK))
    {
	body += " Unfortunately, you have no Magic Book to record the spell with.";
	if(H2Config::MyColor() == GetColor()) Dialog::Message(head, body, Font::BIG, Dialog::OK);
	return;
    }

    // check valid level spell and wisdom skill
    if(3 == spell_level && Skill::Level::NONE == GetLevelSkill(Skill::WISDOM))
    {
	body += " Unfortunately, you do not have the wisdom to understand the spell, and you are unable to learn it.";
	if(H2Config::MyColor() == GetColor()) Dialog::Message(head, body, Font::BIG, Dialog::OK);
	return;
    }

    AppendSpellToBook(spell);

    if(H2Config::MyColor() == GetColor())
    {
	AGG::PlaySound(M82::TREASURE);
	Dialog::SpellInfo(spell_name, body, spell);
    }

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToShrine: " + GetName());
}

void Heroes::ActionToWitchsHut(const u16 dst_index)
{
    MoveNext();
    Display::Get().Flip();

    const Skill::secondary_t skill = world.SkillFromWitchsHut(dst_index);
    const std::string & skill_name = Skill::String(skill);
    const std::string head("Witch's Hut");

    // check full
    if(secondary_skills.isFull())
    {
	const std::string body("You approach the hut and observe a witch inside studying an ancient tome on " + skill_name + ". As you approach, she turns and focuses her one glass eye on you. \"You already know everything you deserve to learn!\" the witch screeches. \"NOW GET OUT OF MY HOUSE!\"");
	if(H2Config::MyColor() == GetColor()) Dialog::Message(head, body, Font::BIG, Dialog::OK);
	return;
    }

    // check present skill
    if(HasSecondarySkill(skill))
    {
	const std::string body("You approach the hut and observe a witch inside studying an ancient tome on " + skill_name + ". As you approach, she turns and speaks. \"You already know that which I would teach you. I can help you no further.\"");
	if(H2Config::MyColor() == GetColor()) Dialog::Message(head, body, Font::BIG, Dialog::OK);
	return;
    }

    LearnBasicSkill(skill);

    if(H2Config::MyColor() == GetColor())
    {
	const std::string body("An ancient and immortal witch living in a hut with bird's legs for stilts teaches you " + skill_name + " for her own inscrutable purposes.");
	Dialog::SkillInfo(skill_name, body, skill, Skill::Level::BASIC);
    }

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToWitchsHut: " + GetName());
}

void Heroes::ActionToLuckObject(const u16 dst_index, const MP2::object_t obj)
{
    MoveNext();
    Display::Get().Flip();

    const char *body_true = NULL;
    const char *body_false = NULL;

    switch(obj)
    {
        case MP2::OBJ_FOUNTAIN:
    	    body_false = "You drink from the enchanted fountain, but nothing happens.";
    	    body_true = "As you drink the sweet water, you gain luck for your next battle.";
    	    break;

        case MP2::OBJ_FAERIERING:
    	    body_false = "You enter the faerie ring, but nothing happens.";
    	    body_true = "Upon entering the mystical faerie ring, your army gains luck for its next battle.";
    	    break;

        case MP2::OBJ_IDOL:
	    body_false = "You've found an ancient and weathered stone idol. It is supposed to grant luck to visitors, but since the stars are already smiling upon you, it does nothing.";
	    body_true = "You've found an ancient and weathered stone idol. Kissing it is supposed to be lucky, so you do. The stone is very cold to the touch.";
    	    break;

    	default: return;
    }

    const std::string header(MP2::StringObject(obj));

    // check already visited
    if(isVisited(obj))
    {
	if(H2Config::MyColor() == GetColor()) Dialog::Message(header, body_false, Font::BIG, Dialog::OK);
	return;
    }

    // modify luck
    SetVisited(dst_index);

    if(H2Config::MyColor() == GetColor())
    {
	AGG::PlaySound(M82::GOODLUCK);
	Dialog::SpriteInfo(header, body_true, AGG::GetICN(ICN::EXPMRL, 0));
    }

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToLuckObject: " + GetName());
}

void Heroes::ActionToSign(const u16 dst_index)
{
    MoveNext();
    Display::Get().Flip();

    if(H2Config::MyColor() == GetColor())
	Dialog::Message("Sign", world.MessageSign(dst_index), Font::BIG, Dialog::OK);

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToSign: " + GetName());
}

void Heroes::ActionToBottle(const u16 dst_index)
{
    Maps::Tiles & tile = world.GetTiles(dst_index);
    const Maps::TilesAddon *addon = tile.FindBottle();

    path.Hide();
    path.Reset();

    if(addon)
    {
	const u32 uniq = addon->uniq;

	if(H2Config::MyColor() == GetColor())
	    Dialog::Message("Bottle", world.MessageSign(dst_index), Font::BIG, Dialog::OK);

	PlayPickupSound();

	tile.Remove(uniq);
	tile.SetObject(MP2::OBJ_ZERO);

	// remove shadow from left cell
	if(Maps::isValidDirection(dst_index, Direction::LEFT))
	{
	    Maps::Tiles & left_tile = world.GetTiles(Maps::GetDirectionIndex(dst_index, Direction::LEFT));

	    left_tile.Remove(uniq);
	    left_tile.Redraw();
        }

	tile.Redraw();

	if(H2Config::Debug()) Error::Verbose("Heroes::ActionToBottle: " + GetName() + " pickup bottle");
    }
}

void Heroes::ActionToMagicWell(const u16 dst_index)
{
    MoveNext();
    Display::Get().Flip();

    const std::string header(MP2::StringObject(MP2::OBJ_MAGICWELL));
    const u16 max_point = GetMaxSpellPoints();

    if(magic_point == max_point)
    {
	if(H2Config::MyColor() == GetColor()) Dialog::Message(header, "A drink at the well is supposed to restore your spell points, but you are already at maximum.", Font::BIG, Dialog::OK);
	return;
    }

    // check already visited
    if(isVisited(MP2::OBJ_MAGICWELL))
    {
	if(H2Config::MyColor() == GetColor()) Dialog::Message(header, "A second drink at the well in one day will not help you.", Font::BIG, Dialog::OK);
	return;
    }

    SetVisited(dst_index);
    magic_point = max_point;

    if(H2Config::MyColor() == GetColor()) Dialog::Message(header, "A drink from the well has restored your spell points to maximum.", Font::BIG, Dialog::OK);

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToMagicWell: " + GetName());
}

void Heroes::ActionToTradingPost(const u16 dst_index)
{
    MoveNext();
    Display::Get().Flip();

    if(H2Config::MyColor() == GetColor()) Dialog::Marketplace();

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToTradingPost: " + GetName());
}

void Heroes::ActionToPrimarySkillObject(const u16 dst_index, const MP2::object_t obj)
{
    MoveNext();
    Display::Get().Flip();

    const char *body_true = NULL;
    const char *body_false = NULL;
    
    Skill::primary_t skill = Skill::ATTACK;

    switch(obj)
    {
        case MP2::OBJ_FORT:
    	    skill = Skill::DEFENCE;
    	    body_false = "\"I'm sorry sir,\" The leader of the soldiers says, \"but you already know everything we have to teach.\"";
    	    body_true = "The soldiers living in the fort teach you a few new defensive tricks.";
    	    break;

        case MP2::OBJ_MERCENARYCAMP:
    	    skill = Skill::ATTACK;
    	    body_false = "You've come upon a mercenary camp practicing their tactics. \"You're too advanced for us,\" the mercenary captain says. \"We can teach nothing more.\"";
    	    body_true = "You've come upon a mercenary camp practicing their tactics. The mercenaries welcome you and your troops and invite you to train with them.";
    	    break;

        case MP2::OBJ_DOCTORHUT:
    	    skill = Skill::KNOWLEDGE;
    	    body_false = "\"Go 'way!\", the witch doctor barks, \"you know all I know.\"";
    	    body_true = "An Orcish witch doctor living in the hut deepens your knowledge of magic by showing you how to cast stones, read portents, and decipher the intricacies of chicken entrails.";
    	    break;

        case MP2::OBJ_STANDINGSTONES:
    	    skill = Skill::POWER;
    	    body_false = "You've found a group of Druids worshipping at one of their strange stone edifices. Silently, the Druids turn you away, indicating they have nothing new to teach you.";
    	    body_true = "You've found a group of Druids worshipping at one of their strange stone edifices. Silently, they teach you new ways to cast spells.";
    	    break;

    	default: return;
    }

    const std::string header(MP2::StringObject(obj));

    // check already visited
    if(isVisited(obj))
    {
	if(H2Config::MyColor() == GetColor()) Dialog::Message(header, body_false, Font::BIG, Dialog::OK);
	return;
    }

    // increase skill
    SetVisited(dst_index);

    switch(skill)
    {
        case Skill::DEFENCE:		++defence; break;
        case Skill::ATTACK:		++attack; break;
        case Skill::KNOWLEDGE:		++knowledge; break;
        case Skill::POWER:		++power; break;
    }

    if(H2Config::MyColor() == GetColor()) Dialog::SkillInfo(header, body_true, skill);

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToPrimarySkillObject: " + GetName());
}

void Heroes::ActionToMoraleObject(const u16 dst_index, const MP2::object_t obj)
{
    MoveNext();
    Display::Get().Flip();

    const char *body_true = NULL;
    const char *body_false = NULL;

    switch(obj)
    {
        case MP2::OBJ_BUOY:
    	    body_false = "Your men spot a navigational buoy, confirming that you are on course.";
    	    body_true = "Your men spot a navigational buoy, confirming that you are on course and increasing their morale.";
    	    break;

        case MP2::OBJ_OASIS:
    	    body_false = "The drink at the oasis is refreshing, but offers no further benefit. The oasis might help again if you fought a battle first.";
    	    body_true = "A drink at the oasis fills your troops with strength and lifts their spirits.  You can travel a bit further today.";
    	    break;

        case MP2::OBJ_TEMPLE:
    	    body_false = "It doesn't help to pray twice before a battle. Come back after you've fought.";
    	    body_true = "A visit and a prayer at the temple raises the morale of your troops.";
    	    break;

/*
	FIXME bud morale modification
        case MP2::OBJ_GRAVEYARD:
    	    body_false = "";
    	    body_true = "";
    	    break;

        case MP2::OBJ_SHIPWRECK:
    	    body_false = "";
    	    body_true = "";
    	    break;

        case MP2::OBJ_DERELICTSHIP:
    	    body_false = "";
    	    body_true = "";
    	    break;
*/
    	default: return;
    }

    const std::string header(MP2::StringObject(obj));

    // check already visited
    if(isVisited(obj))
    {
	if(H2Config::MyColor() == GetColor()) Dialog::Message(header, body_false, Font::BIG, Dialog::OK);
	return;
    }

    // modify morale
    SetVisited(dst_index);

    if(H2Config::MyColor() == GetColor())
    {
	AGG::PlaySound(M82::GOODMRLE);
	Dialog::SpriteInfo(header, body_true, AGG::GetICN(ICN::EXPMRL, 2));
    }

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToMoraleObject: " + GetName());
}

void Heroes::ActionToExperienceObject(const u16 dst_index, const MP2::object_t obj)
{
    MoveNext();
    Display::Get().Flip();

    const char *body_true = NULL;
    const char *body_false = NULL;
    
    u16 exp = 0;

    switch(obj)
    {
        case MP2::OBJ_GAZEBO:
    	    body_false = "An old Knight appears on the steps of the gazebo. \"I am sorry, my liege, I have taught you all I can.\"";
    	    body_true = "An old Knight appears on the steps of the gazebo. \"My liege, I will teach you all that I know to aid you in your travels.\"";
    	    exp = 1000;
    	    break;

    	default: return;
    }

    const std::string header(MP2::StringObject(obj));

    // check already visited
    if(isVisited(obj))
    {
	if(H2Config::MyColor() == GetColor()) Dialog::Message(header, body_false, Font::BIG, Dialog::OK);
	return;
    }

    // visit
    SetVisited(dst_index);

    // increase experience
    experience += exp;

    if(H2Config::MyColor() == GetColor()) Dialog::ExperienceInfo(header, body_true, exp);

    // FIXME: check level up

    if(H2Config::Debug()) Error::Verbose("Heroes::ActionToExperienceObject: " + GetName());
}
