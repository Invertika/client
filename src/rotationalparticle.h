/*
 *  The Mana World
 *  Copyright (C) 2006  The Mana World Development Team
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

#ifndef ROTATIONAL_PARTICLE_H
#define ROTATIONAL_PARTICLE_H

#include "imageparticle.h"

#include <libxml/tree.h>

class Animation;
class Map;
class SimpleAnimation;

class RotationalParticle : public ImageParticle
{
    public:
        RotationalParticle(Map *map, Animation *animation);

        RotationalParticle(Map *map, xmlNodePtr animationNode);

        ~RotationalParticle();

        virtual bool update();

    private:
        SimpleAnimation *mAnimation; /**< Used animation for this particle */
};

#endif
