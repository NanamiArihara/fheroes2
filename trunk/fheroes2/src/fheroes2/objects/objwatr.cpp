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

#include <algorithm>
#include "icn.h"
#include "direction.h"
#include "objwatr.h"

u16 ObjWat2::GetPassable(const u8 & index)
{
#if (__GNUC__ == 4 && __GNUC_MINOR__ == 4)
    // fixed: array subscript is above array bounds
    const u8 disabled[] = { 11, 12, 19, 22, 255 };
#else
    const u8 disabled[] = { 11, 12, 19, 22 };
#endif
    const u8 restricted[] = { 2, 20 };

    if(isShadow(index))
        return DIRECTION_ALL;
    else
    if(isAction(index) ||
        ARRAY_COUNT_END(disabled) != std::find(disabled, ARRAY_COUNT_END(disabled), index))
        return 0;

    return ARRAY_COUNT_END(restricted) != std::find(restricted, ARRAY_COUNT_END(restricted), index) ?
            DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW : DIRECTION_ALL;
}

u16 ObjWatr::GetPassable(const u8 & index)
{
#if (__GNUC__ == 4 && __GNUC_MINOR__ == 4)
    // fixed: array subscript is above array bounds
    const u8 disabled[] = { 11, 12, 19, 22, 255 };
#else
    const u8 disabled[] = { 11, 12, 19, 22 };
#endif
    const u8 restricted[] = { 69, 182, 183, 185, 186, 187, 248 };

    if(isShadow(index))
        return DIRECTION_ALL;
    else
    if(isAction(index) ||
        ARRAY_COUNT_END(disabled) != std::find(disabled, ARRAY_COUNT_END(disabled), index))
        return 0;

    return ARRAY_COUNT_END(restricted) != std::find(restricted, ARRAY_COUNT_END(restricted), index) ?
            DIRECTION_CENTER_ROW | DIRECTION_BOTTOM_ROW : DIRECTION_ALL;
}

bool ObjWat2::isAction(const u8 & index)
{
    // derelict ship
    return 21 == index;
}

bool ObjWatr::isAction(const u8 & index)
{
    /*
	magellan maps: 62:
	buoy: 195
	whirlpool: 202, 206, 210, 214, 218, 222
	shipwrek: 241
    */
    const u8 actions[] = { 62, 195, 202, 206, 210, 214, 218, 222, 241 };

    return ARRAY_COUNT_END(actions) != std::find(actions, ARRAY_COUNT_END(actions), index);
}

bool ObjWatr::isShadow(const u8 & index)
{
#if (__GNUC__ == 4 && __GNUC_MINOR__ == 4)
    // fixed: array subscript is above array bounds
    const u8 shadows [] = { 12, 38, 52, 55, 118, 166, 188, 240, 255 };
#else
    const u8 shadows [] = { 12, 38, 52, 55, 118, 166, 188, 240 };
#endif

    return ARRAY_COUNT_END(shadows) != std::find(shadows, ARRAY_COUNT_END(shadows), index);
}

bool ObjWat2::isShadow(const u8 & index)
{
    return index == 1;
}
