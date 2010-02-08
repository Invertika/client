/*
 *  The Mana World
 *  Copyright (C) 2010  The Mana World Development Team
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

#include "progressindicator.h"

#include "resources/animation.h"
#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include "graphics.h"
#include "simpleanimation.h"

#include <guichan/widgets/label.hpp>

ProgressIndicator::ProgressIndicator()
{
    ResourceManager *rm = ResourceManager::getInstance();
    ImageSet *images = rm->getImageSet("graphics/gui/progress-indicator.png",
                                       32, 32);

    Animation *anim = new Animation;
    for (ImageSet::size_type i = 0; i < images->size(); ++i)
        anim->addFrame(images->get(i), 100, 0, 0);

    mIndicator = new SimpleAnimation(anim);

    setSize(32, 32);
}

ProgressIndicator::~ProgressIndicator()
{
    delete mIndicator;
}

void ProgressIndicator::logic()
{
    mIndicator->update(10);
}

void ProgressIndicator::draw(gcn::Graphics *graphics)
{
    // Draw the indicator centered on the widget
    const int x = (getWidth() - 32) / 2;
    const int y = (getHeight() - 32) / 2;
    mIndicator->draw(static_cast<Graphics*>(graphics), x, y);
}
