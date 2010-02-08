/*
 *  The Mana World
 *  Copyright (C) 2008-2010  The Mana World Development Team
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

#ifndef AVATAR_H
#define AVATAR_H

#include "guichanfwd.h"

#include "gui/widgets/container.h"

#include <string>

class Image;
class Icon;

class Avatar : public Container
{
public:
    Avatar();
    ~Avatar();

    /**
     * Returns the avatar's name.
     */
    std::string getName() const { return mName; };

    /**
     * Set the avatar's name.
     */
    void setName(const std::string &name);

    /**
     * Returns the avatar's online status.
     */
    bool getOnline() const { return mOnline; }

    /**
     * Set the avatar's online status.
     */
    void setOnline(bool online);

    int getHp() const { return mHp; }

    void setHp(int hp);

    int getMaxHp() const { return mMaxHp; }

    void setMaxHp(int maxHp);

    bool getDisplayBold() const { return mDisplayBold; }

    void setDisplayBold(bool displayBold) { mDisplayBold = displayBold; updateAvatarLabel(); }

private:
    void updateAvatarLabel();

    std::string mName;
    int mHp;
    int mMaxHp;
    Icon *mStatus;
    gcn::Label *mLabel;
    bool mOnline;
    bool mDisplayBold;
};

#endif // AVATAR_H
