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

#ifndef MINISTATUS_H
#define MINISTATUS_H

#include "gui/widgets/popup.h"

#include <vector>

class AnimatedSprite;
class Graphics;
class ProgressBar;

/**
 * The player mini-status dialog.
 *
 * \ingroup Interface
 */
class MiniStatusWindow : public Popup
{
    public:
        MiniStatusWindow();

        /**
         * Sets one of the icons.
         */
        void setIcon(int index, AnimatedSprite *sprite);

        void eraseIcon(int index);

        void drawIcons(Graphics *graphics);

        void update(int id); // Same types as status window

        void logic(); // Updates icons

        void draw(gcn::Graphics *graphics)
        { drawChildren(graphics); }

    private:
        /*
         * Mini Status Bars
         */
        ProgressBar *mHpBar;
        ProgressBar *mMpBar;
        ProgressBar *mXpBar;

        std::vector<AnimatedSprite *> mIcons;
};

extern MiniStatusWindow *miniStatusWindow;

#endif
