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
#include "settings.h"
#include "world.h"
#include "artifact.h"

#ifdef WITH_XML
#include "xmlccwrap.h"
#endif

enum { ART_DISABLED = 0x01, ART_RNDUSED = 0x02 };

struct artifactstats_t
{
    u8 bits;
    u8 extra;
    const char* name;
    const char* description;
};

artifactstats_t artifacts[] = {
	{ 0,12, _("Ultimate Book of Knowledge"), _("The Ultimate Book of Knowledge increases your knowledge by %{count}.") },
	{ 0,12, _("Ultimate Sword of Dominion"), _("The Ultimate Sword of Dominion increases your attack skill by %{count}.") },
	{ 0,12, _("Ultimate Cloak of Protection"), _("The Ultimate Cloak of Protection increases your defense skill by %{count}.") },
	{ 0,12, _("Ultimate Wand of Magic"), _("The Ultimate Wand of Magic increases your spell power by %{count}.") },
	{ 0, 6, _("Ultimate Shield"), _("The Ultimate Shield increases your attack and defense skills by %{count} each.") },
	{ 0, 6, _("Ultimate Staff"), _("The Ultimate Staff increases your spell power and knowledge by %{count} each.") },
	{ 0, 4, _("Ultimate Crown"), _("The Ultimate Crown increases each of your basic skills by %{count} points.") },
	{ 0,10, _("Golden Goose"), _("The Golden Goose brings in an income of %{count} gold per turn.") },
	{ 0, 4, _("Arcane Necklace of Magic"), _("The Arcane Necklace of Magic increases your spell power by %{count}.") },
	{ 0, 2, _("Caster's Bracelet of Magic"), _("The Caster's Bracelet of Magic increases your spell power by %{count}.") },
	{ 0, 2, _("Mage's Ring of Power"), _("The Mage's Ring of Power increases your spell power by %{count}.") },
	{ 0, 3, _("Witch's Broach of Magic"), _("The Witch's Broach of Magic increases your spell power by %{count}.") },
	{ 0, 1, _("Medal of Valor"), _("The Medal of Valor increases your morale.") },
	{ 0, 1, _("Medal of Courage"), _("The Medal of Courage increases your morale.") },
	{ 0, 1, _("Medal of Honor"), _("The Medal of Honor increases your morale.") },
	{ 0, 1, _("Medal of Distinction"), _("The Medal of Distinction increases your morale.") },
	{ 0, 2, _("Fizbin of Misfortune"), _("The Fizbin of Misfortune greatly decreases your morale.") },
	{ 0, 1, _("Thunder Mace of Dominion"), _("The Thunder Mace of Dominion increases your attack skill by %{count}.") },
	{ 0, 1, _("Armored Gauntlets of Protection"), _("The Armored Gauntlets of Protection increase your defense skill by %{count}.") },
	{ 0, 1, _("Defender Helm of Protection"), _("The Defender Helm of Protection increases your defense skill by %{count}.") },
	{ 0, 1, _("Giant Flail of Dominion"), _("The Giant Flail of Dominion increases your attack skill by %{count}.") },
	{ 0, 2, _("Ballista of Quickness"), _("The Ballista of Quickness lets your catapult fire twice per combat round.") },
	{ 0, 2, _("Stealth Shield of Protection"), _("The Stealth Shield of Protection increases your defense skill by %{count}.") },
	{ 0, 3, _("Dragon Sword of Dominion"), _("The Dragon Sword of Dominion increases your attack skill by %{count}.") },
	{ 0, 2, _("Power Axe of Dominion"), _("The Power Axe of Dominion increases your attack skill by %{count}.") },
	{ 0, 3, _("Divine Breastplate of Protection"), _("The Divine Breastplate of Protection increases your defense skill by %{count}.") },
	{ 0, 2, _("Minor Scroll of Knowledge"), _("The Minor Scroll of Knowledge increases your knowledge by %{count}.") },
	{ 0, 3, _("Major Scroll of Knowledge"), _("The Major Scroll of Knowledge increases your knowledge by %{count}.") },
	{ 0, 4, _("Superior Scroll of Knowledge"), _("The Superior Scroll of Knowledge increases your knowledge by %{count}.") },
	{ 0, 5, _("Foremost Scroll of Knowledge"), _("The Foremost Scroll of Knowledge increases your knowledge by %{count}.") },
	{ 0, 100, _("Endless Sack of Gold"), _("The Endless Sack of Gold provides you with %{count} gold per day.") },
	{ 0, 75, _("Endless Bag of Gold"), _("The Endless Bag of Gold provides you with %{count} gold per day.") },
	{ 0, 50, _("Endless Purse of Gold"), _("The Endless Purse of Gold provides you with %{count} gold per day.") },
	{ 0, 0, _("Nomad Boots of Mobility"), _("The Nomad Boots of Mobility increase your movement on land.") },
	{ 0, 0, _("Traveler's Boots of Mobility"), _("The Traveler's Boots of Mobility increase your movement on land.") },
	{ 0, 1, _("Lucky Rabbit's Foot"), _("The Lucky Rabbit's Foot increases your luck in combat.") },
	{ 0, 1, _("Golden Horseshoe"), _("The Golden Horseshoe increases your luck in combat.") },
	{ 0, 1, _("Gambler's Lucky Coin"), _("The Gambler's Lucky Coin increases your luck in combat.") },
	{ 0, 1, _("Four-Leaf Clover"), _("The Four-Leaf Clover increases your luck in combat.") },
	{ 0, 0, _("True Compass of Mobility"), _("The True Compass of Mobility increases your movement on land and sea.") },
	{ 0, 0, _("Sailor's Astrolabe of Mobility"), _("The Sailors' Astrolabe of Mobility increases your movement on sea.") },
	{ 0, 0, _("Evil Eye"), _("The Evil Eye reduces the casting cost of curse spells by half.") },
	{ 0, 2, _("Enchanted Hourglass"), _("The Enchanted Hourglass extends the duration of all your spells by %{count} turns.") },
	{ 0, 0, _("Gold Watch"), _("The Gold Watch doubles the effectiveness of your hypnotize spells.") },
	{ 0, 0, _("Skullcap"), _("The Skullcap halves the casting cost of all mind influencing spells.") },
	{ 0, 0, _("Ice Cloak"), _("The Ice Cloak halves all damage your troops take from cold spells.") },
	{ 0, 0, _("Fire Cloak"), _("The Fire Cloak halves all damage your troops take from fire spells.") },
	{ 0, 0, _("Lightning Helm"), _("The Lightning Helm halves all damage your troops take from lightning spells.") },
	{ 0, 50, _("Evercold Icicle"), _("The Evercold Icicle causes your cold spells to do %{count} percent more damage to enemy troops.") },
	{ 0, 50, _("Everhot Lava Rock"), _("The Everhot Lava Rock causes your fire spells to do %{count} percent more damage to enemy troops.") },
	{ 0, 50, _("Lightning Rod"), _("The Lightning Rod causes your lightning spells to do %{count} percent more damage to enemy troops.") },
	{ 0, 0, _("Snake-Ring"), _("The Snake Ring halves the casting cost of all your bless spells.") },
	{ 0, 0, _("Ankh"), _("The Ankh doubles the effectiveness of all your resurrect and animate spells.") },
	{ 0, 0, _("Book of Elements"), _("The Book of Elements doubles the effectiveness of all your summoning spells.") },
	{ 0, 0, _("Elemental Ring"), _("The Elemental Ring halves the casting cost of all summoning spells.") },
	{ 0, 0, _("Holy Pendant"), _("The Holy Pendant makes all your troops immune to curse spells.") },
	{ 0, 0, _("Pendant of Free Will"), _("The Pendant of Free Will makes all your troops immune to hypnotize spells.") },
	{ 0, 0, _("Pendant of Life"), _("The Pendant of Life makes all your troops immune to death spells.") },
	{ 0, 0, _("Serenity Pendant"), _("The Serenity Pendant makes all your troops immune to berserk spells.") },
	{ 0, 0, _("Seeing-eye Pendant"), _("The Seeing-eye Pendant makes all your troops immune to blindness spells.") },
	{ 0, 0, _("Kinetic Pendant"), _("The Kinetic Pendant makes all your troops immune to paralyze spells.") },
	{ 0, 0, _("Pendant of Death"), _("The Pendant of Death makes all your troops immune to holy spells.") },
	{ 0, 0, _("Wand of Negation"), _("The Wand of Negation protects your troops from the Dispel Magic spell.") },
	{ 0, 50, _("Golden Bow"), _("The Golden Bow eliminates the %{count} percent penalty for your troops shooting past obstacles. (e.g. castle walls)") },
	{ 0, 1, _("Telescope"), _("The Telescope increases the amount of terrain your hero reveals when adventuring by %{count} extra square.") },
	{ 0, 10, _("Statesman's Quill"), _("The Statesman's Quill reduces the cost of surrender to %{count} percent of the total cost of troops you have in your army.") },
	{ 0, 10, _("Wizard's Hat"), _("The Wizard's Hat increases the duration of your spells by %{count} turns!") },
	{ 0, 2, _("Power Ring"), _("The Power Ring returns %{count} extra power points/turn to your hero.") },
	{ 0, 0, _("Ammo Cart"), _("The Ammo Cart provides endless ammunition for all your troops that shoot.") },
	{ 0, 25, _("Tax Lien"), _("The Tax Lien costs you %{count} gold pieces/turn.") },
	{ 0, 0, _("Hideous Mask"), _("The Hideous Mask prevents all 'wandering' armies from joining your hero.") },
	{ 0, 1, _("Endless Pouch of Sulfur"), _("The Endless Pouch of Sulfur provides %{count} unit of sulfur per day.") },
	{ 0, 1, _("Endless Vial of Mercury"), _("The Endless Vial of Mercury provides %{count} unit of mercury per day.") },
	{ 0, 1, _("Endless Pouch of Gems"), _("The Endless Pouch of Gems provides %{count} unit of gems per day.") },
	{ 0, 1, _("Endless Cord of Wood"), _("The Endless Cord of Wood provides %{count} unit of wood per day.") },
	{ 0, 1, _("Endless Cart of Ore"), _("The Endless Cart of Ore provides %{count} unit of ore per day.") },
	{ 0, 1, _("Endless Pouch of Crystal"), _("The Endless Pouch of Crystal provides %{count} unit of crystal/day.") },
	{ 0, 1, _("Spiked Helm"), _("The Spiked Helm increases your attack and defense skills by %{count} each.") },
	{ 0, 2, _("Spiked Shield"), _("The Spiked Shield increases your attack and defense skills by %{count} each.") },
	{ 0, 1, _("White Pearl"), _("The White Pearl increases your spell power and knowledge by %{count} each.") },
	{ 0, 2, _("Black Pearl"), _("The Black Pearl increases your spell power and knowledge by %{count} each.") },

	{ 0, 0, _("Magic Book"), _("The Magic Book enables you to cast spells.") },

	{ 0, 0, "Dummy 1", "The reserved artifact." },
	{ 0, 0, "Dummy 2", "The reserved artifact." },
	{ 0, 0, "Dummy 3", "The reserved artifact." },
	{ 0, 0, "Dummy 4", "The reserved artifact." },

	{ 0, 0, _("Spell Scroll"), _("This Spell Scroll gives your hero the ability to cast the %{spell} spell.") },
	{ 0, 3, _("Arm of the Martyr"), _("The Arm of the Martyr increases your spell power by %{count} but adds the undead morale penalty.") },
	{ 0, 5, _("Breastplate of Anduran"), _("The Breastplate increases your defense by %{count}.") },
	{ 0, 50, _("Broach of Shielding"), _("The Broach of Shielding provides %{count} percent protection from Armageddon and Elemental Storm, but decreases spell power by 2.") },
	{ 0, 5, _("Battle Garb of Anduran"), _("The Battle Garb of Anduran combines the powers of the three Anduran artifacts.  It provides maximum luck and morale for your troops and gives you the Town Portal spell.") },
	{ 0, 0, _("Crystal Ball"), _("The Crystal Ball lets you get more specific information about monsters, enemy heroes, and castles nearby the hero who holds it.") },
	{ 0, 50, _("Heart of Fire"), _("The Heart of Fire provides %{count} percent protection from fire, but doubles the damage taken from cold.") },
	{ 0, 50, _("Heart of Ice"), _("The Heart of Ice provides %{count} percent protection from cold, but doubles the damage taken from fire.") },
	{ 0, 5, _("Helmet of Anduran"), _("The Helmet increases your spell power by %{count}.") },
	{ 0, 5, _("Holy Hammer"), _("The Holy Hammer increases your attack skill by %{count}.") },
	{ 0, 2, _("Legendary Scepter"), _("The Legendary Scepter adds %{count} points to all attributes.") },
	{ 0, 1, _("Masthead"), _("The Masthead boosts your luck and morale by %{count} each in sea combat.") },
	{ 0, 0, _("Sphere of Negation"), _("The Sphere of Negation disables all spell casting, for both sides, in combat.") },
	{ 0, 5, _("Staff of Wizardry"), _("The Staff of Wizardry boosts your spell power by %{count}.") },
	{ 0, 4, _("Sword Breaker"), _("The Sword Breaker increases your defense by %{count} and attack by 1.") },
	{ 0, 5, _("Sword of Anduran"), _("The Sword increases your attack skill by %{count}.") },
	{ 0, 0, _("Spade of Necromancy"), _("The Spade gives you increased necromancy skill.") },

	{ 0, 0, "Unknown", "Unknown" },
};

