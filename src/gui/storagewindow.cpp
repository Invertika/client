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

#include "gui/storagewindow.h"

#include "inventory.h"
#include "item.h"
#include "localplayer.h"
#include "units.h"
#include "keyboardconfig.h"

#include "gui/inventorywindow.h"
#include "gui/itemamount.h"
#include "gui/setup.h"
#include "gui/viewport.h"

#include "gui/widgets/button.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>
#include <guichan/mouseinput.hpp>

#include <string>

StorageWindow::WindowList StorageWindow::instances;

StorageWindow::StorageWindow(Inventory *inventory):
    Window(_("Storage")),
    mInventory(inventory),
    mItemDesc(false)
{
    setWindowName("Storage");
    setResizable(true);
    setCloseButton(true);
    setupWindow->registerWindowForReset(this);

    // If you adjust these defaults, don't forget to adjust the trade window's.
    setDefaultSize(375, 300, ImageRect::CENTER);

    mStoreButton = new Button(_("Store"), "store", this);
    mRetrieveButton = new Button(_("Retrieve"), "retrieve", this);

    mCloseButton = new Button(_("Close"), "close", this);

    mItems = new ItemContainer(mInventory, true);
    mItems->addSelectionListener(this);

    gcn::ScrollArea *invenScroll = new ScrollArea(mItems);
    invenScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mUsedSlots = mInventory->getNumberOfSlotsUsed();

    mSlotsLabel = new Label(_("Slots:"));

    mSlotsBar = new ProgressBar(0.0f, 100, 20, gcn::Color(225, 200, 25));
    mSlotsBar->setText(strprintf("%d/%d", mUsedSlots, mInventory->getSize()));
    mSlotsBar->setProgress((float) mUsedSlots / mInventory->getSize());

    setMinHeight(130);
    setMinWidth(200);

    place(0, 0, mSlotsLabel).setPadding(3);
    place(1, 0, mSlotsBar, 3);
    place(0, 1, invenScroll, 4, 4);
    place(0, 5, mStoreButton);
    place(1, 5, mRetrieveButton);
    place(3, 5, mCloseButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, mStoreButton->getHeight());

    loadWindowState();

    instances.push_back(this);
    setVisible(true);
}

StorageWindow::~StorageWindow()
{
    instances.remove(this);
}

void StorageWindow::logic()
{
    if (!isVisible())
        return;

    Window::logic();

    const int usedSlots = mInventory->getNumberOfSlotsUsed();

    if (mUsedSlots != usedSlots)
    {
        mUsedSlots = usedSlots;

        mSlotsBar->setProgress((float) mUsedSlots / mInventory->getSize());

        mSlotsBar->setText(strprintf("%d/%d", mUsedSlots,
                                     mInventory->getSize()));
    }
}

void StorageWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "store")
    {
        if (!inventoryWindow->isVisible()) return;

        Item *item = inventoryWindow->getSelectedItem();

        if (!item)
            return;

        ItemAmountWindow::showWindow(ItemAmountWindow::StoreAdd, this, item);
    }
    else if (event.getId() == "retrieve")
    {
        Item *item = mItems->getSelectedItem();

        if (!item)
            return;

        ItemAmountWindow::showWindow(ItemAmountWindow::StoreRemove, this,
                                     item);
    }
    else if (event.getId() == "close")
    {
        close();
    }
}

void StorageWindow::mouseClicked(gcn::MouseEvent &event)
{
    Window::mouseClicked(event);

    if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        Item *item = mItems->getSelectedItem();

        if (!item)
        {
            mRetrieveButton->setEnabled(false);
            return;
        }

        mRetrieveButton->setEnabled(true);

        /* Convert relative to the window coordinates to absolute screen
         * coordinates.
         */
        const int mx = event.getX() + getX();
        const int my = event.getY() + getY();
        viewport->showPopup(this, mx, my, item, false);
    }
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (keyboard.isKeyActive(keyboard.KEY_EMOTE))
        {
            Item *item = mItems->getSelectedItem();

            if(!item)
                return;

            removeStore(item, item->getQuantity());
        }
    }
}

Item *StorageWindow::getSelectedItem() const
{
    return mItems->getSelectedItem();
}

void StorageWindow::addStore(Item *item, int amount)
{
    Net::getInventoryHandler()->moveItem(Net::InventoryHandler::INVENTORY,
                                         item->getInvIndex(), amount,
                                         Net::InventoryHandler::STORAGE);
}

void StorageWindow::removeStore(Item *item, int amount)
{
    Net::getInventoryHandler()->moveItem(Net::InventoryHandler::STORAGE,
                                         item->getInvIndex(), amount,
                                         Net::InventoryHandler::INVENTORY);
}

void StorageWindow::close()
{
    Net::getInventoryHandler()->closeStorage(Net::InventoryHandler::STORAGE);

    scheduleDelete();
}
