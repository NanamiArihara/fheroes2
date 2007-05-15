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
#ifndef H2ARMY_H
#define H2ARMY_H

#include <string>
#include <utility>
#include "monster.h"
#include "gamedefs.h"

namespace Army
{
    namespace Size
    {
	typedef enum
        {
	    FEW		= 1,
    	    SEVERAL	= 5,
    	    PACK	= 10,
    	    LOTS	= 20,
    	    HORDE	= 50,
    	    THRONG	= 100,
    	    SWARM	= 250,
    	    ZOUNDS	= 500,
    	    LEGION	= 1000
	} size_t;

	const std::string & String(size_t size);
    };

    class Army
    {
    public:
	Army(Monster::monster_t ms = Monster::UNKNOWN, u16 num = 0) : monster(ms), count(num){};
	
	bool Valid(void) const{ return Monster::UNKNOWN > monster && count; };
	Monster::monster_t GetMonster(void) const{ return monster; };
	u16 GetCount(void) const{ return count; };

	void SetMonster(Monster::monster_t ms){ monster = ms; };
	void SetCount(u16 num){ count = num; };

    private:
	Monster::monster_t monster;
	u16 count;
    };
};

#endif
