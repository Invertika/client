/*
 *  The Mana World
 *  Copyright (C) 2007-2010  The Mana World Development Team
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

#ifndef UTILS_SHA256_H
#define UTILS_SHA256_H

#include <string>

/**
 * Returns the SHA-256 hash for the given string.
 *
 * @param string the string to create the SHA-256 hash for
 * @return the SHA-256 hash for the given string.
 */
std::string sha256(const std::string &string);

#endif // UTILS_SHA256_H
