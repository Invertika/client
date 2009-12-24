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

#include "gui/unregisterdialog.h"

#include "main.h"
#include "log.h"

#include "gui/okdialog.h"
#include "gui/register.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/passwordfield.h"
#include "gui/widgets/textfield.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <string>
#include <sstream>

UnRegisterDialog::UnRegisterDialog(LoginData *loginData):
    Window(_("Unregister"), true),
    mWrongDataNoticeListener(new WrongDataNoticeListener),
    mLoginData(loginData)
{
    gcn::Label *userLabel = new Label(strprintf(_("Name: %s"), mLoginData->
                                                username.c_str()));
    gcn::Label *passwordLabel = new Label(_("Password:"));
    mPasswordField = new PasswordField(mLoginData->password);
    mUnRegisterButton = new Button(_("Unregister"), "unregister", this);
    mCancelButton = new Button(_("Cancel"), "cancel", this);

    const int width = 210;
    const int height = 80;
    setContentSize(width, height);

    userLabel->setPosition(5, 5);
    userLabel->setWidth(width - 5);
    mPasswordField->setPosition(
            68, userLabel->getY() + userLabel->getHeight() + 7);
    mPasswordField->setWidth(130);

    passwordLabel->setPosition(5, mPasswordField->getY() + 1);

    mCancelButton->setPosition(
            width - 5 - mCancelButton->getWidth(),
            height - 5 - mCancelButton->getHeight());
    mUnRegisterButton->setPosition(
            mCancelButton->getX() - 5 - mUnRegisterButton->getWidth(),
            mCancelButton->getY());

    add(userLabel);
    add(passwordLabel);
    add(mPasswordField);
    add(mUnRegisterButton);
    add(mCancelButton);

    center();
    setVisible(true);
    mPasswordField->requestFocus();
    mPasswordField->setActionEventId("cancel");
}

UnRegisterDialog::~UnRegisterDialog()
{
    delete mWrongDataNoticeListener;
}

void
UnRegisterDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "cancel")
    {
        state = STATE_CHAR_SELECT;
    }
    else if (event.getId() == "unregister")
    {
        const std::string username = mLoginData->username.c_str();
        const std::string password = mPasswordField->getText();
        logger->log("UnregisterDialog::unregistered, Username is %s",
                     username.c_str());

        std::stringstream errorMessage;
        bool error = false;

        unsigned int min = Net::getLoginHandler()->getMinPasswordLength();
        unsigned int max = Net::getLoginHandler()->getMaxPasswordLength();

        // Check password
        if (password.length() < min)
        {
            // Pass too short
            errorMessage << strprintf(_("The password needs to be at least %d "
                                        "characters long."), min);
            error = true;
        }
        else if (password.length() > max - 1)
        {
            // Pass too long
            errorMessage << strprintf(_("The password needs to be less than "
                                        "%d characters long."), max);
            error = true;
        }

        if (error)
        {
            mWrongDataNoticeListener->setTarget(this->mPasswordField);

            OkDialog *dlg = new OkDialog(_("Error"), errorMessage.str());
            dlg->addActionListener(mWrongDataNoticeListener);
        }
        else
        {
            // No errors detected, unregister the new user.
            mUnRegisterButton->setEnabled(false);
            mLoginData->password = password;
            state = STATE_UNREGISTER_ATTEMPT;
        }
    }
}
