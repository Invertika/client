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

#ifndef TA_GUILDTAB_H
#define TA_GUILDTAB_H

#include "gui/widgets/chattab.h"

namespace TmwAthena {

/**
 * A tab for a guild chat channel.
 */
class GuildTab : public ChatTab
{
    public:
        GuildTab();
        ~GuildTab();

        void showHelp();

        bool handleCommand(const std::string &type, const std::string &args);

    protected:
        void handleInput(const std::string &msg);

        void getAutoCompleteList(std::vector<std::string> &names) const;
};

extern GuildTab *guildTab;

} // namespace TmwAthena

#endif // TA_GUILDTAB_H