bool SkipExtra(u8 art)
{
    switch(art)
    {
	case Artifact::BALLISTA:
        case Artifact::NOMAD_BOOTS_MOBILITY:
        case Artifact::TRAVELER_BOOTS_MOBILITY:
        case Artifact::RABBIT_FOOT:
        case Artifact::GOLDEN_HORSESHOE:
        case Artifact::GAMBLER_LUCKY_COIN:
        case Artifact::FOUR_LEAF_CLOVER:
        case Artifact::TRUE_COMPASS_MOBILITY:
        case Artifact::SAILORS_ASTROLABE_MOBILITY:
        case Artifact::EVIL_EYE:
	case Artifact::GOLD_WATCH:
        case Artifact::SKULLCAP:
        case Artifact::ICE_CLOAK:
        case Artifact::FIRE_CLOAK:
        case Artifact::LIGHTNING_HELM:
	case Artifact::GOLDEN_BOW:
	case Artifact::TELESCOPE:

	    return true;

	default: break;
    }

    return false;
}

void Artifact::UpdateStats(const std::string & spec)
{
#ifdef WITH_XML
VERBOSE("!!!!!!!!!!!!!!!!!!!");
    // parse artifacts.xml
    TiXmlDocument doc;
    const TiXmlElement* xml_artifacts = NULL;
    artifactstats_t* ptr = &artifacts[0];

    if(doc.LoadFile(spec.c_str()) &&
        NULL != (xml_artifacts = doc.FirstChildElement("artifacts")))
    {
        const TiXmlElement* xml_artifact = xml_artifacts->FirstChildElement("artifact");
        for(; xml_artifact; xml_artifact = xml_artifact->NextSiblingElement("artifact"))
        {
            int value;
	    int art = (ptr - &artifacts[0]) / sizeof(artifactstats_t);

            xml_artifact->Attribute("disable", &value);
            if(value) ptr->bits |= ART_DISABLED;

            xml_artifact->Attribute("extra", &value);
	    if(value && !SkipExtra(art)) ptr->extra = value;

            ++ptr;

	    
            // out of range
            if(art >= UNKNOWN) break;
        }
    }
    else
    VERBOSE(spec << ": " << doc.ErrorDesc());
#endif
}

