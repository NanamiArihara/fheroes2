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

#include <cstring>
#include <locale>
#include <algorithm>
#include <fstream>
#include "difficulty.h"
#include "color.h"
#include "race.h"
#include "world.h"
#include "settings.h"
#include "dir.h"
#include "artifact.h"
#include "maps_fileinfo.h"

#define LENGTHNAME		16
#define LENGTHDESCRIPTION	143

#ifdef WITH_XML
#include "xmlccwrap.h"
#endif

template <typename CharType>
bool AlphabeticalCompare(const std::basic_string<CharType> & lhs, const std::basic_string<CharType> & rhs)
{
    return std::use_facet< std::collate< CharType > >( std::locale() ).compare( lhs.data(), lhs.data() + lhs.size(),
		    rhs.data(), rhs.data() + rhs.size() ) == -1;
}

namespace Editor
{
    enum { Wins = 0x1000, CaptureTown = 0x1001, DefeatHero = 0x1002, FindArtifact = 0x1003, SideWins = 0x1004, AccumulateGold = 0x1005,
    	CompAlsoWins = 0x0100, AllowNormalVictory = 0x0200,
    	Loss = 0x2000, LoseTown = 0x2001, LoseHero = 0x2002, OutTime = 0x2003 };
}

u8 ByteToColor(u8 byte)
{
    switch(byte)
    {
        case 0: return Color::BLUE;
        case 1: return Color::GREEN;
        case 2: return Color::RED;
        case 3: return Color::YELLOW;
        case 4: return Color::ORANGE;
        case 5: return Color::PURPLE;

        default: break;
    }

    return Color::NONE;
}

u8 ByteToRace(u8 byte)
{
    switch(byte)
    {
	case 0x00:	return Race::KNGT;
	case 0x01:	return Race::BARB;
	case 0x02:	return Race::SORC;
	case 0x03:	return Race::WRLK;
	case 0x04:	return Race::WZRD;
	case 0x05:	return Race::NECR;
	case 0x06:	return Race::MULT;
	case 0x07:	return Race::RAND;

	default: 	break;
    }

    return Race::NONE;
}

Maps::FileInfo::FileInfo()
{
    Reset();
}

void Maps::FileInfo::Reset(void)
{
    file.clear();
    name.clear();
    description.clear();
    size_w = 0;
    size_h = 0;
    difficulty = 0;
    kingdom_colors = 0;
    allow_human_colors = 0;
    allow_comp_colors = 0;
    rnd_races = 0;
    conditions_wins = 0;
    comp_also_wins = false;
    allow_normal_victory = false;
    wins1 = 0;
    wins2 = 0;
    conditions_loss = 0;
    loss1 = 0;
    loss2 = 0;
    localtime = 0;
    with_heroes = false;

    for(u8 ii = 0; ii < KINGDOMMAX; ++ii)
    {
	races[ii] = Race::NONE;
	unions[ii] = ByteToColor(ii);
    }
}

bool Maps::FileInfo::ReadSAV(const std::string & filename)
{
    Reset();
    return Game::LoadSAV2FileInfo(filename,  *this);
}

