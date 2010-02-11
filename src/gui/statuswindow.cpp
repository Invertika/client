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

#include "gui/statuswindow.h"

#include "localplayer.h"
#include "units.h"

#include "gui/ministatus.h"
#include "gui/setup.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/vertcontainer.h"
#include "gui/widgets/windowcontainer.h"

#include "net/net.h"
#include "net/playerhandler.h"

#include "utils/gettext.h"
#include "utils/mathutils.h"
#include "utils/stringutils.h"

class AttrDisplay : public Container
{
    public:
        virtual std::string update();

    protected:
        AttrDisplay(int id, const std::string &name);

        const int mId;
        const std::string mName;

        LayoutHelper *mLayout;
        Label *mLabel;
        Label *mValue;
};

class DerDisplay : public AttrDisplay
{
    public:
        DerDisplay(int id, const std::string &name);
};

class ChangeDisplay : public AttrDisplay, gcn::ActionListener
{
    public:
        ChangeDisplay(int id, const std::string &name);
        std::string update();
        void setPointsNeeded(int needed);

    private:
        void action(const gcn::ActionEvent &event);

        int mNeeded;

        Label *mPoints;
        Button *mDec;
        Button *mInc;
};

StatusWindow::StatusWindow():
    Window(player_node->getName())
{
    setWindowName("Status");
    setupWindow->registerWindowForReset(this);
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setDefaultSize((windowContainer->getWidth() - 365) / 2,
                   (windowContainer->getHeight() - 255) / 2, 365, 275);

    // ----------------------
    // Status Part
    // ----------------------

    mLvlLabel = new Label(strprintf(_("Level: %d"), 0));
    mMoneyLabel = new Label(strprintf(_("Money: %s"), ""));

    mHpLabel = new Label(_("HP:"));
    mHpBar = new ProgressBar((float) player_node->getHp()
                             / (float) player_node->getMaxHp(),
                             80, 15, gcn::Color(0, 171, 34));

    mXpLabel = new Label(_("Exp:"));
    mXpBar = new ProgressBar((float) player_node->getExp()
                             / player_node->getExpNeeded(),
                             80, 15, gcn::Color(143, 192, 211));

    mMpLabel = new Label(_("MP:"));
    mMpBar = new ProgressBar((float) player_node->getMaxMP()
                             / (float) player_node->getMaxMP(),
                             80, 15, Net::getPlayerHandler()->canUseMagic() ?
                             gcn::Color(26, 102, 230) :
                             gcn::Color(100, 100, 100));

    place(0, 0, mLvlLabel, 3);
    // 5, 0 Job Level
    place(8, 0, mMoneyLabel, 3);
    place(0, 1, mHpLabel).setPadding(3);
    place(1, 1, mHpBar, 4);
    place(5, 1, mXpLabel).setPadding(3);
    place(6, 1, mXpBar, 5);
    place(0, 2, mMpLabel).setPadding(3);
    // 5, 2 and 6, 2 Job Progress Bar
    place(1, 2, mMpBar, 4);

    if (Net::getPlayerHandler()->getJobLocation() > 0)
    {
        mJobLvlLabel = new Label(strprintf(_("Job: %d"), 0));
        mJobLabel = new Label(_("Job:"));
        mJobBar = new ProgressBar(0.0f, 80, 15, gcn::Color(220, 135, 203));

        place(5, 0, mJobLvlLabel, 3);
        place(5, 2, mJobLabel).setPadding(3);
        place(6, 2, mJobBar, 5);
    }

    // ----------------------
    // Stats Part
    // ----------------------

    mAttrCont = new VertContainer(32);
    mAttrScroll = new ScrollArea(mAttrCont);
    mAttrScroll->setOpaque(false);
    mAttrScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mAttrScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
    place(0, 3, mAttrScroll, 5, 3);

    mDAttrCont = new VertContainer(32);
    mDAttrScroll = new ScrollArea(mDAttrCont);
    mDAttrScroll->setOpaque(false);
    mDAttrScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mDAttrScroll->setVerticalScrollPolicy(ScrollArea::SHOW_AUTO);
    place(6, 3, mDAttrScroll, 5, 3);

    getLayout().setRowHeight(3, Layout::AUTO_SET);

    mCharacterPointsLabel = new Label("C");
    place(0, 6, mCharacterPointsLabel, 5);

    if (Net::getPlayerHandler()->canCorrectAttributes())
    {
        mCorrectionPointsLabel = new Label("C");
        place(0, 7, mCorrectionPointsLabel, 5);
    }

    loadWindowState();

    update(HP);
    update(MP);
    update(EXP);
    update(MONEY);
    update(CHAR_POINTS); // This also updates all attributes (none atm)
    update(LEVEL);
    int job = Net::getPlayerHandler()->getJobLocation();
    if (job > 0)
    {
        update(job);
    }
}