Artifact::Artifact() : id(Artifact::UNKNOWN), ext(0)
{
}

Artifact::Artifact(artifact_t a) : id(a), ext(0)
{
}

void Artifact::Set(artifact_t a)
{
    id = a;
}

bool Artifact::operator== (artifact_t a) const
{
    return id == a;
}

bool Artifact::operator!= (artifact_t a) const
{
    return id != a;
}

Artifact::artifact_t Artifact::operator() (void) const
{
    return id;
}

Artifact::artifact_t Artifact::GetID(void) const
{
    return id;
}

const char* Artifact::GetName(void) const
{
    return GetName(id);
}

const char* Artifact::GetDescription(void) const
{
    return GetDescription(id);
}

bool Artifact::isUltimate(void) const
{
    switch(id)
    {
	case ULTIMATE_BOOK:
	case ULTIMATE_SWORD:
	case ULTIMATE_CLOAK:
	case ULTIMATE_WAND:
	case ULTIMATE_SHIELD:
	case ULTIMATE_STAFF:
	case ULTIMATE_CROWN:
	case GOLDEN_GOOSE: return true;
	default: break;
    }

    return false;
}

bool Artifact::isValid(void) const
{
    return id != UNKNOWN;
}

u8 Artifact::GetIndexSprite(void) const
{
    return IndexSprite(id);
}

