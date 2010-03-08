/*
 *  The Mana Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
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
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NPC_DB_H
#define NPC_DB_H

#include <list>
#include <map>
#include <string>

struct NPCsprite
{
    std::string sprite;
    int variant;
};

struct NPCInfo
{
    std::list<NPCsprite*> sprites;
    std::list<std::string> particles;
};

typedef std::map<int, NPCInfo*> NPCInfos;

/**
 * NPC information database.
 */
namespace NPCDB
{
    void load();

    void unload();

    const NPCInfo& get(int id);

    typedef NPCInfos::iterator NPCInfosIterator;
}

#endif
