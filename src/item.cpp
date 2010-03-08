/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "item.h"

#include "gui/theme.h"

#include "resources/image.h"
#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

Item::Item(int id, int quantity, bool equipment, bool equipped):
    mImage(0),
    mDrawImage(0),
    mQuantity(quantity),
    mEquipment(equipment), mEquipped(equipped), mInEquipment(false)
{
    setId(id);
}

Item::~Item()
{
    if (mImage)
        mImage->decRef();
}

void Item::setId(int id)
{
    mId = id;

    // Types 0 and 1 are not equippable items.
    mEquipment = id && getInfo().getType() >= 2;

    // Load the associated image
    if (mImage)
        mImage->decRef();

    if (mDrawImage)
        mDrawImage->decRef();

    ResourceManager *resman = ResourceManager::getInstance();
    std::string imagePath = "graphics/items/" + getInfo().getImageName();
    mImage = resman->getImage(imagePath);
    mDrawImage = resman->getImage(imagePath);

    if (!mImage)
        mImage = Theme::getImageFromTheme("unknown-item.png");

    if (!mDrawImage)
        mDrawImage = Theme::getImageFromTheme("unknown-item.png");
}