u8 Artifact::GetIndexSprite32(void) const
{
    return IndexSprite32(id);
}

u8 Artifact::GetIndexSprite64(void) const
{
    return IndexSprite64(id);
}

/* get rand all artifact */
Artifact::artifact_t Artifact::Rand(level_t lvl)
{
    std::vector<artifact_t> v;
    v.reserve(25);

    // if possibly: make unique on map
    for(u8 art = ULTIMATE_BOOK; art < UNKNOWN; ++art)
        if((lvl & GetLevel(FromInt(art))) &&
            !(artifacts[art].bits & ART_DISABLED) &&
	    !(artifacts[art].bits & ART_RNDUSED)) v.push_back(FromInt(art));

    //
    if(v.empty())
    {
	for(u8 art = ULTIMATE_BOOK; art < UNKNOWN; ++art)
    	if((lvl & GetLevel(FromInt(art))) &&
            !(artifacts[art].bits & ART_DISABLED)) v.push_back(FromInt(art));
    }

    artifact_t res = v.size() ? *Rand::Get(v) : Artifact::UNKNOWN;
    artifacts[res].bits |= ART_RNDUSED;

    return res;
}

u8 Artifact::GetLevelLoyalty(artifact_t art)
{
    switch(art)
    {
        case MASTHEAD:
        case SPADE_NECROMANCY:
        case CRYSTAL_BALL:
	    return ART_LEVEL2;

        case BREASTPLATE_ANDURAN:
        case BATTLE_GARB:
        case HELMET_ANDURAN:
        case HOLY_HAMMER:
        case LEGENDARY_SCEPTER:
        case SPHERE_NEGATION:
        case STAFF_WIZARDRY:
        case SWORD_BREAKER:
        case SWORD_ANDURAN:
	    return ART_LEVEL3;

	case SPELL_SCROLL:
        case ARM_MARTYR:
        case BROACH_SHIELDING:
        case HEART_FIRE:
        case HEART_ICE:
	    return ART_NOLEVEL;

	default: break;
    }

    return ART_NONE;
}