bool Maps::FileInfo::ReadMAP(const std::string & filename)
{
#ifdef WITH_XML
    Reset();

    TiXmlDocument doc;
    const TiXmlElement* xml_map = NULL;

    if(doc.LoadFile(filename.c_str()) &&
        NULL != (xml_map = doc.FirstChildElement("map")))
    {
	const TiXmlElement* xml_header = xml_map->FirstChildElement("header");
	if(! xml_header)
	{
	    DEBUG(DBG_GAME, DBG_WARN, filename << ", " << "header not found");
	    return false;
	}

	const TiXmlElement* xml;

	xml = xml_header->FirstChildElement("name");
	if(xml) name = xml->GetText();

	xml = xml_header->FirstChildElement("description");
	if(xml) description = xml->GetText();

	xml = xml_header->FirstChildElement("info");

	if(xml)
	{
	    std::string info = xml->GetText();
	    std::vector<u8> bytes = decodeBase64(info);
	
	    if(bytes.size() >= 89)
	    {
		ReadLE32(& bytes[0]); // version
		localtime = ReadLE32(& bytes[4]);
		size_w = ReadLE32(& bytes[8]);
		size_h = ReadLE32(& bytes[12]);

		switch(ReadLE32(& bytes[16]))
		{
		    case 0x00:	difficulty = Difficulty::EASY; break;
		    case 0x01:  difficulty = Difficulty::NORMAL; break;
		    case 0x02:	difficulty = Difficulty::HARD; break;
		    case 0x03:	difficulty = Difficulty::EXPERT; break;
		    default:	difficulty = Difficulty::NORMAL; break;
		}

		kingdom_colors = ReadLE32(& bytes[20]);
		allow_human_colors = ReadLE32(& bytes[24]);
		allow_comp_colors = ReadLE32(& bytes[28]);

		for(int col = 0; col < 6; ++col)
		{
		    int v = ReadLE32(& bytes[32] + col * 4);
		    races[col] = 1 < CountBits(v) ? Race::MULT : v;

		    if(v & Race::RAND)
		    switch(col)
		    {
			case 0: rnd_races |= Color::BLUE; break;
			case 1: rnd_races |= Color::GREEN; break;
			case 2: rnd_races |= Color::RED; break;
			case 3: rnd_races |= Color::YELLOW; break;
			case 4: rnd_races |= Color::ORANGE; break;
			case 5: rnd_races |= Color::PURPLE; break;
		        default: break;
		    }
		}

		with_heroes = 0 != bytes[56];

		int cond1[4], cond2[4];

		for(int it = 0; it < 4; ++it)
		    cond1[it] = ReadLE32(& bytes[57 + it * 4]);

		for(int it = 0; it < 4; ++it)
		    cond2[it] = ReadLE32(& bytes[73 + it * 4]);

		comp_also_wins = cond1[0] & Editor::CompAlsoWins;
		allow_normal_victory = cond1[0] & Editor::AllowNormalVictory;

		switch(cond1[0] & 0x10FF)
		{
		    case Editor::Wins:
			conditions_wins = 0;
			break;

		    case Editor::CaptureTown:
	    		conditions_wins = 1;
			wins1 = cond1[2];
			wins2 = cond1[3];
			break;

		    case Editor::DefeatHero:
			conditions_wins = 2;
			wins1 = cond1[2];
			wins2 = cond1[3];
			break;

		    case Editor::FindArtifact:
			conditions_wins = 3;
			wins1 = cond1[2];
			break;

		    case Editor::SideWins:
			conditions_wins = 4;
			//wins1 =; FIX:: Editor::ConditionSidePart
			break;

		    case Editor::AccumulateGold:
			conditions_wins = 5;
			wins1 = cond1[2] / 1000;
			break;

		    default: break;
		}

		switch(cond2[0] & 0x20FF)
		{
		    case Editor::Loss:
			conditions_loss = 0;
			break;

		    case Editor::LoseTown:
			conditions_loss = 1;
			loss1 = cond2[2];
			loss2 = cond2[3];
			break;

		    case Editor::LoseHero:
			conditions_loss = 2;
			loss1 = cond2[2];
			loss2 = cond2[3];
			break;

		    case Editor::OutTime:
			conditions_loss = 3;
			loss1 = cond2[2];
			break;

		    default: break;
		}

		file = filename;

		return true;
	    }
	    else
	    {
		DEBUG(DBG_GAME, DBG_WARN, filename << ", " << "incorrect header decode" << ", " << "size: " << bytes.size());
	    }
	}

	return false;
    }
    else
    VERBOSE(filename << ": " << doc.ErrorDesc());
#else
    DEBUG(DBG_GAME, DBG_WARN, filename << ", " << "unsupported map format");
#endif
    return false;
}

