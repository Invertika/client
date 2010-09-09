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

#include "gui/charcreatedialog.h"

#include "game.h"
#include "localplayer.h"
#include "main.h"
#include "units.h"

#include "gui/charselectdialog.h"
#include "gui/confirmdialog.h"
#include "gui/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/playerbox.h"
#include "gui/widgets/radiobutton.h"
#include "gui/widgets/slider.h"
#include "gui/widgets/textfield.h"

#include "net/charhandler.h"
#include "net/messageout.h"
#include "net/net.h"

#include "resources/colordb.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>

CharCreateDialog::CharCreateDialog(CharSelectDialog *parent, int slot):
    Window(_("Create Character"), true, parent),
    mCharSelectDialog(parent),
    mSlot(slot)
{
    mPlayer = new Being(0, ActorSprite::PLAYER, 0, NULL);
    mPlayer->setGender(GENDER_MALE);

    int numberOfHairColors = ColorDB::size();

    mHairStyle = rand() % mPlayer->getNumOfHairstyles();
    mHairColor = rand() % numberOfHairColors;
    updateHair();

    mNameField = new TextField("");
    mNameLabel = new Label(_("Name:"));
    // TRANSLATORS: This is an arrow symbol used to denote 'next'.
    // You may change this symbol if your language uses another.
    mNextHairColorButton = new Button(_(">"), "nextcolor", this);
    // TRANSLATORS: This is an arrow symbol used to denote 'previous'.
    // You may change this symbol if your language uses another.
    mPrevHairColorButton = new Button(_("<"), "prevcolor", this);
    mHairColorLabel = new Label(_("Hair color:"));
    mNextHairStyleButton = new Button(_(">"), "nextstyle", this);
    mPrevHairStyleButton = new Button(_("<"), "prevstyle", this);
    mHairStyleLabel = new Label(_("Hair style:"));
    mCreateButton = new Button(_("Create"), "create", this);
    mCancelButton = new Button(_("Cancel"), "cancel", this);
    mMale = new RadioButton(_("Male"), "gender");
    mFemale = new RadioButton(_("Female"), "gender");

    // Default to a Male character
    mMale->setSelected(true);

    mMale->setActionEventId("gender");
    mFemale->setActionEventId("gender");

    mMale->addActionListener(this);
    mFemale->addActionListener(this);

    mPlayerBox = new PlayerBox(mPlayer);
    mPlayerBox->setWidth(74);

    mNameField->setActionEventId("create");
    mNameField->addActionListener(this);

    mAttributesLeft = new Label(
            strprintf(_("Please distribute %d points"), 99));

    int w = 200;
    int h = 330;
    setContentSize(w, h);
    mPlayerBox->setDimension(gcn::Rectangle(80, 30, 110, 85));
    mNameLabel->setPosition(5, 5);
    mNameField->setDimension(
            gcn::Rectangle(45, 5, w - 45 - 7, mNameField->getHeight()));
    mPrevHairColorButton->setPosition(90, 35);
    mNextHairColorButton->setPosition(165, 35);
    mHairColorLabel->setPosition(5, 40);
    mPrevHairStyleButton->setPosition(90, 64);
    mNextHairStyleButton->setPosition(165, 64);
    mHairStyleLabel->setPosition(5, 70);
    mAttributesLeft->setPosition(15, 280);
    updateSliders();
    mCancelButton->setPosition(
            w - 5 - mCancelButton->getWidth(),
            h - 5 - mCancelButton->getHeight());
    mCreateButton->setPosition(
            mCancelButton->getX() - 5 - mCreateButton->getWidth(),
            h - 5 - mCancelButton->getHeight());

    mMale->setPosition(30, 120);
    mFemale->setPosition(100, 120);

    add(mPlayerBox);
    add(mNameField);
    add(mNameLabel);
    add(mNextHairColorButton);
    add(mPrevHairColorButton);
    add(mHairColorLabel);
    add(mNextHairStyleButton);
    add(mPrevHairStyleButton);
    add(mHairStyleLabel);
    add(mAttributesLeft);
    add(mCreateButton);
    add(mCancelButton);

    add(mMale);
    add(mFemale);

    center();
    setVisible(true);
    mNameField->requestFocus();
}

CharCreateDialog::~CharCreateDialog()
{
    delete mPlayer;

    // Make sure the char server handler knows that we're gone
    Net::getCharHandler()->setCharCreateDialog(0);
}

void CharCreateDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "create")
    {
        if (getName().length() >= 4)
        {
            // Attempt to create the character
            mCreateButton->setEnabled(false);

            std::vector<int> atts;
            for (unsigned i = 0; i < mAttributeSlider.size(); i++)
            {
                atts.push_back((int) mAttributeSlider[i]->getValue());
            }

            Net::getCharHandler()->newCharacter(getName(), mSlot,
                                                mFemale->isSelected(),
                                                mHairStyle,
                                                mHairColor, atts);
        }
        else
        {
            new OkDialog(_("Error"),
                         _("Your name needs to be at least 4 characters."),
                         true,  this);
        }
    }
    else if (event.getId() == "cancel")
        scheduleDelete();
    else if (event.getId() == "nextcolor")
    {
        mHairColor++;
        updateHair();
    }
    else if (event.getId() == "prevcolor")
    {
        mHairColor--;
        updateHair();
    }
    else if (event.getId() == "nextstyle")
    {
        mHairStyle++;
        updateHair();
    }
    else if (event.getId() == "prevstyle")
    {
        mHairStyle--;
        updateHair();
    }
    else if (event.getId() == "statslider")
    {
        updateSliders();
    }
    else if (event.getId() == "gender")
    {
        if (mMale->isSelected())
        {
            mPlayer->setGender(GENDER_MALE);
        }
        else
        {
            mPlayer->setGender(GENDER_FEMALE);
        }
    }
}

std::string CharCreateDialog::getName() const
{
    std::string name = mNameField->getText();
    trim(name);
    return name;
}

void CharCreateDialog::updateSliders()
{
    for (unsigned i = 0; i < mAttributeSlider.size(); i++)
    {
        // Update captions
        mAttributeValue[i]->setCaption(
                toString((int) (mAttributeSlider[i]->getValue())));
        mAttributeValue[i]->adjustSize();
    }

    // Update distributed points
    int pointsLeft = mMaxPoints - getDistributedPoints();
    if (pointsLeft == 0)
    {
        mAttributesLeft->setCaption(_("Character stats OK"));
        mCreateButton->setEnabled(true);
    }
    else
    {
        mCreateButton->setEnabled(false);
        if (pointsLeft > 0)
        {
            mAttributesLeft->setCaption(
                    strprintf(_("Please distribute %d points"), pointsLeft));
        }
        else
        {
            mAttributesLeft->setCaption(
                    strprintf(_("Please remove %d points"), -pointsLeft));
        }
    }

    mAttributesLeft->adjustSize();
}

void CharCreateDialog::unlock()
{
    mCreateButton->setEnabled(true);
}

int CharCreateDialog::getDistributedPoints() const
{
    int points = 0;

    for (unsigned i = 0; i < mAttributeSlider.size(); i++)
    {
        points += (int) mAttributeSlider[i]->getValue();
    }
    return points;
}

void CharCreateDialog::setAttributes(const std::vector<std::string> &labels,
                                     unsigned int available, unsigned int min,
                                     unsigned int max)
{
    mMaxPoints = available;

    for (unsigned i = 0; i < mAttributeLabel.size(); i++)
    {
        remove(mAttributeLabel[i]);
        delete mAttributeLabel[i];
        remove(mAttributeSlider[i]);
        delete mAttributeSlider[i];
        remove(mAttributeValue[i]);
        delete mAttributeValue[i];
    }

    mAttributeLabel.resize(labels.size());
    mAttributeSlider.resize(labels.size());
    mAttributeValue.resize(labels.size());

    int w = 200;
    int h = 330;

    for (unsigned i = 0; i < labels.size(); i++)
    {
        mAttributeLabel[i] = new Label(labels[i]);
        mAttributeLabel[i]->setWidth(70);
        mAttributeLabel[i]->setPosition(5, 140 + i*20);
        add(mAttributeLabel[i]);

        mAttributeSlider[i] = new Slider(min, max);
        mAttributeSlider[i]->setDimension(gcn::Rectangle(75, 140 + i * 20,
                                                         100, 10));
        mAttributeSlider[i]->setActionEventId("statslider");
        mAttributeSlider[i]->addActionListener(this);
        add(mAttributeSlider[i]);

        mAttributeValue[i] = new Label(toString(min));
        mAttributeValue[i]->setPosition(180, 140 + i*20);
        add(mAttributeValue[i]);
    }

    mAttributesLeft->setPosition(15, 280);
    updateSliders();

    mCancelButton->setPosition(
            w - 5 - mCancelButton->getWidth(),
            h - 5 - mCancelButton->getHeight());
    mCreateButton->setPosition(
            mCancelButton->getX() - 5 - mCreateButton->getWidth(),
            h - 5 - mCancelButton->getHeight());
}

void CharCreateDialog::setFixedGender(bool fixed, Gender gender)
{
    if (gender == GENDER_FEMALE)
    {
        mFemale->setSelected(true);
        mMale->setSelected(false);
    }
    else
    {
        mMale->setSelected(true);
        mFemale->setSelected(false);
    }

    mPlayer->setGender(gender);

    if (fixed)
    {
        mMale->setEnabled(false);
        mFemale->setEnabled(false);
    }
}

void CharCreateDialog::updateHair()
{
    mHairStyle %= Being::getNumOfHairstyles();
    if (mHairStyle < 0)
       mHairStyle += Being::getNumOfHairstyles();

    mHairColor %= ColorDB::size();
    if (mHairColor < 0)
       mHairColor += ColorDB::size();

    mPlayer->setSprite(Net::getCharHandler()->hairSprite(),
                      mHairStyle * -1, ColorDB::get(mHairColor));
}