u8 Artifact::GetLevel(artifact_t art)
{
    switch(art)
    {
        case MEDAL_VALOR:
        case MEDAL_COURAGE:
        case MEDAL_HONOR:
        case MEDAL_DISTINCTION:
        case THUNDER_MACE:
        case ARMORED_GAUNTLETS:
        case DEFENDER_HELM:
        case GIANT_FLAIL:
        case RABBIT_FOOT:
        case GOLDEN_HORSESHOE:
        case GAMBLER_LUCKY_COIN:
        case FOUR_LEAF_CLOVER:
        case ENCHANTED_HOURGLASS:
        case ICE_CLOAK:
        case FIRE_CLOAK:
        case LIGHTNING_HELM:
        case SNAKE_RING:
        case HOLY_PENDANT:
        case PENDANT_FREE_WILL:
        case PENDANT_LIFE:
        case PENDANT_DEATH:
        case GOLDEN_BOW:
        case TELESCOPE:
        case SERENITY_PENDANT:
        case STATESMAN_QUILL:
        case KINETIC_PENDANT:
	case SEEING_EYE_PENDANT:
	    return ART_LEVEL1;

        case CASTER_BRACELET:
        case MAGE_RING:
        case STEALTH_SHIELD:
        case POWER_AXE:
        case MINOR_SCROLL:
        case ENDLESS_PURSE_GOLD:
        case SAILORS_ASTROLABE_MOBILITY:
        case ENDLESS_CORD_WOOD:
        case ENDLESS_CART_ORE:
        case SPIKED_HELM:
        case WHITE_PEARL:
        case EVIL_EYE:
        case GOLD_WATCH:
        case ANKH:
        case BOOK_ELEMENTS:
        case ELEMENTAL_RING:
        case SKULLCAP:
        case EVERCOLD_ICICLE:
        case POWER_RING:
        case AMMO_CART:
        case EVERHOT_LAVA_ROCK:
	    return ART_LEVEL2;

        case ARCANE_NECKLACE:
        case WITCHES_BROACH:
        case BALLISTA:
        case DRAGON_SWORD:
        case DIVINE_BREASTPLATE:
        case MAJOR_SCROLL:
        case SUPERIOR_SCROLL:
        case FOREMOST_SCROLL:
        case ENDLESS_SACK_GOLD:
        case ENDLESS_BAG_GOLD:
        case NOMAD_BOOTS_MOBILITY:
        case TRAVELER_BOOTS_MOBILITY:
        case TRUE_COMPASS_MOBILITY:
        case ENDLESS_POUCH_SULFUR:
        case ENDLESS_POUCH_GEMS:
        case ENDLESS_POUCH_CRYSTAL:
        case ENDLESS_VIAL_MERCURY:
        case SPIKED_SHIELD:
        case BLACK_PEARL:
        case LIGHTNING_ROD:
        case WAND_NEGATION:
        case WIZARD_HAT:
	    return ART_LEVEL3;

        case ULTIMATE_BOOK:
        case ULTIMATE_SWORD:
        case ULTIMATE_CLOAK:
        case ULTIMATE_WAND:
        case ULTIMATE_SHIELD:
        case ULTIMATE_STAFF:
        case ULTIMATE_CROWN:
        case GOLDEN_GOOSE:
	    return ART_ULTIMATE;

	// no level
	case MAGIC_BOOK:
	case FIZBIN_MISFORTUNE:
	case TAX_LIEN:
	case HIDEOUS_MASK:
	    return ART_NOLEVEL;

	// price loyalty
	case SPELL_SCROLL:
        case ARM_MARTYR:
        case BREASTPLATE_ANDURAN:
        case BROACH_SHIELDING:
        case BATTLE_GARB:
        case CRYSTAL_BALL:
        case HEART_FIRE:
        case HEART_ICE:
        case HELMET_ANDURAN:
        case HOLY_HAMMER:
        case LEGENDARY_SCEPTER:
        case MASTHEAD:
        case SPHERE_NEGATION:
        case STAFF_WIZARDRY:
        case SWORD_BREAKER:
        case SWORD_ANDURAN:
        case SPADE_NECROMANCY:
	    return Settings::Get().PriceLoyaltyVersion() ? ART_LOYALTY | GetLevelLoyalty(art) : ART_LOYALTY;

	default: break;
    }

    return ART_NONE;
}

