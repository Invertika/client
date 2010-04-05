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

#ifndef SERVERINFO_H
#define SERVERINFO_H

#include "utils/stringutils.h"

#include <string>
#include <vector>

class ServerInfo
{
public:
    enum Type {
        UNKNOWN,
        MANASERV,
        EATHENA
    };

    Type type;
    std::string name;
    std::string hostname;
    unsigned short port;

    std::string description;

    bool save;
    bool meetsMinimumVersion;

    ServerInfo()
    {
        type = UNKNOWN;
        port = 0;
        save = false;
        meetsMinimumVersion = true;
    }

    ServerInfo(const ServerInfo &info)
    {
        type = info.type;
        name = info.name;
        hostname = info.hostname;
        port = info.port;
        description = info.description;
        save = info.save;
        meetsMinimumVersion = info.meetsMinimumVersion;
    }

    bool isValid() const
    {
        return !(hostname.empty() || port == 0 || type == UNKNOWN);
    }

    void clear()
    {
        type = UNKNOWN;
        name.clear();
        hostname.clear();
        port = 0;
        description.clear();
        save = false;
        meetsMinimumVersion = true;
    }

    bool operator==(const ServerInfo &other) const
    {
        return (type == other.type && hostname == other.hostname &&
                port == other.port);
    }

    bool operator!=(const ServerInfo &other) const
    {
        return (type != other.type || hostname != other.hostname ||
                port != other.port);
    }

    static Type parseType(const std::string &type)
    {
        if (compareStrI(type, "eathena") == 0)
            return EATHENA;
        else if (compareStrI(type, "manaserv") == 0)
            return MANASERV;

        return UNKNOWN;
    }
};

typedef std::vector<ServerInfo> ServerInfos;

#endif // SERVERINFO_H
