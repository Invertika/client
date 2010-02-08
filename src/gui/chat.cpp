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

#include "chat.h"

#include "beingmanager.h"
#include "configuration.h"
#include "localplayer.h"

#include "gui/recorder.h"
#include "gui/setup.h"
#include "gui/sdlinput.h"
#include "gui/partywindow.h"

#include "gui/widgets/chattab.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabbedarea.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/whispertab.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/focushandler.hpp>
#include <guichan/focuslistener.hpp>

#include <sstream>

/**
 * The chat input hides when it loses focus. It is also invisible by default.
 */
class ChatInput : public TextField, public gcn::FocusListener
{
    public:
        ChatInput():
            TextField("", false)
        {
            setVisible(false);
            addFocusListener(this);
        }

        /**
         * Called if the chat input loses focus. It will set itself to
         * invisible as result.
         */
        void focusLost(const gcn::Event &event)
        {
            setVisible(false);
        }
};


ChatWindow::ChatWindow():
    Window(_("Chat")),
    mTmpVisible(false),
    mCurrentTab(NULL)
{
    setWindowName("Chat");

    setupWindow->registerWindowForReset(this);

    // no title presented, title bar is padding so window can be moved.
    gcn::Window::setTitleBarHeight(gcn::Window::getPadding() + 4);
    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(true);
    setSaveVisible(true);
    setDefaultSize(600, 123, ImageRect::LOWER_LEFT);
    setMinWidth(150);
    setMinHeight(90);

    mItemLinkHandler = new ItemLinkHandler;

    mChatInput = new ChatInput;
    mChatInput->setActionEventId("chatinput");
    mChatInput->addActionListener(this);

    mChatTabs = new TabbedArea;

    place(0, 0, mChatTabs, 3, 3);
    place(0, 3, mChatInput, 3);

    loadWindowState();

    // Add key listener to chat input to be able to respond to up/down
    mChatInput->addKeyListener(this);
    mCurHist = mHistory.end();

    mReturnToggles = config.getValue("ReturnToggles", "0") == "1";

    mRecorder = new Recorder(this);
}

ChatWindow::~ChatWindow()
{
    config.setValue("ReturnToggles", mReturnToggles);
    delete mRecorder;
    delete_all(mWhispers);
    delete mItemLinkHandler;
}

void ChatWindow::resetToDefaultSize()
{
    mRecorder->resetToDefaultSize();
    Window::resetToDefaultSize();
}

void ChatWindow::logic()
{
    Window::logic();

    Tab *tab = getFocused();
    if (tab != mCurrentTab)
        mCurrentTab = tab;
}

ChatTab *ChatWindow::getFocused() const
{
    return dynamic_cast<ChatTab*>(mChatTabs->getSelectedTab());
}

void ChatWindow::clearTab(ChatTab *tab)
{
    if (tab)
        tab->clearText();
}

void ChatWindow::clearTab()
{
    clearTab(getFocused());
}

void ChatWindow::prevTab()
{
    int tab = mChatTabs->getSelectedTabIndex();

    if (tab == 0)
        tab = mChatTabs->getNumberOfTabs();
    tab--;

    mChatTabs->setSelectedTab(tab);
}

void ChatWindow::nextTab()
{
    int tab = mChatTabs->getSelectedTabIndex();

    tab++;
    if (tab == mChatTabs->getNumberOfTabs())
        tab = 0;

    mChatTabs->setSelectedTab(tab);
}

void ChatWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "chatinput")
    {
        std::string message = mChatInput->getText();

        if (!message.empty())
        {
            // If message different from previous, put it in the history
            if (mHistory.empty() || message != mHistory.back())
            {
                mHistory.push_back(message);
            }
            // Reset history iterator
            mCurHist = mHistory.end();

            // Send the message to the server
            chatInput(message);

            // Clear the text from the chat input
            mChatInput->setText("");
        }

        if (message.empty() || !mReturnToggles)
        {
            // Remove focus and hide input
            mFocusHandler->focusNone();

            // If the chatWindow is shown up because you want to send a message
            // It should hide now
            if (mTmpVisible)
                setVisible(false);
        }
    }
}

bool ChatWindow::requestChatFocus()
{
    // Make sure chatWindow is visible
    if (!isVisible())
    {
        setVisible(true);

        /*
         * This is used to hide chatWindow after sending the message. There is
         * a trick here, because setVisible will set mTmpVisible to false, you
         * have to put this sentence *after* setVisible, not before it
         */
        mTmpVisible = true;
    }

    // Don't do anything else if the input is already visible and has focus
    if (mChatInput->isVisible() && mChatInput->isFocused())
        return false;

    // Give focus to the chat input
    mChatInput->setVisible(true);
    mChatInput->requestFocus();
    return true;
}

