/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef CHARHANDLER_H
#define CHARHANDLER_H

#include "localplayer.h"
#include "lockedarray.h"
#include "logindata.h"

#include <iosfwd>
#include <vector>

class CharCreateDialog;
class CharSelectDialog;
class LocalPlayer;

namespace Net {

class CharHandler
{
    public:
        virtual void setCharInfo(LockedArray<LocalPlayer*> *charInfo) = 0;

        virtual void setCharSelectDialog(CharSelectDialog *window) = 0;

        virtual void setCharCreateDialog(CharCreateDialog *window) = 0;

        virtual void getCharacters() = 0;

        virtual void chooseCharacter(int slot, LocalPlayer *character) = 0;

        virtual void newCharacter(const std::string &name, int slot,
                        bool gender, int hairstyle, int hairColor,
                        std::vector<int> stats) = 0;

        virtual void deleteCharacter(int slot, LocalPlayer* character) = 0;

        virtual void switchCharacter() = 0;

        virtual unsigned int baseSprite() const = 0;

        virtual unsigned int hairSprite() const = 0;

        virtual unsigned int maxSprite() const = 0;

        virtual ~CharHandler() {}
};

} // namespace Net

#endif // CHARHANDLER_H
