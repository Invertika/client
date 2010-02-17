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

#include "buysell.h"

#include "npc.h"

#include "gui/setup.h"

#include "gui/widgets/button.h"

#include "net/net.h"
#include "net/npchandler.h"

#include "utils/gettext.h"

BuySellDialog::DialogList BuySellDialog::instances;

BuySellDialog::BuySellDialog(int npcId):
    Window(_("Shop")),
    mNpcId(npcId),
    mBuyButton(0)
{
    setWindowName("BuySell");
    //setupWindow->registerWindowForReset(this);
    setCloseButton(true);

    static const char *buttonNames[] = {
        N_("Buy"), N_("Sell"), N_("Cancel"), 0
    };
    int x = 10, y = 10;

    for (const char **curBtn = buttonNames; *curBtn; curBtn++)
    {
        Button *btn = new Button(gettext(*curBtn), *curBtn, this);
        if (!mBuyButton)
            mBuyButton = btn; // For focus request
        btn->setPosition(x, y);
        add(btn);
        x += btn->getWidth() + 10;
    }
    mBuyButton->requestFocus();

    setContentSize(x, 2 * y + mBuyButton->getHeight());

    center();
    setDefaultSize();
    loadWindowState();

    instances.push_back(this);
    setVisible(true);
}

BuySellDialog::~BuySellDialog()
{
    instances.remove(this);
}

void BuySellDialog::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (visible)
    {
        mBuyButton->requestFocus();
    }
    else
    {
        scheduleDelete();
    }
}

void BuySellDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "Buy")
    {
        Net::getNpcHandler()->buy(mNpcId);
    }
    else if (event.getId() == "Sell")
    {
        Net::getNpcHandler()->sell(mNpcId);
    }

    close();
}

void BuySellDialog::closeAll()
{
    DialogList::iterator it = instances.begin();
    DialogList::iterator it_end = instances.end();

    for (; it != it_end; it++)
    {
        (*it)->close();
    }
}
