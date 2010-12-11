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

#ifndef EQUIPMENTWINDOW_H
#define EQUIPMENTWINDOW_H

#include "equipment.h"

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

class Inventory;
class Item;
class ItemPopup;

/**
 * Equipment dialog.
 *
 * \ingroup Interface
 */
class EquipmentWindow : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        EquipmentWindow(Equipment *equipment);

        /**
         * Destructor.
         */
        ~EquipmentWindow();

        /**
         * Draws the equipment window.
         */
        void draw(gcn::Graphics *graphics);

        void action(const gcn::ActionEvent &event);

        void mousePressed(gcn::MouseEvent& mouseEvent);

    private:
        void mouseExited(gcn::MouseEvent &event);
        void mouseMoved(gcn::MouseEvent &event);

        Item *getItem(int x, int y) const;

        void setSelected(int index);

        Equipment *mEquipment;

        /**
         * Equipment box.
         */
        struct EquipBox
        {
            int posX;
            int posY;
        };

        EquipBox mEquipBox[Equipment::EQUIP_VECTOREND]; /**< Equipment Boxes. */

        ItemPopup *mItemPopup;
        gcn::Button *mUnequip;

        int mSelected; /**< Index of selected item. */
};

extern EquipmentWindow *equipmentWindow;

#endif