bool Maps::FileInfo::ReadMP2(const std::string & filename)
{
    Reset();
    std::ifstream fd(filename.c_str(), std::ios::binary);

    if(!fd.is_open())
    {
	DEBUG(DBG_GAME, DBG_WARN, "file not found " << filename);
	return false;
    }

    file = filename;
    kingdom_colors = 0;
    allow_human_colors = 0;
    allow_comp_colors = 0;
    rnd_races = 0;
    localtime = 0;

    u8  byte8;
    u16 byte16;
    u32 byte32;

    // magic byte
    fd.read(reinterpret_cast<char *>(&byte32), sizeof(byte32));
    SwapLE32(byte32);

    if(byte32 != 0x0000005C)
    {
	DEBUG(DBG_GAME, DBG_WARN, "incorrect maps file " << filename);
	fd.close();
	return false;
    }

    // level
    fd.read(reinterpret_cast<char *>(&byte16), sizeof(byte16));
    SwapLE16(byte16);

    switch(byte16)
    {
	case 0x00:	difficulty = Difficulty::EASY; break;
	case 0x01:  	difficulty = Difficulty::NORMAL; break;
	case 0x02:	difficulty = Difficulty::HARD; break;
	case 0x03:	difficulty = Difficulty::EXPERT; break;
	default:	difficulty = Difficulty::NORMAL; break;
    }

    // width
    fd.read(reinterpret_cast<char *>(&byte8), 1);
    size_w = byte8;

    // height
    fd.read(reinterpret_cast<char *>(&byte8), 1);
    size_h = byte8;

    Colors colors(Color::ALL);

    // kingdom color - blue, green, red, yellow, orange, purple
    for(Colors::const_iterator
	it = colors.begin(); it != colors.end(); ++it)
    {
	fd.read(reinterpret_cast<char *>(&byte8), 1);
	if(byte8) kingdom_colors |= *it;
    }

    // allow human color - blue, green, red, yellow, orange, purple
    for(Colors::const_iterator
	it = colors.begin(); it != colors.end(); ++it)
    {
	fd.read(reinterpret_cast<char *>(&byte8), 1);
	if(byte8) allow_human_colors |= *it;
    }

    // allow comp color - blue, green, red, yellow, orange, purple
    for(Colors::const_iterator
	it = colors.begin(); it != colors.end(); ++it)
    {
	fd.read(reinterpret_cast<char *>(&byte8), 1);
	if(byte8) allow_comp_colors |= *it;
    }

    // kingdom count
    // fd.seekg(0x1A, std::ios_base::beg);
    // fd.read(&byte8, 1);

    // wins
    fd.seekg(0x1D, std::ios_base::beg);
    fd.read(reinterpret_cast<char *>(&byte8), 1);
    conditions_wins = byte8;

    // data wins
    fd.read(reinterpret_cast<char *>(&byte8), 1);
    comp_also_wins = byte8;
    // data wins
    fd.read(reinterpret_cast<char *>(&byte8), 1);
    allow_normal_victory = byte8;
    // data wins
    fd.read(reinterpret_cast<char *>(&byte16), 2);
    SwapLE16(byte16);
    wins1 = byte16;
    // data wins
    fd.seekg(0x2c, std::ios_base::beg);
    fd.read(reinterpret_cast<char *>(&byte16), 2);
    SwapLE16(byte16);
    wins2 = byte16;

    // loss
    fd.seekg(0x22, std::ios_base::beg);
    fd.read(reinterpret_cast<char *>(&byte8), 1);
    conditions_loss = byte8;
    // data loss
    fd.read(reinterpret_cast<char *>(&byte16), 2);
    SwapLE16(byte16);
    loss1 = byte16;
    // data loss
    fd.seekg(0x2e, std::ios_base::beg);
    fd.read(reinterpret_cast<char *>(&byte16), 2);
    SwapLE16(byte16);
    loss2 = byte16;

    // start with hero
    fd.seekg(0x25, std::ios_base::beg);
    fd.read(reinterpret_cast<char *>(&byte8), 1);
    with_heroes = 0 == byte8;

    // race color
    for(Colors::const_iterator
	it = colors.begin(); it != colors.end(); ++it)
    {
	fd.read(reinterpret_cast<char *>(&byte8), 1);
	u8 race = ByteToRace(byte8);
	races[Color::GetIndex(*it)] = race;
	if(Race::RAND == race) rnd_races |= *it;
    }

    // name
    char bufname[LENGTHNAME];
    fd.seekg(0x3A, std::ios_base::beg);
    fd.read(bufname, LENGTHNAME);
    bufname[LENGTHNAME - 1] = 0;
    name = Game::GetEncodeString(bufname);

    // description
    char bufdescription[LENGTHDESCRIPTION];
    fd.seekg(0x76, std::ios_base::beg);
    fd.read(bufdescription, LENGTHDESCRIPTION);
    bufdescription[LENGTHDESCRIPTION - 1] = 0;
    description = Game::GetEncodeString(bufdescription);

    fd.close();
    
    //fill unions
    if(4 == conditions_wins)
	FillUnions();

    return true;
}

void Maps::FileInfo::FillUnions(void)
{
    u8 side1 = 0;
    u8 side2 = 0;

    const Colors colors(kingdom_colors);

    for(Colors::const_iterator
	it = colors.begin(); it != colors.end(); ++it)
    {
	if(Color::GetIndex(*it) < wins1)
	    side1 |= *it;
	else
	    side2 |= *it;
    }

    for(u8 ii = 0; ii < KINGDOMMAX; ++ii)
    {
	u8 cl = ByteToColor(ii);
	
	if(side1 & cl)
	    unions[ii] = side1;
	else
	if(side2 & cl)
	    unions[ii] = side2;
	else
	    unions[ii] = cl;
    }
}

bool Maps::FileInfo::FileSorting(const FileInfo & fi1, const FileInfo & fi2)
{
    return AlphabeticalCompare(fi1.file, fi2.file);
}