std::string StatusWindow::update(int id)
{
    if (miniStatusWindow)
        miniStatusWindow->update(id);

    if (id == HP)
    {
        updateHPBar(mHpBar, true);

        return _("HP");
    }
    else if (id == MP)
    {
        updateMPBar(mMpBar, true);

        return _("MP");
    }
    else if (id == EXP)
    {
        updateXPBar(mXpBar, false);

        return _("Exp");
    }
    else if (id == MONEY)
    {
        int money = player_node->getMoney();
        mMoneyLabel->setCaption(strprintf(_("Money: %s"),
                                        Units::formatCurrency(money).c_str()));
        mMoneyLabel->adjustSize();

        return _("Money");
    }
    else if (id == Net::getPlayerHandler()->getJobLocation())
    {
        mJobLvlLabel->setCaption(strprintf(_("Job: %d"),
                                        player_node->getAttributeBase(id)));
        mJobLvlLabel->adjustSize();

        updateProgressBar(mJobBar, id, false);

        return _("Job");
    }
    else if (id == CHAR_POINTS)
    {
        mCharacterPointsLabel->setCaption(strprintf(_("Character points: %d"),
                                        player_node->getCharacterPoints()));
        mCharacterPointsLabel->adjustSize();

        if (Net::getPlayerHandler()->canCorrectAttributes())
        {
            mCorrectionPointsLabel->setCaption(strprintf(_("Correction points: %d"),
                                            player_node->getCorrectionPoints()));
            mCorrectionPointsLabel->adjustSize();
        }

        for (Attrs::iterator it = mAttrs.begin(); it != mAttrs.end(); it++)
        {
            it->second->update();
        }
    }
    else if (id == LEVEL)
    {
        mLvlLabel->setCaption(strprintf(_("Level: %d"),
                                        player_node->getLevel()));
        mLvlLabel->adjustSize();

        return _("Level");
    }
    else
    {
        Attrs::iterator it = mAttrs.find(id);

        if (it != mAttrs.end())
        {
            return it->second->update();
        }
    }

    return "";
}

void StatusWindow::setPointsNeeded(int id, int needed)
{
    Attrs::iterator it = mAttrs.find(id);

    if (it != mAttrs.end())
    {
        ChangeDisplay *disp = dynamic_cast<ChangeDisplay*>(it->second);
        if (disp)
            disp->setPointsNeeded(needed);
    }
}

void StatusWindow::addAttribute(int id, const std::string &name,
                                bool modifiable)
{
    AttrDisplay *disp;

    if (modifiable)
    {
        disp = new ChangeDisplay(id, name);
        mAttrCont->add(disp);
    }
    else
    {
        disp = new DerDisplay(id, name);
        mDAttrCont->add(disp);
    }

    mAttrs[id] = disp;
}