bool ChatWindow::isInputFocused() const
{
    return mChatInput->isFocused();
}

void ChatWindow::removeTab(ChatTab *tab)
{
    // Prevent removal of the local chat tab
    if (tab == localChatTab)
        return;

    mChatTabs->removeTab(tab);
}

void ChatWindow::addTab(ChatTab *tab)
{
    // Make sure we don't end up with duplicates in the gui
    // TODO

    mChatTabs->addTab(tab, tab->mScrollArea);

    // Update UI
    logic();
}

void ChatWindow::removeWhisper(const std::string &nick)
{
    std::string tempNick = nick;
    toLower(tempNick);
    mWhispers.erase(tempNick);
}

void ChatWindow::chatInput(const std::string &msg)
{
    ChatTab *tab = getFocused();
    tab->chatInput(msg);
}

void ChatWindow::doPresent()
{
    const Beings &beings = beingManager->getAll();
    std::string response = "";
    int playercount = 0;

    for (Beings::const_iterator bi = beings.begin(), be = beings.end();
         bi != be; ++bi)
    {
        if ((*bi)->getType() == Being::PLAYER)
        {
            if (!response.empty())
            {
                response += ", ";
            }
            response += (*bi)->getName();
            ++playercount;
        }
    }

    std::string log = strprintf(_("Present: %s; %d players are present."),
                                response.c_str(), playercount);

    if (mRecorder->isRecording())
    {
        // Get the current system time
        time_t t;
        time(&t);

        // Format the time string properly
        std::ostringstream timeStr;
        timeStr << "[" << ((((t / 60) / 60) % 24 < 10) ? "0" : "")
            << (int) (((t / 60) / 60) % 24)
            << ":" << (((t / 60) % 60 < 10) ? "0" : "")
            << (int) ((t / 60) % 60)
            << "] ";

        mRecorder->record(timeStr.str() + log);
        getFocused()->chatLog(_("Attendance written to record log."),
                              BY_SERVER, true);
    }
    else
    {
        getFocused()->chatLog(log, BY_SERVER);
    }
}

void ChatWindow::scroll(int amount)
{
    if (!isVisible())
        return;

    ChatTab *tab = getFocused();
    if (tab)
        tab->scroll(amount);
}

void ChatWindow::mousePressed(gcn::MouseEvent &event)
{
    Window::mousePressed(event);

    if (event.isConsumed())
        return;

    mMoved = event.getY() <= mCurrentTab->getHeight();
    mDragOffsetX = event.getX();
    mDragOffsetY = event.getY();

}

void ChatWindow::mouseDragged(gcn::MouseEvent &event)
{
    Window::mouseDragged(event);

    if (event.isConsumed())
        return;

    if (isMovable() && mMoved)
    {
        int newX = std::max(0, getX() + event.getX() - mDragOffsetX);
        int newY = std::max(0, getY() + event.getY() - mDragOffsetY);
        newX = std::min(graphics->getWidth() - getWidth(), newX);
        newY = std::min(graphics->getHeight() - getHeight(), newY);
        setPosition(newX, newY);
    }
}


void ChatWindow::keyPressed(gcn::KeyEvent &event)
{
    if (event.getKey().getValue() == Key::DOWN)
    {
        if (mCurHist != mHistory.end())
        {
            // Move forward through the history
            HistoryIterator prevHist = mCurHist++;

            if (mCurHist != mHistory.end())
            {
                mChatInput->setText(*mCurHist);
                mChatInput->setCaretPosition(mChatInput->getText().length());
            }
            else
            {
                mChatInput->setText("");
                mCurHist = prevHist;
            }
        }
        else if (mChatInput->getText() != "")
        {
            mChatInput->setText("");
        }
    }
    else if (event.getKey().getValue() == Key::UP &&
            mCurHist != mHistory.begin() && mHistory.size() > 0)
    {
        // Move backward through the history
        mCurHist--;
        mChatInput->setText(*mCurHist);
        mChatInput->setCaretPosition(mChatInput->getText().length());
    }
    else if (event.getKey().getValue() == Key::TAB &&
             mChatInput->getText() != "")
    {
        autoComplete();
        return;
    }
}

void ChatWindow::addInputText(const std::string &text)
{
    const int caretPos = mChatInput->getCaretPosition();
    const std::string inputText = mChatInput->getText();

    std::ostringstream ss;
    ss << inputText.substr(0, caretPos) << text << " ";
    ss << inputText.substr(caretPos);

    mChatInput->setText(ss.str());
    mChatInput->setCaretPosition(caretPos + text.length() + 1);
    requestChatFocus();
}

