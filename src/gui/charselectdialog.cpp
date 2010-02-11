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

#include "gui/charselectdialog.h"

#include "game.h"
#include "localplayer.h"
#include "main.h"
#include "units.h"

#include "gui/changeemaildialog.h"
#include "gui/changepassworddialog.h"
#include "gui/charcreatedialog.h"
#include "gui/confirmdialog.h"
#include "gui/okdialog.h"
#include "gui/sdlinput.h"
#include "gui/unregisterdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/container.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/textfield.h"

#include "net/charhandler.h"
#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/messageout.h"
#include "net/net.h"

#include "resources/colordb.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>

#include <string>

/**
 * Listener for confirming character deletion.
 */
class CharDeleteConfirm : public ConfirmDialog
{
    public:
        CharDeleteConfirm(CharSelectDialog *m):
            ConfirmDialog(_("Confirm Character Delete"),
                    _("Are you sure you want to delete this character?"), m),
            master(m)
        {
        }

        void action(const gcn::ActionEvent &event)
        {
            //ConfirmDialog::action(event);
            if (event.getId() == "yes")
            {
                master->attemptCharDelete();
            }
            ConfirmDialog::action(event);
        }

    private:
        CharSelectDialog *master;
};

class CharEntry : public Container
{
    public:
        CharEntry(CharSelectDialog *m, char slot, LocalPlayer *chr);

        char getSlot() const
        { return mSlot; }

        LocalPlayer *getChar() const
        { return mCharacter; }

        void setChar(LocalPlayer *chr);

        void requestFocus();

        void update();

    protected:
        friend class CharSelectDialog;
        char mSlot;
        LocalPlayer *mCharacter;

        PlayerBox *mPlayerBox;
        Label *mName;
        Label *mLevel;
        Label *mMoney;
        Button *mButton;
        Button *mDelete;
};

CharSelectDialog::CharSelectDialog(LockedArray<LocalPlayer*> *charInfo,
                                   LoginData *loginData):
    Window(_("Account and Character Management")),
    mCharInfo(charInfo),
    mLoginData(loginData),
    mCharHandler(Net::getCharHandler())
{
    setCloseButton(false);

    mAccountNameLabel = new Label(loginData->username);
    mSwitchLoginButton = new Button(_("Switch Login"), "switch", this);
    mChangePasswordButton = new Button(_("Change Password"), "change_password",
                                       this);

    for (int i = 0; i < MAX_CHARACTER_COUNT; i++)
    {
        charInfo->select(i);
        mCharEntries[i] = new CharEntry(this, i, charInfo->getEntry());
    }

    int optionalActions = Net::getLoginHandler()->supportedOptionalActions();

    ContainerPlacer place;
    place = getPlacer(0, 0);

    place(0, 0, mAccountNameLabel, 2);
    place(0, 1, mSwitchLoginButton);

    if (optionalActions & Net::LoginHandler::Unregister)
    {
        gcn::Button *unregisterButton = new Button(_("Unregister"),
                                                   "unregister", this);
        place(3, 1, unregisterButton);
    }

    place(0, 2, mChangePasswordButton);

    if (optionalActions & Net::LoginHandler::ChangeEmail)
    {
        gcn::Button *changeEmailButton = new Button(_("Change Email"),
                                                    "change_email", this);
        place(3, 2, changeEmailButton);
    }

    place = getPlacer(0, 1);
    place(0, 0, mCharEntries[0]);
    place(1, 0, mCharEntries[1]);
    place(2, 0, mCharEntries[2]);

    reflowLayout();

    addKeyListener(this);

    center();
    mCharEntries[0]->requestFocus();
    setVisible(true);

    Net::getCharHandler()->setCharSelectDialog(this);
}

void CharSelectDialog::action(const gcn::ActionEvent &event)
{
    const gcn::Widget *sourceParent = event.getSource()->getParent();

    // Update which character is selected when applicable
    if (const CharEntry *entry = dynamic_cast<const CharEntry*>(sourceParent))
        mCharInfo->select(entry->getSlot());

    if (event.getId() == "use")
    {
        chooseSelected();
    }
    else if (event.getId() == "switch")
    {
        mCharInfo->clear();
        state = STATE_SWITCH_LOGIN;
    }
    else if (event.getId() == "new")
    {
        if (!(mCharInfo->getEntry()))
        {
            // Start new character dialog
            CharCreateDialog *charCreateDialog =
                new CharCreateDialog(this, mCharInfo->getPos());
            mCharHandler->setCharCreateDialog(charCreateDialog);
        }
    }
    else if (event.getId() == "delete")
    {
        if (mCharInfo->getEntry())
        {
            new CharDeleteConfirm(this);
        }
    }
    else if (event.getId() == "change_password")
    {
        state = STATE_CHANGEPASSWORD;
    }
    else if (event.getId() == "change_email")
    {
        state = STATE_CHANGEEMAIL;
    }
    else if (event.getId() == "unregister")
    {
        state = STATE_UNREGISTER;
    }
}

void CharSelectDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    gcn::Key key = keyEvent.getKey();

    if (key.getValue() == Key::ESCAPE)
    {
        action(gcn::ActionEvent(mSwitchLoginButton, mSwitchLoginButton->getActionEventId()));
    }
}

void CharSelectDialog::attemptCharDelete()
{
    mCharHandler->deleteCharacter(mCharInfo->getPos(), mCharInfo->getEntry());
    mCharInfo->lock();
}

void CharSelectDialog::attemptCharSelect()
{
    mCharHandler->chooseCharacter(mCharInfo->getPos(), mCharInfo->getEntry());
    mCharInfo->lock();
}

bool CharSelectDialog::selectByName(const std::string &name)
{
    if (mCharInfo->isLocked())
        return false;

    unsigned int oldPos = mCharInfo->getPos();

    mCharInfo->select(0);
    do
    {
        LocalPlayer *player = mCharInfo->getEntry();

        if (player && player->getName() == name)
        {
            mCharEntries[mCharInfo->getPos()]->requestFocus();
            return true;
        }

        mCharInfo->next();
    } while (mCharInfo->getPos());

    mCharInfo->select(oldPos);

    return false;
}

bool CharSelectDialog::chooseSelected()
{
    if (!mCharInfo->getSize())
        return false;

    setVisible(false);
    attemptCharSelect();

    return true;
}

void CharSelectDialog::update(int slot)
{
    if (slot >= 0 && slot < MAX_CHARACTER_COUNT)
    {
        mCharInfo->select(slot);
        mCharEntries[slot]->setChar(mCharInfo->getEntry());
        mCharEntries[slot]->requestFocus();
    }
    else
    {
        int slot = mCharInfo->getPos();
        for (int i = 0; i < MAX_CHARACTER_COUNT; i++)
        {
            mCharInfo->select(slot);
            mCharEntries[slot]->setChar(mCharInfo->getEntry());
        }
        mCharInfo->select(slot);
    }
}

CharEntry::CharEntry(CharSelectDialog *m, char slot, LocalPlayer *chr):
        mSlot(slot),
        mCharacter(chr),
        mPlayerBox(new PlayerBox(chr))
{
    mButton = new Button("wwwwwwwww", "go", m);
    mName = new Label("wwwwwwwwwwwwwwwwwwwwwwww");
    mLevel = new Label("(888)");
    mMoney = new Label("wwwwwwwww");

    mDelete = new Button(_("Delete"), "delete", m);

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mPlayerBox, 3, 5);
    place(0, 5, mName, 3);
    place(0, 6, mLevel, 3);
    place(0, 7, mMoney, 3);
    place(0, 8, mButton, 3);
    place(0, 9, mDelete, 3);

    update();

    // Setting the width so that the largest label fits.
    mName->adjustSize();
    mMoney->adjustSize();
    int width = 74;
    if (width < 20 + mName->getWidth())
        width = 20 + mName->getWidth();
    if (width < 20 + mMoney->getWidth())
        width = 20 + mMoney->getWidth();
    h.reflowLayout(width, 112 + mName->getHeight() + mLevel->getHeight() +
            mMoney->getHeight() + mButton->getHeight() + mDelete->getHeight());
}

void CharEntry::setChar(LocalPlayer *chr)
{
    mCharacter = chr;

    mPlayerBox->setPlayer(chr);

    update();
}

void CharEntry::requestFocus()
{
    mButton->requestFocus();
}

void CharEntry::update()
{
    if (mCharacter)
    {
        mButton->setCaption(_("Choose"));
        mButton->setActionEventId("use");
        mName->setCaption(strprintf("%s", mCharacter->getName().c_str()));
        mLevel->setCaption(strprintf("Level %d", mCharacter->getLevel()));
        mMoney->setCaption(Units::formatCurrency(mCharacter->getMoney()));

        mDelete->setVisible(true);
    }
    else
    {
        mButton->setCaption(_("Create"));
        mButton->setActionEventId("new");
        mName->setCaption(_("(empty)"));
        mLevel->setCaption(_("(empty)"));
        mMoney->setCaption(Units::formatCurrency(0));

        mDelete->setVisible(false);
    }

    // Recompute layout
    distributeResizedEvent();
}