void StatusWindow::updateHPBar(ProgressBar *bar, bool showMax)
{

    if (showMax)
        bar->setText(toString(player_node->getHp()) +
                    "/" + toString(player_node->getMaxHp()));
    else
        bar->setText(toString(player_node->getHp()));

    if (player_node->getMaxHp() < 4)
    {
        bar->setColor(guiPalette->getColor(Palette::HPBAR_ONE_QUARTER));
    }
    else
    {
        // HP Bar coloration
        float r1 = 255;
        float g1 = 255;
        float b1 = 255;

        float r2 = 255;
        float g2 = 255;
        float b2 = 255;

        float weight = 1.0f;

        int curHP = player_node->getHp();
        int thresholdLevel = player_node->getMaxHp() / 4;
        int thresholdProgress = curHP % thresholdLevel;

        if (thresholdLevel)
            weight = 1 - ((float)thresholdProgress) / ((float)thresholdLevel);
        else
            weight = 0;

        if (curHP < (thresholdLevel))
        {
            gcn::Color color1 = guiPalette->getColor(Palette::HPBAR_ONE_HALF);
            gcn::Color color2 = guiPalette->getColor(Palette::HPBAR_ONE_QUARTER);
            r1 = color1.r; r2 = color2.r;
            g1 = color1.g; g2 = color2.g;
            b1 = color1.b; b2 = color2.b;
        }
        else if (curHP < (thresholdLevel*2))
        {
            gcn::Color color1 = guiPalette->getColor(Palette::HPBAR_THREE_QUARTERS);
            gcn::Color color2 = guiPalette->getColor(Palette::HPBAR_ONE_HALF);
            r1 = color1.r; r2 = color2.r;
            g1 = color1.g; g2 = color2.g;
            b1 = color1.b; b2 = color2.b;
        }
        else if (curHP < thresholdLevel*3)
        {
            gcn::Color color1 = guiPalette->getColor(Palette::HPBAR_FULL);
            gcn::Color color2 = guiPalette->getColor(Palette::HPBAR_THREE_QUARTERS);
            r1 = color1.r; r2 = color2.r;
            g1 = color1.g; g2 = color2.g;
            b1 = color1.b; b2 = color2.b;
        }
        else
        {
            gcn::Color color1 = guiPalette->getColor(Palette::HPBAR_FULL);
            gcn::Color color2 = guiPalette->getColor(Palette::HPBAR_FULL);
            r1 = color1.r; r2 = color2.r;
            g1 = color1.g; g2 = color2.g;
            b1 = color1.b; b2 = color2.b;
        }

        // Safety checks
        if (weight > 1.0f) weight = 1.0f;
        if (weight < 0.0f) weight = 0.0f;

        // Do the color blend
        r1 = (int) weightedAverage(r1, r2,weight);
        g1 = (int) weightedAverage(g1, g2, weight);
        b1 = (int) weightedAverage(b1, b2, weight);

        // More safety checks
        if (r1 > 255) r1 = 255;
        if (g1 > 255) g1 = 255;
        if (b1 > 255) b1 = 255;

        bar->setColor((int)r1,(int)g1, (int)b1);
    }

    bar->setProgress((float) player_node->getHp() / (float) player_node->getMaxHp());

}

void StatusWindow::updateMPBar(ProgressBar *bar, bool showMax)
{
    if (showMax)
        bar->setText(toString(player_node->getMP()) +
                    "/" + toString(player_node->getMaxMP()));
    else
        bar->setText(toString(player_node->getMP()));

    if (Net::getPlayerHandler()->canUseMagic())
        bar->setColor(26, 102, 230); // blue, to indicate that we have magic
    else
        bar->setColor(100, 100, 100); // grey, to indicate that we lack magic

    bar->setProgress((float) player_node->getMP() /
                     (float) player_node->getMaxMP());
}

void StatusWindow::updateProgressBar(ProgressBar *bar, int value, int max,
                              bool percent)
{
    if (max == 0)
    {
        bar->setText(_("Max"));
        bar->setProgress(1.0);
    }
    else
    {
        float progress = ((float) value) / max;

        if (percent)
            bar->setText(strprintf("%2.2f", 100 * progress) + "%");
        else
            bar->setText(toString(value) + "/" + toString(max));

        bar->setProgress(progress);
    }
}