const char* Artifact::GetName(artifact_t a)
{
    return _(artifacts[a].name);
}

const char* Artifact::GetDescription(artifact_t a)
{
    return _(artifacts[a].description);
}

u16 Artifact::GetExtraValue(artifact_t a)
{
    switch(a)
    {
	case GOLDEN_GOOSE:
	    return 1000 * artifacts[a].extra;

        case ENDLESS_SACK_GOLD:
        case ENDLESS_BAG_GOLD:
        case ENDLESS_PURSE_GOLD:
	case TAX_LIEN:
	    return 10 * artifacts[a].extra;

	default: break;
    }

    return artifacts[a].extra;
}

u8 Artifact::GetExt(void) const
{
    return ext;
}

void Artifact::SetExt(u8 v)
{
    ext = v;
}

Artifact::artifact_t Artifact::FromInt(u16 index)
{
    return index > UNKNOWN ? UNKNOWN : static_cast<artifact_t>(index);
}

Artifact::artifact_t Artifact::FromIndexSprite(u8 index)
{
    if(0x10 < index && 0xA2 > index) return Artifact::FromInt((index - 1)/2);
    else
    if(Settings::Get().PriceLoyaltyVersion() && 0xAB < index && 0xCE > index) return Artifact::FromInt((index - 1)/2);
    else
    if(0xA3 == index) return Rand(ART_LEVEL123);
    else
    if(0xA4 == index) return Rand(ART_ULTIMATE);
    else
    if(0xA7 == index) return Rand(ART_LEVEL1);
    else
    if(0xA9 == index) return Rand(ART_LEVEL2);
    else
    if(0xAB == index) return Rand(ART_LEVEL3);
    else
	DEBUG(DBG_GAME , DBG_WARN, "Artifact::FromIndexSprite: unknown: " << static_cast<int>(index));

    return Artifact::UNKNOWN;
}