bool Maps::FileInfo::NameSorting(const FileInfo & fi1, const FileInfo & fi2)
{
    return AlphabeticalCompare(fi1.name, fi2.name);
}

bool Maps::FileInfo::NameCompare(const FileInfo & fi1, const FileInfo & fi2)
{
    return fi1.name == fi2.name;
}

u8 Maps::FileInfo::KingdomRace(u8 color) const
{
    switch(color)
    {
        case Color::BLUE:       return races[0];
        case Color::GREEN:      return races[1];
	case Color::RED:        return races[2];
        case Color::YELLOW:     return races[3];
        case Color::ORANGE:     return races[4];
        case Color::PURPLE:     return races[5];
        default: break;
    }
    return 0;
}

u16 Maps::FileInfo::ConditionWins(void) const
{
    switch(conditions_wins)
    {
	case 0: return GameOver::WINS_ALL;
	case 1:	return allow_normal_victory ? GameOver::WINS_TOWN | GameOver::WINS_ALL : GameOver::WINS_TOWN;
	case 2:	return allow_normal_victory ? GameOver::WINS_HERO | GameOver::WINS_ALL : GameOver::WINS_HERO;
	case 3:	return allow_normal_victory ? GameOver::WINS_ARTIFACT | GameOver::WINS_ALL : GameOver::WINS_ARTIFACT;
	case 4:	return GameOver::WINS_SIDE;
	case 5:	return allow_normal_victory ? GameOver::WINS_GOLD | GameOver::WINS_ALL : GameOver::WINS_GOLD;
	default: break;
    }

    return GameOver::COND_NONE;
}

u16 Maps::FileInfo::ConditionLoss(void) const
{
    switch(conditions_loss)
    {
	case 0: return GameOver::LOSS_ALL;
        case 1:	return GameOver::LOSS_TOWN;
        case 2: return GameOver::LOSS_HERO;
        case 3: return GameOver::LOSS_TIME;
        default: break;
    }

    return GameOver::COND_NONE;
}

bool Maps::FileInfo::WinsCompAlsoWins(void) const
{
    return comp_also_wins && ((GameOver::WINS_TOWN | GameOver::WINS_GOLD) & ConditionWins());
}

bool Maps::FileInfo::WinsAllowNormalVictory(void) const
{
    return allow_normal_victory && ((GameOver::WINS_TOWN | GameOver::WINS_ARTIFACT | GameOver::WINS_GOLD) & ConditionWins());
}

u8 Maps::FileInfo::WinsFindArtifactID(void) const
{
    return wins1 ? wins1 - 1 : Artifact::UNKNOWN;
}

bool Maps::FileInfo::WinsFindUltimateArtifact(void) const
{
    return 0 == wins1;
}

u32 Maps::FileInfo::WinsAccumulateGold(void) const
{
    return wins1 * 1000;
}

u32 Maps::FileInfo::WinsMapsIndexObject(void) const
{
    return wins2 * world.w() + wins1;
}

u32 Maps::FileInfo::LossMapsIndexObject(void) const
{
    return loss2 * world.w() + loss1;
}

u16 Maps::FileInfo::LossCountDays(void) const
{
    return loss1;
}

u8 Maps::FileInfo::AllowCompHumanColors(void) const
{
    return allow_human_colors & allow_comp_colors;
}

u8 Maps::FileInfo::AllowHumanColors(void) const
{
    return allow_human_colors;
}

u8 Maps::FileInfo::AllowComputerColors(void) const
{
    return allow_comp_colors;
}

u8 Maps::FileInfo::HumanOnlyColors(void) const
{
    return allow_human_colors & ~(allow_comp_colors);
}

u8 Maps::FileInfo::ComputerOnlyColors(void) const
{
    return allow_comp_colors & ~(allow_human_colors);
}

bool Maps::FileInfo::isAllowCountPlayers(u8 colors) const
{
    const u8 human_only = Color::Count(HumanOnlyColors());
    const u8 comp_human = Color::Count(AllowCompHumanColors());

    return human_only <= colors && colors <= human_only + comp_human;
}

bool Maps::FileInfo::isMultiPlayerMap(void) const
{
    return 1 < Color::Count(HumanOnlyColors());
}

