/*
 *  The Mana World
 *  Copyright (C) 2007  The Mana World Development Team
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

#include "utils/stringutils.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>

std::string &trim(std::string &str)
{
    std::string::size_type pos = str.find_last_not_of(' ');
    if (pos != std::string::npos)
    {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');

        if (pos != std::string::npos)
            str.erase(0, pos);
    }
    else
    {
        // There is nothing else but whitespace in the string
        str.clear();
    }
    return str;
}

std::string &toLower(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return str;
}

unsigned int atox(const std::string &str)
{
    unsigned int value;
    sscanf(str.c_str(), "0x%06x", &value);

    return value;
}

const char *ipToString(int address)
{
    static char asciiIP[16];

    sprintf(asciiIP, "%i.%i.%i.%i",
            (unsigned char)(address),
            (unsigned char)(address >> 8),
            (unsigned char)(address >> 16),
            (unsigned char)(address >> 24));

    return asciiIP;
}

std::string strprintf(char const *format, ...)
{
    char buf[256];
    va_list(args);
    va_start(args, format);
    int nb = vsnprintf(buf, 256, format, args);
    va_end(args);
    if (nb < 256)
    {
        return buf;
    }
    // The static size was not big enough, try again with a dynamic allocation.
    ++nb;
    char *buf2 = new char[nb];
    va_start(args, format);
    vsnprintf(buf2, nb, format, args);
    va_end(args);
    std::string res(buf2);
    delete [] buf2;
    return res;
}

std::string &removeBadChars(std::string &str)
{
    std::string::size_type pos;
    do
    {
        pos = str.find_first_of("@#[]");
        if (pos != std::string::npos)
            str.erase(pos, 1);
    } while (pos != std::string::npos);

    return str;
}

std::string removeColors(std::string msg)
{
    for (unsigned int f = 0; f < msg.length() - 2 && msg.length() > 2; f++)
    {
        while (msg.length() > f + 2 && msg.at(f) == '#' && msg.at(f + 1) == '#')
        {
            msg = msg.erase(f, 3);
        }
    }
    return msg;
}

int compareStrI(const std::string &a, const std::string &b)
{
    std::string::const_iterator itA = a.begin();
    std::string::const_iterator endA = a.end();
    std::string::const_iterator itB = b.begin();
    std::string::const_iterator endB = b.end();

    for (; itA < endA, itB < endB; ++itA, ++itB)
    {
        int comp = tolower(*itA) - tolower(*itB);
        if (comp)
            return comp;
    }

    return 0;
}
