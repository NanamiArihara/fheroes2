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

#include <iostream>
#include "midi.h"

u32 MIDI::ReadBE32(const char * p)
{
    const unsigned char * u = (const unsigned char *) p;

    return ((((u32) *u) << 24) | (((u32) *(u + 1)) << 16) | (((u32) *(u + 2)) << 8) | ((u32) *(u + 3)));
}

u16 MIDI::ReadBE16(const char * p)
{
    const unsigned char * u = (const unsigned char *) p;

    return((((u16) *u) << 8) | ((u16) *(u + 1)));
}

void MIDI::WriteBE32(char * p, u32 x)
{
    p[0] = static_cast<char>(x >> 24);
    p[1] = static_cast<char>((x & 0x00FF0000) >> 16);
    p[2] = static_cast<char>((x & 0x0000FF00) >> 8);
    p[3] = static_cast<char>(x & 0x000000FF);
}

void MIDI::WriteBE16(char * p, u16 x)
{
    p[0] = static_cast<char>(x >> 8);
    p[1] = static_cast<char>(x & 0x00FF);
}

u8 MIDI::UnpackDelta(const char *p, u32 & d)
{
    const char *p2 = p;
    d = 0;

    while(*p2 & 0x80)
    {
	if(4 <= p2 - p)
	{
	    std::cerr << "Event: unpack delta mistake" << std::endl;
	    break;
	}

	d |= 0x0000007F & static_cast<u32>(*p2);
	d <<= 7;
	++p2;
    }

    d += *p2;

    return p2 - p + 1;
}

u8 MIDI::PackDelta(char *p, const u32 & d)
{
    const char c1 = static_cast<char>(d & 0x0000007F);
    const char c2 = static_cast<char>((d & 0x00003F80) >> 7);
    const char c3 = static_cast<char>((d & 0x001FC000) >> 14);
    const char c4 = static_cast<char>((d & 0x0FE00000) >> 21);

    if(c4)
    { p[0] = c4 | 0x80; p[1] = c3 | 0x80; p[2] = c2 | 0x80; p[3] = c1; }
    else
    if(c3)
    { p[0] = c3 | 0x80; p[1] = c2 | 0x80; p[2] = c1; }
    else
    if(c2)
    { p[0] = c2 | 0x80; p[1] = c1; }
    else
    { p[0] = c1; }

    return (c4 ? 4 : (c3 ? 3 : (c2 ? 2 : 1)));
}

