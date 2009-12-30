/*
 *  The Mana World
 *  Copyright (C) 2008  The Mana World Development Team
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

#ifndef GUI_GUILDLISTBOX_H
#define GUI_GUILDLISTBOX_H

#include <map>
#include <string>
#include <vector>

#include "gui/widgets/listbox.h"

class Image;

class GuildListBox : public ListBox
{
public:
    /**
     * Constructor
     */
    GuildListBox();

    /**
     * Draws the list box.
     */
    void draw(gcn::Graphics *gcnGraphics);

    void mousePressed(gcn::MouseEvent &event);

    /**
     * Sets the index of the selected element.
     */
//    void setSelected(int selected);

    /**
     * Set whether a member is online or offline
     */
    void setOnlineStatus(const std::string &user, bool online);

private:
    Image *onlineIcon;
    Image *offlineIcon;
    typedef std::map<std::string, bool> UserMap;
    UserMap mUsers;
};

#endif