void ChatWindow::addItemText(const std::string &item)
{
    std::ostringstream text;
    text << "[" << item << "]";
    addInputText(text.str());
}

void ChatWindow::setVisible(bool isVisible)
{
    Window::setVisible(isVisible);

    /*
     * For whatever reason, if setVisible is called, the mTmpVisible effect
     * should be disabled.
     */
    mTmpVisible = false;
}

void ChatWindow::setRecordingFile(const std::string &msg)
{
    mRecorder->setRecordingFile(msg);
}

void ChatWindow::whisper(const std::string &nick,
                         const std::string &mes, bool own)
{
    if (mes.empty())
        return;

    std::string playerName = player_node->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    if (tempNick.compare(playerName) == 0)
        return;

    ChatTab *tab = 0;
    TabMap::const_iterator i = mWhispers.find(tempNick);

    if (i != mWhispers.end())
        tab = i->second;
    else if (config.getValue("whispertab", true))
        tab = addWhisperTab(nick);

    if (tab)
    {
        if (own)
            tab->chatInput(mes);
        else
            tab->chatLog(nick, mes);
    }
    else
    {
        if (own)
        {
            Net::getChatHandler()->privateMessage(nick, mes);

            localChatTab->chatLog(strprintf(_("Whispering to %s: %s"),
                            nick.c_str(), mes.c_str()), BY_PLAYER);
        }
        else
        {
            localChatTab->chatLog(nick + " : " + mes, ACT_WHISPER, false);
        }
    }
}

ChatTab *ChatWindow::addWhisperTab(const std::string &nick, bool switchTo)
{
    std::string playerName = player_node->getName();
    std::string tempNick = nick;

    toLower(playerName);
    toLower(tempNick);

    if (mWhispers.find(tempNick) != mWhispers.end()
        || tempNick.compare(playerName) == 0)
        return NULL;

    ChatTab *ret = new WhisperTab(nick);
    mWhispers[tempNick] = ret;

    if (switchTo)
        mChatTabs->setSelectedTab(ret);

    return ret;
}

void ChatWindow::autoComplete()
{
    int caretPos = mChatInput->getCaretPosition();
    int startName = 0;
    const std::string inputText = mChatInput->getText();
    std::string name = inputText.substr(0, caretPos);
    std::string newName("");

    for (int f = caretPos - 1; f > -1; f --)
    {
        if (isWordSeparator(inputText[f]))
        {
            startName = f + 1;
            name = inputText.substr(f + 1, caretPos - f);
            break;
        }
    }

    if (caretPos - 1 + 1 == startName)
        return;


    ChatTab *cTab = static_cast<ChatTab*>(mChatTabs->getSelectedTab());
    std::vector<std::string> nameList;
    if (cTab && cTab->getType() == ChatTab::PARTY)
    {
        partyWindow->getNames(nameList);
        newName = autoComplete(nameList, name);
    }
    if (newName == "")
    {
        beingManager->getPlayerNames(nameList, true);
        newName = autoComplete(nameList, name);
    }
    if (newName == "")
    {
        newName = autoCompleteHistory(name);
    }

    if (newName != "")
    {
        if(inputText[0] == '@' || inputText[0] == '/')
            newName = "\"" + newName + "\"";

        mChatInput->setText(inputText.substr(0, startName) + newName
                            + inputText.substr(caretPos, inputText.length() - caretPos));

        if (startName > 0)
            mChatInput->setCaretPosition(caretPos - name.length() + newName.length() + 1);
        else
            mChatInput->setCaretPosition(caretPos - name.length() + newName.length());
    }
}

std::string ChatWindow::autoComplete(std::vector<std::string> &names,
                                     std::string partName) const
{
    std::vector<std::string>::iterator i = names.begin();
    toLower(partName);
    std::string newName("");

    while (i != names.end())
    {
        if (!i->empty())
        {
            std::string name = *i;
            toLower(name);

            std::string::size_type pos = name.find(partName, 0);
            if (pos == 0)
            {
                if (newName != "")
                {
                    toLower(newName);
                    newName = findSameSubstring(name, newName);
                }
                else
                {
                    newName = *i;
                }
            }
        }
        ++i;
    }

    return newName;
}

std::string ChatWindow::autoCompleteHistory(std::string partName)
{
    History::iterator i = mHistory.begin();
    std::vector<std::string> nameList;

    while (i != mHistory.end())
    {
        std::string line = *i;
        unsigned int f = 0;
        while (f < line.length() && !isWordSeparator(line.at(f)))
        {
            f++;
        }
        line = line.substr(0, f);
        if (line != "")
        {
            nameList.push_back(line);
        }
        ++i;
    }
    return autoComplete(nameList, partName);
}
