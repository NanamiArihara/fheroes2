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

#ifndef H2ICN_H
#define H2ICN_H

#include <string>
#include "gamedefs.h"

class Sprite;

namespace ICN
{
    typedef enum
    {
	ADVBORDE,
	ADVBORD,
	ADVBTNS,
	ADVEBTNS,
	ADVMCO,
	AELEM,
	APANBKGE,
	APANBKG,
	APANELE,
	APANEL,
	ARCHER2,
	ARCHER,
	ARCH_MSL,
	ART32,
	ARTFX,
	ARTIFACT,
	BARB32,
	B_BFLG32,
	BERZERK,
	B_FLAG32,
	BIGBAR,
	BLDGXTRA,
	BLESS,
	BLIND,
	BLUEFIRE,
	BOAR,
	BOAT32,
	BOATSHAD,
	BOATWIND,
	BOOK,
	BORDEDIT,
	BOULDER,
	BRCREST,
	BROTHERS,
	BTNBAUD,
	BTNCMPGN,
	BTNCOM,
	BTNDCCFG,
	BTNDC,
	BTNEMAIN,
	BTNENEW,
	BTNESIZE,
	BTNHOTST,
	BTNMCFG,
	BTNMODEM,
	BTNMP,
	BTNNET2,
	BTNNET,
	BTNNEWGM,
	BTNSHNGL,
	BUILDING,
	BUYBUILD,
	BUYBUILE,
	CAMPBKGE,
	CAMPBKGG,
	CAMPXTRE,
	CAMPXTRG,
	CAPTCOVR,
	CASLBAR,
	CASLWIND,
	CASLXTRA,
	CASTBKGB,
	CASTBKGK,
	CASTBKGN,
	CASTBKGS,
	CASTBKGW,
	CASTBKGZ,
	CASTLEB,
	CASTLEK,
	CASTLEN,
	CASTLES,
	CASTLEW,
	CASTLEZ,
	CATAPULT,
	CAVALRYB,
	CAVALRYR,
	CBKGBEAC,
	CBKGCRCK,
	CBKGDIMT,
	CBKGDITR,
	CBKGDSRT,
	CBKGGRAV,
	CBKGGRMT,
	CBKGGRTR,
	CBKGLAVA,
	CBKGSNMT,
	CBKGSNTR,
	CBKGSWMP,
	CBKGWATR,
	CELLWIN,
	CENTAUR,
	CFLGSMAL,
	CLOP32,
	CLOUDLUK,
	CMBTCAPB,
	CMBTCAPK,
	CMBTCAPN,
	CMBTCAPS,
	CMBTCAPW,
	CMBTCAPZ,
	CMBTFLE1,
	CMBTFLE2,
	CMBTFLE3,
	CMBTHROB,
	CMBTHROK,
	CMBTHRON,
	CMBTHROS,
	CMBTHROW,
	CMBTHROZ,
	CMBTLOS1,
	CMBTLOS2,
	CMBTLOS3,
	CMBTMISC,
	CMBTSURR,
	CMSECO,
	COBJ0000,
	COBJ0001,
	COBJ0002,
	COBJ0003,
	COBJ0004,
	COBJ0005,
	COBJ0006,
	COBJ0007,
	COBJ0008,
	COBJ0009,
	COBJ0010,
	COBJ0011,
	COBJ0012,
	COBJ0013,
	COBJ0014,
	COBJ0015,
	COBJ0016,
	COBJ0017,
	COBJ0018,
	COBJ0019,
	COBJ0020,
	COBJ0021,
	COBJ0022,
	COBJ0023,
	COBJ0024,
	COBJ0025,
	COBJ0026,
	COBJ0027,
	COBJ0028,
	COBJ0029,
	COBJ0030,
	COBJ0031,
	COLDRAY,
	COLDRING,
	CONGRATS,
	COVR0001,
	COVR0002,
	COVR0003,
	COVR0004,
	COVR0005,
	COVR0006,
	COVR0007,
	COVR0008,
	COVR0009,
	COVR0010,
	COVR0011,
	COVR0012,
	COVR0013,
	COVR0014,
	COVR0015,
	COVR0016,
	COVR0017,
	COVR0018,
	COVR0019,
	COVR0020,
	COVR0021,
	COVR0022,
	COVR0023,
	COVR0024,
	CPANBKGE,
	CPANBKG,
	CPANELE,
	CPANEL,
	CREST,
	CSPANBKE,
	CSPANBKG,
	CSPANBTE,
	CSPANBTN,
	CSPANEL,
	CSTLBARB,
	CSTLCAPB,
	CSTLCAPK,
	CSTLCAPN,
	CSTLCAPS,
	CSTLCAPW,
	CSTLCAPZ,
	CSTLKNGT,
	CSTLNECR,
	CSTLSORC,
	CSTLWRLK,
	CSTLWZRD,
	CTRACK00,
	CTRACK01,
	CTRACK02,
	CTRACK03,
	CTRACK04,
	CTRACK05,
	CTRACK06,
	CURSE,
	CYCLOPS,
	DISRRAY,
	DRAGBLAK,
	DRAGBONE,
	DRAGGREE,
	DRAGRED,
	DRAGSLAY,
	DROPLISL,
	DROPLIST,
	DRUID2,
	DRUID,
	DRUIDMSL,
	DUMMY,
	DWARF2,
	DWARF,
	ECPANEL,
	EDITBTNS,
	EDITOR,
	EDITPANL,
	EELEM,
	ELECTRIC,
	ELF2,
	ELF,
	ELF__MSL,
	ESCROLL,
	ESPANBKG,
	ESPANBTN,
	ESPANEL,
	EVIW_ALL,
	EVIWDDOR,
	EVIWHROS,
	EVIWMINE,
	EVIWPUZL,
	EVIWRSRC,
	EVIWRTFX,
	EVIWTWNS,
	EVIWWRLD,
	EXPMRL,
	EXTRAOVR,
	FELEM,
	FIREBAL2,
	FIREBALL,
	FLAG32,
	FONT,
	FRNG0001,
	FRNG0002,
	FRNG0003,
	FRNG0004,
	FRNG0005,
	FRNG0006,
	FRNG0007,
	FRNG0008,
	FRNG0009,
	FRNG0010,
	FRNG0011,
	FRNG0012,
	FRNG0013,
	FROTH,
	GARGOYLE,
	G_BFLG32,
	GENIE,
	G_FLAG32,
	GHOST,
	GOBLIN,
	GOLEM2,
	GOLEM,
	GRIFFIN,
	GROUND12,
	GROUND4,
	GROUND6,
	HALFLING,
	HALFLMSL,
	HASTE,
	HEROBKG,
	HEROES,
	HEROEXTE,
	HEROEXTG,
	HEROFL00,
	HEROFL01,
	HEROFL02,
	HEROFL03,
	HEROFL04,
	HEROFL05,
	HEROFL06,
	HEROLOGE,
	HEROLOGO,
	HISCORE,
	HOURGLAS,
	HSBKG,
	HSBTNS,
	HSICONS,
	HYDRA,
	HYPNOTIZ,
	ICECLOUD,
	KEEP,
	KNGT32,
	LETTER12,
	LETTER4,
	LETTER6,
	LGNDXTRA,
	LGNDXTRE,
	LICH2,
	LICHCLOD,
	LICH,
	LICH_MSL,
	LISTBOX,
	LISTBOXS,
	LOCATORE,
	LOCATORS,
	MAGE1,
	MAGE2,
	MAGEGLDB,
	MAGEGLDK,
	MAGEGLDN,
	MAGEGLDS,
	MAGEGLDW,
	MAGEGLDZ,
	MAGIC01,
	MAGIC02,
	MAGIC03,
	MAGIC04,
	MAGIC06,
	MAGIC07,
	MAGIC08,
	MANA,
	MEDUSA,
	METEOR,
	MINICAPT,
	MINIHERO,
	MINILKMR,
	MINIMON,
	MINIPORT,
	MINISS,
	MINITOWN,
	MINOTAU2,
	MINOTAUR,
	MISC12,
	MISC4,
	MISC6,
	MOATPART,
	MOATWHOL,
	MOBILITY,
	MONH0000,
	MONH0001,
	MONH0002,
	MONH0003,
	MONH0004,
	MONH0005,
	MONH0006,
	MONH0007,
	MONH0008,
	MONH0009,
	MONH0010,
	MONH0011,
	MONH0012,
	MONH0013,
	MONH0014,
	MONH0015,
	MONH0016,
	MONH0017,
	MONH0018,
	MONH0019,
	MONH0020,
	MONH0021,
	MONH0022,
	MONH0023,
	MONH0024,
	MONH0025,
	MONH0026,
	MONH0027,
	MONH0028,
	MONH0029,
	MONH0030,
	MONH0031,
	MONH0032,
	MONH0033,
	MONH0034,
	MONH0035,
	MONH0036,
	MONH0037,
	MONH0038,
	MONH0039,
	MONH0040,
	MONH0041,
	MONH0042,
	MONH0043,
	MONH0044,
	MONH0045,
	MONH0046,
	MONH0047,
	MONH0048,
	MONH0049,
	MONH0050,
	MONH0051,
	MONH0052,
	MONH0053,
	MONH0054,
	MONH0055,
	MONH0056,
	MONH0057,
	MONH0058,
	MONH0059,
	MONH0060,
	MONH0061,
	MONH0062,
	MONH0063,
	MONH0064,
	MONH0065,
	MONS32,
	MORALEB,
	MORALEG,
	MTNCRCK,
	MTNDIRT,
	MTNDSRT,
	MTNGRAS,
	MTNLAVA,
	MTNMULT,
	MTNSNOW,
	MTNSWMP,
	MUMMY2,
	MUMMYW,
	NECR32,
	NETBOX,
	NGEXTRA,
	NGHSBKG,
	NGMPBKG,
	NGSPBKG,
	NOMAD,
	O_BFLG32,
	OBJNARTI,
	OBJNCRCK,
	OBJNDIRT,
	OBJNDSRT,
	OBJNGRA2,
	OBJNGRAS,
	OBJNHAUN,
	OBJNLAV2,
	OBJNLAV3,
	OBJNLAVA,
	OBJNMUL2,
	OBJNMULT,
	OBJNRSRC,
	OBJNSNOW,
	OBJNSWMP,
	OBJNTOWN,
	OBJNTWBA,
	OBJNTWRD,
	OBJNTWSH,
	OBJNWAT2,
	OBJNWATR,
	OBJNXTRA,
	OBJPALET,
	O_FLAG32,
	OGRE2,
	OGRE,
	ORC2,
	ORC,
	ORC__MSL,
	OVERBACK,
	OVERLAY,
	OVERVIEW,
	PALADIN2,
	PALADIN,
	PARALYZE,
	P_BFLG32,
	PEASANT,
	P_FLAG32,
	PHOENIX,
	PHYSICAL,
	PIKEMAN2,
	PIKEMAN,
	PORT0000,
	PORT0001,
	PORT0002,
	PORT0003,
	PORT0004,
	PORT0005,
	PORT0006,
	PORT0007,
	PORT0008,
	PORT0009,
	PORT0010,
	PORT0011,
	PORT0012,
	PORT0013,
	PORT0014,
	PORT0015,
	PORT0016,
	PORT0017,
	PORT0018,
	PORT0019,
	PORT0020,
	PORT0021,
	PORT0022,
	PORT0023,
	PORT0024,
	PORT0025,
	PORT0026,
	PORT0027,
	PORT0028,
	PORT0029,
	PORT0030,
	PORT0031,
	PORT0032,
	PORT0033,
	PORT0034,
	PORT0035,
	PORT0036,
	PORT0037,
	PORT0038,
	PORT0039,
	PORT0040,
	PORT0041,
	PORT0042,
	PORT0043,
	PORT0044,
	PORT0045,
	PORT0046,
	PORT0047,
	PORT0048,
	PORT0049,
	PORT0050,
	PORT0051,
	PORT0052,
	PORT0053,
	PORT0054,
	PORT0055,
	PORT0056,
	PORT0057,
	PORT0058,
	PORT0059,
	PORT0060,
	PORT0061,
	PORT0062,
	PORT0063,
	PORT0064,
	PORT0065,
	PORT0066,
	PORT0067,
	PORT0068,
	PORT0069,
	PORT0070,
	PORT0090,
	PORT0091,
	PORT0092,
	PORT0093,
	PORT0094,
	PORT0095,
	PORTCFLG,
	PORTMEDI,
	PORTXTRA,
	PRIMSKIL,
	PUZZLE,
	QWIKHERO,
	QWIKINFO,
	QWIKTOWN,
	RADAR,
	R_BFLG32,
	RECR2BKG,
	RECRBKG,
	RECRUIT,
	REDBACK,
	REDDEATH,
	REDFIRE,
	REQBKG,
	REQSBKG,
	REQUEST,
	REQUESTS,
	RESOURCE,
	RESSMALL,
	R_FLAG32,
	ROAD,
	ROC,
	ROGUE,
	ROUTE,
	SCENIBKG,
	SCROLL2,
	SCROLLCN,
	SCROLLE,
	SCROLL,
	SECSKILL,
	SHADOW32,
	SHIELD,
	SHNGANIM,
	SKELETON,
	SMALCLOD,
	SMALFONT,
	SMALLBAR,
	SORC32,
	SPANBKGE,
	SPANBKG,
	SPANBTNE,
	SPANBTN,
	SPANEL,
	SPARKS,
	SPELCO,
	SPELLINF,
	SPELLINL,
	SPELLS,
	SPRITE,
	STELSKIN,
	STONBACK,
	STONBAKE,
	STONEBAK,
	STONEBK2,
	STONSKIN,
	STORM,
	STREAM,
	STRIP,
	SUNMOONE,
	SUNMOON,
	SURDRBKE,
	SURDRBKG,
	SURRENDE,
	SURRENDR,
	SWAPBTN,
	SWAPWIN,
	SWORDSM2,
	SWORDSMN,
	SYSTEME,
	SYSTEM,
	TAVWIN,
	TENT,
	TERRAINS,
	TEXTBACK,
	TEXTBAK2,
	TEXTBAR,
	TITANBLA,
	TITANBLU,
	TITANMSL,
	TOWNBKG0,
	TOWNBKG1,
	TOWNBKG2,
	TOWNBKG3,
	TOWNBKG4,
	TOWNBKG5,
	TOWNFIX,
	TOWNNAME,
	TOWNWIND,
	TRADPOSE,
	TRADPOST,
	TREASURY,
	TREDECI,
	TREEVIL,
	TREFALL,
	TREFIR,
	TREJNGL,
	TRESNOW,
	TROLL2,
	TROLL,
	TROLLMSL,
	TWNBBOAT,
	TWNBCAPT,
	TWNBCSTL,
	TWNBDOCK,
	TWNBDW_0,
	TWNBDW_1,
	TWNBDW_2,
	TWNBDW_3,
	TWNBDW_4,
	TWNBDW_5,
	TWNBEXT0,
	TWNBEXT1,
	TWNBEXT2,
	TWNBEXT3,
	TWNBLTUR,
	TWNBMAGE,
	TWNBMARK,
	TWNBMOAT,
	TWNBRTUR,
	TWNBSPEC,
	TWNBSTAT,
	TWNBTENT,
	TWNBTHIE,
	TWNBTVRN,
	TWNBUP_1,
	TWNBUP_3,
	TWNBUP_4,
	TWNBWEL2,
	TWNBWELL,
	TWNKBOAT,
	TWNKCAPT,
	TWNKCSTL,
	TWNKDOCK,
	TWNKDW_0,
	TWNKDW_1,
	TWNKDW_2,
	TWNKDW_3,
	TWNKDW_4,
	TWNKDW_5,
	TWNKEXT0,
	TWNKEXT1,
	TWNKEXT2,
	TWNKLTUR,
	TWNKMAGE,
	TWNKMARK,
	TWNKMOAT,
	TWNKRTUR,
	TWNKSPEC,
	TWNKSTAT,
	TWNKTENT,
	TWNKTHIE,
	TWNKTVRN,
	TWNKUP_1,
	TWNKUP_2,
	TWNKUP_3,
	TWNKUP_4,
	TWNKUP_5,
	TWNKWEL2,
	TWNKWELL,
	TWNNBOAT,
	TWNNCAPT,
	TWNNCSTL,
	TWNNDOCK,
	TWNNDW_0,
	TWNNDW_1,
	TWNNDW_2,
	TWNNDW_3,
	TWNNDW_4,
	TWNNDW_5,
	TWNNEXT0,
	TWNNLTUR,
	TWNNMAGE,
	TWNNMARK,
	TWNNMOAT,
	TWNNRTUR,
	TWNNSPEC,
	TWNNSTAT,
	TWNNTENT,
	TWNNTHIE,
	TWNNTVRN,
	TWNNUP_1,
	TWNNUP_2,
	TWNNUP_3,
	TWNNUP_4,
	TWNNWEL2,
	TWNNWELL,
	TWNSBOAT,
	TWNSCAPT,
	TWNSCSTL,
	TWNSDOCK,
	TWNSDW_0,
	TWNSDW_1,
	TWNSDW_2,
	TWNSDW_3,
	TWNSDW_4,
	TWNSDW_5,
	TWNSEXT0,
	TWNSEXT1,
	TWNSLTUR,
	TWNSMAGE,
	TWNSMARK,
	TWNSMOAT,
	TWNSRTUR,
	TWNSSPEC,
	TWNSSTAT,
	TWNSTENT,
	TWNSTHIE,
	TWNSTVRN,
	TWNSUP_1,
	TWNSUP_2,
	TWNSUP_3,
	TWNSWEL2,
	TWNSWELL,
	TWNWBOAT,
	TWNWCAPT,
	TWNWCSTL,
	TWNWDOCK,
	TWNWDW_0,
	TWNWDW_1,
	TWNWDW_2,
	TWNWDW_3,
	TWNWDW_4,
	TWNWDW_5,
	TWNWEXT0,
	TWNWLTUR,
	TWNWMAGE,
	TWNWMARK,
	TWNWMOAT,
	TWNWRTUR,
	TWNWSPEC,
	TWNWSTAT,
	TWNWTENT,
	TWNWTHIE,
	TWNWTVRN,
	TWNWUP_3,
	TWNWUP5B,
	TWNWUP_5,
	TWNWWEL2,
	TWNWWELL,
	TWNZBOAT,
	TWNZCAPT,
	TWNZCSTL,
	TWNZDOCK,
	TWNZDW_0,
	TWNZDW_1,
	TWNZDW_2,
	TWNZDW_3,
	TWNZDW_4,
	TWNZDW_5,
	TWNZEXT0,
	TWNZLTUR,
	TWNZMAGE,
	TWNZMARK,
	TWNZMOAT,
	TWNZRTUR,
	TWNZSPEC,
	TWNZSTAT,
	TWNZTENT,
	TWNZTHIE,
	TWNZTVRN,
	TWNZUP_2,
	TWNZUP_4,
	TWNZUP_5,
	TWNZWEL2,
	TWNZWELL,
	UNICORN,
	VAMPIRE2,
	VAMPIRE,
	VGENBKGE,
	VGENBKG,
	VIEW_ALL,
	VIEWARME,
	VIEWARMY,
	VIEWARSM,
	VIEWDDOR,
	VIEWGEN,
	VIEWHROS,
	VIEWMINE,
	VIEWPUZL,
	VIEWRSRC,
	VIEWRTFX,
	VIEWTWNS,
	VIEWWRLD,
	VWFLAG12,
	VWFLAG4,
	VWFLAG6,
	WELEM,
	WELLBKG,
	WELLXTRA,
	WINCMBBE,
	WINCMBTB,
	WINCMBT,
	WINLOSEB,
	WINLOSEE,
	WINLOSE,
	WOLF,
	WRLK32,
	WZRD32,
	X_LOC1,
	X_LOC2,
	X_LOC3,
	Y_BFLG32,
	Y_FLAG32,
	YINYANG,
	ZOMBIE2,
	ZOMBIE,
	
	// system
	ROUTERED,
	TELEPORT1,
	TELEPORT2,
	TELEPORT3,

	UNKNOWN

    } icn_t;

    class Header
    {
	public:
	Header();
	
	void Load(const char * p);

	u16 OffsetX(void) const{ return offset_x; };
	u16 OffsetY(void) const{ return offset_y; };
	u16 Width(void) const{ return width; };
	u16 Height(void) const{ return height; };
	u8  Type(void) const{ return type; };
	u32 OffsetData(void) const{ return offset_data; };

	inline static u8 SizeOf(void){ return 13; };

	private:
	u16 offset_x;
	u16 offset_y;
	u16 width;
	u16 height;
	u8  type;
	u32 offset_data;
    };

    typedef struct
    {
        icn_t		type;
        std::string	string;

    } icnmap_t;

    const std::string  & GetString(const icn_t icn);
    u16 AnimationFrame(const icn_t icn, const u16 start, const u32 ticket = 0);
};

#endif