/* return index sprite objnarti.icn */
u8 Artifact::IndexSprite(Artifact::artifact_t a)
{
    switch(a)
    {
	// null sprite
        case ULTIMATE_BOOK:			return 0x01;
	// null sprite
        case ULTIMATE_SWORD:			return 0x03;
	// null sprite
        case ULTIMATE_CLOAK:			return 0x05;
	// null sprite
	case ULTIMATE_WAND:			return 0x07;
	// sprite artifact
        case ULTIMATE_SHIELD:			return 0x09;
        case ULTIMATE_STAFF:			return 0x0B;
        case ULTIMATE_CROWN:			return 0x0D;
        case GOLDEN_GOOSE:			return 0x0F;
	case ARCANE_NECKLACE:			return 0x11;
	case CASTER_BRACELET:			return 0x13;
	case MAGE_RING:				return 0x15;
	case WITCHES_BROACH:			return 0x17;
	case MEDAL_VALOR:			return 0x19;
	case MEDAL_COURAGE:			return 0x1B;
	case MEDAL_HONOR:			return 0x1D;
	case MEDAL_DISTINCTION:			return 0x1F;
	case FIZBIN_MISFORTUNE:			return 0x21;
	case THUNDER_MACE:			return 0x23;
	case ARMORED_GAUNTLETS:			return 0x25;
	case DEFENDER_HELM:			return 0x27;
	case GIANT_FLAIL:			return 0x29;
	case BALLISTA:				return 0x2B;
	case STEALTH_SHIELD:			return 0x2D;
	case DRAGON_SWORD:			return 0x2F;
	case POWER_AXE:				return 0x31;
	case DIVINE_BREASTPLATE:		return 0x33;
	case MINOR_SCROLL:			return 0x35;
	case MAJOR_SCROLL:			return 0x37;
	case SUPERIOR_SCROLL:			return 0x39;
	case FOREMOST_SCROLL:			return 0x3B;
	case ENDLESS_SACK_GOLD:			return 0x3D;
	case ENDLESS_BAG_GOLD:			return 0x3F;
	case ENDLESS_PURSE_GOLD:		return 0x41;
	case NOMAD_BOOTS_MOBILITY:		return 0x43;
	case TRAVELER_BOOTS_MOBILITY:		return 0x45;
	case RABBIT_FOOT:			return 0x47;
	case GOLDEN_HORSESHOE:			return 0x49;
	case GAMBLER_LUCKY_COIN:		return 0x4B;
	case FOUR_LEAF_CLOVER:			return 0x4D;
	case TRUE_COMPASS_MOBILITY:		return 0x4F;
	case SAILORS_ASTROLABE_MOBILITY:	return 0x51;
	case EVIL_EYE:				return 0x53;
	case ENCHANTED_HOURGLASS:		return 0x55;
	case GOLD_WATCH:			return 0x57;
	case SKULLCAP:				return 0x59;
	case ICE_CLOAK:				return 0x5B;
	case FIRE_CLOAK:			return 0x5D;
	case LIGHTNING_HELM:			return 0x5F;
	case EVERCOLD_ICICLE:			return 0x61;
	case EVERHOT_LAVA_ROCK:			return 0x63;
	case LIGHTNING_ROD:			return 0x65;
	case SNAKE_RING:			return 0x67;
	case ANKH:				return 0x69;
	case BOOK_ELEMENTS:			return 0x6B;
	case ELEMENTAL_RING:			return 0x6D;
	case HOLY_PENDANT:			return 0x6F;
	case PENDANT_FREE_WILL:			return 0x71;
	case PENDANT_LIFE:			return 0x73;
	case SERENITY_PENDANT:			return 0x75;
	case SEEING_EYE_PENDANT:		return 0x77;
	case KINETIC_PENDANT:			return 0x79;
	case PENDANT_DEATH:			return 0x7B;
	case WAND_NEGATION:			return 0x7D;
	case GOLDEN_BOW:			return 0x7F;
	case TELESCOPE:				return 0x81;
	case STATESMAN_QUILL:			return 0x83;
	case WIZARD_HAT:			return 0x85;
	case POWER_RING:			return 0x87;
	case AMMO_CART:				return 0x89;
	case TAX_LIEN:				return 0x8B;
	case HIDEOUS_MASK:			return 0x8D;
	case ENDLESS_POUCH_SULFUR:		return 0x8F;
	case ENDLESS_VIAL_MERCURY:		return 0x91;
	case ENDLESS_POUCH_GEMS:		return 0x93;
	case ENDLESS_CORD_WOOD:			return 0x95;
	case ENDLESS_CART_ORE:			return 0x97;
	case ENDLESS_POUCH_CRYSTAL:		return 0x99;
	case SPIKED_HELM:			return 0x9B;
	case SPIKED_SHIELD:			return 0x9D;
	case WHITE_PEARL:			return 0x9F;
	case BLACK_PEARL:			return 0xA1;

	case MAGIC_BOOK:			return 0xA3;	// sprite RND_ARTIFACT
	case DUMMY1:				return 0xA4;	// sprite ULTIMA_ARTIFACT
	case DUMMY2:				return 0xA7;	// sprite RND1_ARTIFACT
	case DUMMY3:				return 0xA9;	// sprite RND2_ARTIFACT
	case DUMMY4:				return 0xAB;	// sprite RND3_ARTIFACT

	default: break;
    }

    if(Settings::Get().PriceLoyaltyVersion())
    switch(a)
    {
	case SPELL_SCROLL:			return 0xAD;
	case ARM_MARTYR:			return 0xAF;
	case BREASTPLATE_ANDURAN:		return 0xB1;
	case BROACH_SHIELDING:			return 0xB3;
	case BATTLE_GARB:			return 0xB5;
	case CRYSTAL_BALL:			return 0xB7;
	case HEART_FIRE:			return 0xB9;
	case HEART_ICE:				return 0xBB;
	case HELMET_ANDURAN:			return 0xBD;
	case HOLY_HAMMER:			return 0xBF;
	case LEGENDARY_SCEPTER:			return 0xC1;
	case MASTHEAD:				return 0xC3;
	case SPHERE_NEGATION:			return 0xC5;
	case STAFF_WIZARDRY:			return 0xC7;
	case SWORD_BREAKER:			return 0xC9;
	case SWORD_ANDURAN:			return 0xCB;
	case SPADE_NECROMANCY:			return 0xCD;

	default: break;
    }

    DEBUG(DBG_GAME , DBG_WARN, "Artifact::IndexSprite: unknown:" <<  static_cast<int>(a));

    // null sprite
    return 0;
}

u8 Artifact::IndexSprite32(Artifact::artifact_t a)
{
    return a;
}

u8 Artifact::IndexSprite64(Artifact::artifact_t a)
{
    return a + 1;
}

