/*
 *  The Mana Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef INVENTORYHANDLER_H
#define INVENTORYHANDLER_H

#include "inventory.h"
#include "item.h"

#include <iosfwd>

namespace Net {

class InventoryHandler
{
    public:
        virtual void equipItem(const Item *item) = 0;

        virtual void unequipItem(const Item *item) = 0;

        virtual void useItem(const Item *item) = 0;

        virtual void dropItem(const Item *item, int amount) = 0;

        virtual bool canSplit(const Item *item) = 0;

        virtual void splitItem(const Item *item, int amount) = 0;

        virtual void moveItem(int oldIndex, int newIndex) = 0;

        virtual void openStorage(int type) = 0;

        virtual void closeStorage(int type) = 0;

        //void changeCart() = 0;

        virtual void moveItem(int source, int slot, int amount,
                              int destination) = 0;

        // TODO: fix/remove me
        virtual size_t getSize(int type) const = 0;

        virtual ~InventoryHandler() {}
};

} // namespace Net

#endif // INVENTORYHANDLER_H
