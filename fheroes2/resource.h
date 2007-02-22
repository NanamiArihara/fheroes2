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
#ifndef H2RESOURCE_H
#define H2RESOURCE_H

#include "gamedefs.h"

#define RNDRESOURCEMAX  15 
#define RNDRESOURCEMIN  5 

namespace Resource
{
    typedef enum {
        WOOD,
        MERCURY,
        ORE,
        SULFUR,
        CRYSTAL,
        GEMS,
        GOLD,
    } resource_t;

    typedef struct {
        u16 wood;
        u16 mercury;
        u16 ore;
        u16 sulfur;
        u16 crystal;
        u16 gems;
        u32 gold;
    } funds_t;

    funds_t operator+ (const funds_t &pm1, const funds_t &pm2);
    funds_t operator- (const funds_t &pm1, const funds_t &pm2);

    resource_t Rand(void);
};

#endif
