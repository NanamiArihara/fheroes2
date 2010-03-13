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
#ifndef H2MORALE_H
#define H2MORALE_H

#include "gamedefs.h"
#include <string>

namespace Morale
{
    typedef enum
    {
	UNKNOWN  = -4,
	TREASON  = -3,
        AWFUL    = -2,
        POOR     = -1,
        NORMAL   = 0,
        GOOD     = 1,
        GREAT    = 2,
        BLOOD    = 3
    } morale_t;

    const char* String(s8);
    const char* Description(s8);

    inline morale_t & operator++ (morale_t & morale){ return morale = BLOOD == morale ? BLOOD : morale_t(morale + 1); };
    inline morale_t & operator-- (morale_t & morale){ return morale = TREASON == morale ? TREASON : morale_t(morale - 1); }
};

#endif