std::string Maps::FileInfo::String(void) const
{
    std::ostringstream os;

    os << "file: " << file << ", " << "name: " << name << ", " << "kingdom colors: " << static_cast<int>(kingdom_colors) << \
	", " << "allow human colors: " << static_cast<int>(allow_human_colors) << ", " << "allow comp colors: " << static_cast<int>(allow_comp_colors) << ", " << "rnd races: " << \
	static_cast<int>(rnd_races) << ", " << "conditions wins: " << static_cast<int>(conditions_wins) << ", " << "comp also wins: " << (comp_also_wins ? "true" : "false") << \
	", " << "allow normal victory: " << (allow_normal_victory ? "true" : "false") << ", " << "wins1: " << wins1 << ", " << "wins2: " << wins2 << ", " << "conditions loss: " << static_cast<int>(conditions_loss) << \
        ", " << "loss1: " << loss1 << ", " << "loss2: " << loss2;

    return os.str();
}

ListFiles GetMapsFiles(const char* suffix)
{
    const Settings & conf = Settings::Get();
    ListFiles maps = conf.GetListFiles("maps", suffix);
    const ListDirs & list = conf.GetMapsParams();

    if(! list.empty())
    {
	for(ListDirs::const_iterator
	    it = list.begin(); it != list.end(); ++it)
	    if(*it != "maps")
    		maps.Append(conf.GetListFiles(*it, suffix));
    }

    return maps;
}

bool PrepareMapsFileInfoList(MapsFileInfoList & lists, bool multi)
{
    const Settings & conf = Settings::Get();

    ListFiles maps_old = GetMapsFiles(".mp2");
    if(conf.PriceLoyaltyVersion())
        maps_old.Append(GetMapsFiles(".mx2"));

    for(ListFiles::const_iterator
	it = maps_old.begin(); it != maps_old.end(); ++it)
    {
	Maps::FileInfo fi;
	if(fi.ReadMP2(*it)) lists.push_back(fi);
    }

#ifdef WITH_XML
    ListFiles maps_new = GetMapsFiles(".map");

    for(ListFiles::const_iterator
	it = maps_new.begin(); it != maps_new.end(); ++it)
    {
	Maps::FileInfo fi;
	if(fi.ReadMAP(*it)) lists.push_back(fi);
    }
#endif

    if(lists.empty()) return false;

    std::sort(lists.begin(), lists.end(), Maps::FileInfo::NameSorting);
    lists.resize(std::unique(lists.begin(), lists.end(), Maps::FileInfo::NameCompare) - lists.begin());

    if(multi == false)
    {
	MapsFileInfoList::iterator it = std::remove_if(lists.begin(), lists.end(),
				    std::mem_fun_ref(&Maps::FileInfo::isMultiPlayerMap));
	if(it != lists.begin()) lists.resize(std::distance(lists.begin(), it));
    }

    // set preferably count filter
    if(conf.PreferablyCountPlayers())
    {

	MapsFileInfoList::iterator it = std::remove_if(lists.begin(), lists.end(),
		std::not1(std::bind2nd(std::mem_fun_ref(&Maps::FileInfo::isAllowCountPlayers), conf.PreferablyCountPlayers())));
	if(it != lists.begin()) lists.resize(std::distance(lists.begin(), it));
    }

    return lists.size();
}

StreamBase & Maps::operator<< (StreamBase & msg, const FileInfo & fi)
{
    msg << System::GetBasename(fi.file) << fi.name << fi.description <<
	fi.size_w << fi.size_h << fi.difficulty << static_cast<u8>(KINGDOMMAX);

    for(u8 ii = 0; ii < KINGDOMMAX; ++ii)
	msg << fi.races[ii] << fi.unions[ii];

    msg << fi.kingdom_colors << fi.allow_human_colors << fi.allow_comp_colors <<
	fi.rnd_races << fi.conditions_wins << fi.comp_also_wins << fi.allow_normal_victory << fi.wins1 << fi.wins2 <<
	fi.conditions_loss << fi.loss1 << fi.loss2 << fi.localtime << fi.with_heroes;

    return msg;
}

StreamBase & Maps::operator>> (StreamBase & msg, FileInfo & fi)
{
    u8 kingdommax;

    msg >> fi.file >> fi.name >> fi.description >>
	fi.size_w >> fi.size_h >> fi.difficulty >> kingdommax;

    for(u8 ii = 0; ii < kingdommax; ++ii)
	msg >> fi.races[ii] >> fi.unions[ii];

    msg >> fi.kingdom_colors >> fi.allow_human_colors >> fi.allow_comp_colors >>
	fi.rnd_races >> fi.conditions_wins >> fi.comp_also_wins >> fi.allow_normal_victory >> fi.wins1 >> fi.wins2 >>
	fi.conditions_loss >> fi.loss1 >> fi.loss2 >> fi.localtime >> fi.with_heroes;

    return msg;
}
