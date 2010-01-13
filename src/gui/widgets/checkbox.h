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

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <guichan/widgets/checkbox.hpp>

class Image;

/**
 * Check box widget. Same as the Guichan check box but with custom look.
 *
 * \ingroup GUI
 */
class CheckBox : public gcn::CheckBox
{
    public:
        /**
         * Constructor.
         */
        CheckBox(const std::string &caption, bool selected = false);

        /**
         * Destructor.
         */
        ~CheckBox();

        /**
         * Draws the caption, then calls drawBox to draw the check box.
         */
        void draw(gcn::Graphics* graphics);

        /**
         * Update the alpha value to the checkbox components.
         */
        void updateAlpha();

        /**
         * Draws the check box, not the caption.
         */
        void drawBox(gcn::Graphics* graphics);

        /**
         * Called when the mouse enteres the widget area.
         */
        void mouseEntered(gcn::MouseEvent& event);

        /**
         * Called when the mouse leaves the widget area.
         */
        void mouseExited(gcn::MouseEvent& event);

    private:
        static int instances;
        static float mAlpha;
        bool mHasMouse;
        static Image *checkBoxNormal;
        static Image *checkBoxChecked;
        static Image *checkBoxDisabled;
        static Image *checkBoxDisabledChecked;
        static Image *checkBoxNormalHi;
        static Image *checkBoxCheckedHi;
};

#endif
