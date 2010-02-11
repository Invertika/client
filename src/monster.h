/*
 *  The Mana World
 *  Copyright (C) 2004-2010  The Mana World Development Team
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

#ifndef MONSTER_H
#define MONSTER_H

#include "being.h"

class MonsterInfo;
class Text;

class Monster : public Being
{
    public:
        Monster(int id, int job, Map *map);

        virtual void logic();

        virtual void setAction(Action action, int attackType = 0);

        virtual Type getType() const { return MONSTER; }

        virtual TargetCursorSize
        getTargetCursorSize() const;

        /**
         * Handles an attack of another being by this monster. Plays a hit or
         * miss sound when appropriate.
         *
         * @param victim the victim being
         * @param damage the amount of damage dealt (0 means miss)
         * @param type the attack type
         */
        virtual void handleAttack(Being *victim, int damage, AttackType type);

        /**
         * Puts a damage bubble above this monster and plays the hurt sound
         *
         * @param attacker the attacking being
         * @param damage the amount of damage recieved (0 means miss)
         * @param type the attack type
         */
        virtual void takeDamage(Being *attacker, int amount, AttackType type);

        /**
         * Returns the MonsterInfo, with static data about this monster.
         */
        const MonsterInfo& getInfo() const;

        /**
         * Gets the way the monster is blocked by other objects
         */
        virtual unsigned char getWalkMask() const
        {
            return Map::BLOCKMASK_WALL
                    | Map::BLOCKMASK_CHARACTER
                    | Map::BLOCKMASK_MONSTER;
        }

    protected:
        /**
         * Gets the way the monster blocks pathfinding for other objects
         */
        virtual Map::BlockType getBlockType() const
        { return Map::BLOCKTYPE_MONSTER; }

        /**
         * Update the text when the monster moves
         */
        void updateCoords();

        void showName();

    private:
        int mAttackType;
};

#endif