const char* Artifact::GetScenario(artifact_t art)
{
    switch(art)
    {
	case SPELL_SCROLL: return _("You find an elaborate aontainer which housesan old vellum scroll. The runes on the container are very old, and the artistry with whitch it was put together is stunning. As you pull the scroll out, you feel imbued with magical power.");
        case ARM_MARTYR: return _("One of the less intelligent members of your party picks up an arm off of the ground. Despite its missing a body, it is still moving. Your troops find the dismembered arm repulsive, but you cannot bring yourself to drop it: it seems to hold some sort of magical power that influences your decision making.");
        case BREASTPLATE_ANDURAN: return _("You come upon a sign. It reads: \"Here lies the body of Anduran. Bow and swear fealty, and you shall be rewarded.\" You decide to do as it says. As you stand up, you feel a coldness against your skin. Looking down, you find that you are suddenly wearing a gleaming, ornate breastplate.");
        case BROACH_SHIELDING: return _("A kindly Sorceress thinks that your army's defenses could use a magical boost. She offers to enchant the Broach that you wear on your cloak, and you accept.");
        case BATTLE_GARB: return _("Out of pity for a poor peasant, you purchase a chest of old junk they are hawking for too much gold. Later, as you search through it, you find it contains the 3 pieces of the legendary battle garb of Anduran!");
	case CRYSTAL_BALL: return _("You come upon a caravan of gypsies who are feasting and fortifying their bodies with mead. They call you forward and say \"If you prove that you can dance the Rama-Buta, we will reward you.\" You don't know it, but try anyway. They laugh hysterically, but admire your bravery, giving you a Crystal Ball.");
        case HEART_FIRE: return _("You enter a recently burned glade and come upon a Fire Elemental sitting atop a rock. It looks up, its flaming face contorted in a look of severe pain. It then tosses a glowing object at you. You put up your hands to block it, but it passes right through them and sears itself into your chest.");
        case HEART_ICE: return _("Suddenly, a biting coldness engulfs your body. You seize up, falling from your horse. The pain subsides, but you still feel as if your chest is frozen.  As you pick yourself up off of the ground, you hear hearty laughter. You turn around just in time to see a Frost Giant run off into the woods and disappear.");
        case HELMET_ANDURAN: return _("You spy a gleaming object poking up out of the ground. You send a member of your party over to investigate. He comes back with a golden helmet in his hands. You realize that it must be the helmet of the legendary Anduran, the only man who was known to wear solid gold armor.");
        case HOLY_HAMMER: return _("You come upon a battle where a Paladin has been mortally wounded by a group of Zombies. He asks you to take his hammer and finish what he started.  As you pick it up, it begins to hum, and then everything becomes a blur. The Zombies lie dead, the hammer dripping with blood. You strap it to your belt.");
        case LEGENDARY_SCEPTER: return _("Upon cresting a small hill, you come upon a ridiculous looking sight. A Sprite is attempting to carry a Scepter that is almost as big as it is. Trying not to laugh, you ask, \"Need help?\" The Sprite glares at you and answers: \"You think this is funny? Fine. You can carry it. I much prefer flying anyway.\"");
        case MASTHEAD: return _("An old seaman tells you a tale of an enchanted masthead that he used in his youth to rally his crew during times of trouble. He then hands you a faded map that shows where he hid it. After much exploring, you find it stashed underneath a nearby dock.");
        case SPHERE_NEGATION: return _("You stop to help a Peasant catch a runaway mare. To show his gratitude, he hands you a tiny sphere. As soon as you grasp it, you feel the magical energy drain from your limbs...");
        case STAFF_WIZARDRY: return _("While out scaring up game, your troops find a mysterious staff levitating about three feet off of the ground. They hand it to you, and you notice an inscription. It reads: \"Brains best brawn and magic beats might. Heed my words, and you'll win every fight.\"");
        case SWORD_BREAKER: return _("A former Captain of the Guard admires your quest and gives you the enchanted Sword Breaker that he relied on during his tour of duty.");
        case SWORD_ANDURAN: return _("A Troll stops you and says: \"Pay me 5,000 gold, or the Sword of Anduran will slay you where you stand.\" You refuse. The troll grabs the sword hanging from its belt, screams in pain, and runs away. Picking up the fabled sword, you give thanks that half-witted Trolls tend to grab the wrong end of sharp objects.");
        case SPADE_NECROMANCY: return _("A dirty shovel has been thrust into a dirt mound nearby. Upon investigation, you discover it to be the enchanted shovel of the Gravediggers, long thought lost by mortals.");

	default: break;
    }

    return NULL;
}