void StatusWindow::updateXPBar(ProgressBar *bar, bool percent)
{
    updateProgressBar(bar, player_node->getExp(), player_node->getExpNeeded(),
                      percent);
}

void StatusWindow::updateProgressBar(ProgressBar *bar, int id, bool percent)
{
    std::pair<int, int> exp =  player_node->getExperience(id);
    updateProgressBar(bar, exp.first, exp.second, percent);
}

AttrDisplay::AttrDisplay(int id, const std::string &name):
        mId(id),
        mName(name)
{
    setSize(100, 32);
    mLabel = new Label(name);
    mValue = new Label("1");

    mLabel->setAlignment(Graphics::CENTER);
    mValue->setAlignment(Graphics::CENTER);

    mLayout = new LayoutHelper(this);
}

std::string AttrDisplay::update()
{
    int base = player_node->getAttributeBase(mId);
    int bonus = player_node->getAttributeEffective(mId) - base;
    std::string value = toString(base);
    if (bonus)
        value += strprintf(" (%+d)", bonus);
    mValue->setCaption(value);

    return mName;
}

DerDisplay::DerDisplay(int id, const std::string &name):
        AttrDisplay(id, name)
{
    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = mLayout->getPlacer(0, 0);

    place(0, 0, mLabel, 3);
    place(3, 0, mValue, 2);

    update();
}

ChangeDisplay::ChangeDisplay(int id, const std::string &name):
        AttrDisplay(id, name), mNeeded(1)
{
    mPoints = new Label(_("Max"));
    mInc = new Button(_("+"), "inc", this);

    // Do the layout
    ContainerPlacer place = mLayout->getPlacer(0, 0);

    place(0, 0, mLabel, 3);
    place(4, 0, mValue, 2);
    place(6, 0, mInc);
    place(7, 0, mPoints);

    if (Net::getPlayerHandler()->canCorrectAttributes())
    {
        mDec = new Button(_("-"), "dec", this);
        mDec->setWidth(mInc->getWidth());

        place(3, 0, mDec);
    }
    else
    {
        mDec = 0;
    }

    update();
}

std::string ChangeDisplay::update()
{
    if (mNeeded > 0)
    {
        mPoints->setCaption(toString(mNeeded));
    }
    else
    {
        mPoints->setCaption(_("Max"));
    }

    if (mDec)
    {
        mDec->setEnabled(player_node->getCorrectionPoints());
    }
    mInc->setEnabled(player_node->getCharacterPoints() >= mNeeded &&
                     mNeeded > 0);

    return AttrDisplay::update();
}

void ChangeDisplay::setPointsNeeded(int needed)
{
    mNeeded = needed;

    update();
}

void ChangeDisplay::action(const gcn::ActionEvent &event)
{
    if (Net::getPlayerHandler()->canCorrectAttributes() &&
        event.getSource() == mDec)
    {
        int newcorpoints = player_node->getCorrectionPoints() - 1;
        player_node->setCorrectionPoints(newcorpoints);
        int newpoints = player_node->getCharacterPoints() + 1;
        player_node->setCharacterPoints(newpoints);
        int newbase = player_node->getAttributeBase(mId) - 1;
        player_node->setAttributeBase(mId, newbase);
        int newmod = player_node->getAttributeEffective(mId) - 1;
        player_node->setAttributeEffective(mId, newmod);
        Net::getPlayerHandler()->decreaseAttribute(mId);
    }
    else if (event.getSource() == mInc)
    {
        int newpoints = player_node->getCharacterPoints() - 1;
        player_node->setCharacterPoints(newpoints);
        int newbase = player_node->getAttributeBase(mId) + 1;
        player_node->setAttributeBase(mId, newbase);
        int newmod = player_node->getAttributeEffective(mId) + 1;
        player_node->setAttributeEffective(mId, newmod);
        Net::getPlayerHandler()->increaseAttribute(mId);
    }
}
