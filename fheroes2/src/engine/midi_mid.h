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

#ifndef MIDI_MID_H
#define MIDI_MID_H

#include <list>
#include "midi.h"
#include "midi_mthd.h"
#include "midi_mtrk.h"

namespace MIDI
{
    class Mid
    {
    public:
	Mid();
	Mid(const Mid & m);
	~Mid();

	Mid & operator= (const Mid & m);

	bool Read(const std::string & filename);
	bool Write(const std::string & filename);

        void SetFormat(const u16 f){ mthd.SetFormat(f); };
        void SetTracks(const u16 t){ mthd.SetTracks(t); };
        void SetPPQN(const u16 p){ mthd.SetPPQN(p); };
	void AddTrack(MTrk & track);

	void Dump(void) const;

    private:
	MThd			mthd;
	std::list<MTrk *>	tracks;
    };
};

#endif
