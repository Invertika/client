/*
 *  The Mana World
 *  Copyright (C) 2004  The Mana World Development Team
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

#ifndef _ENGINE_H
#define _ENGINE_H

#include <string>

class Map;

/**
 * Game engine. Actually hardly does anything anymore except keeping track of
 * the current map.
 */
class Engine
{
    public:
        Engine();

        ~Engine();

        /**
         * Returns the currently active map.
         */
        Map *getCurrentMap() { return mCurrentMap; }

        const std::string &getCurrentMapName() { return mMapName; }

        /**
         * Sets the currently active map.
         */
        void changeMap(const std::string &mapName);

        /**
         * Performs engine logic. This method is called 100 times per second.
         */
        void logic();

    private:
        Map *mCurrentMap;
        std::string mMapName;
};

extern Engine *engine;

#endif
