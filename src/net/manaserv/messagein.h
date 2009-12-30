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

#ifndef NET_MANASERV_MESSAGEIN_H
#define NET_MANASERV_MESSAGEIN_H

#include "net/messagein.h"

namespace ManaServ {

/**
 * Used for parsing an incoming message.
 *
 * \ingroup Network
 */
class MessageIn : public Net::MessageIn
{
    public:
        /**
         * Constructor.
         */
        MessageIn(const char *data, unsigned int length);

        int readInt16();            /**< Reads a short. */
        int readInt32();            /**< Reads a long. */
};

}

#endif // NET_MANASERV_MESSAGEIN_H
