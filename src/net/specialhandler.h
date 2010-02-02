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

#ifndef SPECIALHANDLER_H
#define SPECIALHANDLER_H

#include <iosfwd>

namespace Net {
class SpecialHandler
{
    public:
        virtual void use(int id) = 0;

        virtual void use(int id, int level, int beingId) = 0;

        virtual void use(int id, int level, int x, int y) = 0;

        virtual void use(int id, const std::string &map) = 0;

        virtual ~SpecialHandler () {}
};
}

#endif // SPECIALHANDLER_H
