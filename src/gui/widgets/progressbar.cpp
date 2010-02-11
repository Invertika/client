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

#include "gui/widgets/progressbar.h"

#include "configuration.h"
#include "graphics.h"
#include "textrenderer.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/skin.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "utils/dtor.h"

#include <guichan/font.hpp>

ImageRect ProgressBar::mBorder;
int ProgressBar::mInstances = 0;
float ProgressBar::mAlpha = 1.0;

ProgressBar::ProgressBar(float progress,
                         int width, int height,
                         const gcn::Color &color):
    gcn::Widget(),
    mSmoothProgress(true),
    mColor(color),
    mColorToGo(color),
    mSmoothColorChange(true)
{
    // The progress value is directly set at load time:
    if (progress > 1.0f || progress < 0.0f)
        progress = 1.0f;

    mProgress = mProgressToGo = progress;

    setSize(width, height);

    if (mInstances == 0)
    {
        ResourceManager *resman = ResourceManager::getInstance();
        Image *dBorders = resman->getImage("graphics/gui/vscroll_grey.png");
        mBorder.grid[0] = dBorders->getSubImage(0, 0, 4, 4);
        mBorder.grid[1] = dBorders->getSubImage(4, 0, 3, 4);
        mBorder.grid[2] = dBorders->getSubImage(7, 0, 4, 4);
        mBorder.grid[3] = dBorders->getSubImage(0, 4, 4, 10);
        mBorder.grid[4] = dBorders->getSubImage(4, 4, 3, 10);
        mBorder.grid[5] = dBorders->getSubImage(7, 4, 4, 10);
        mBorder.grid[6] = dBorders->getSubImage(0, 15, 4, 4);
        mBorder.grid[7] = dBorders->getSubImage(4, 15, 3, 4);
        mBorder.grid[8] = dBorders->getSubImage(7, 15, 4, 4);

        for (int i = 0; i < 9; i++)
        {
            mBorder.grid[i]->setAlpha(mAlpha);
        }

        dBorders->decRef();
    }

    mInstances++;
}

ProgressBar::~ProgressBar()
{
    mInstances--;

    if (mInstances == 0)
    {
        for_each(mBorder.grid, mBorder.grid + 9, dtor<Image*>());
    }
}

void ProgressBar::logic()
{
    if (mSmoothColorChange)
    {
        // Smoothly changing the color for a nicer effect.
        if (mColorToGo.r > mColor.r)
            mColor.r++;
        if (mColorToGo.g > mColor.g)
            mColor.g++;
        if (mColorToGo.b > mColor.b)
            mColor.b++;
        if (mColorToGo.r < mColor.r)
            mColor.r--;
        if (mColorToGo.g < mColor.g)
            mColor.g--;
        if (mColorToGo.b < mColor.b)
            mColor.b--;
    }

    if (mSmoothProgress)
    {
        // Smoothly showing the progressbar changes.
        if (mProgressToGo > mProgress)
            mProgress = std::min(1.0f, mProgress + 0.005f);
        if (mProgressToGo < mProgress)
            mProgress = std::max(0.0f, mProgress - 0.005f);
    }
}

void ProgressBar::updateAlpha()
{
    float alpha = std::max(config.getValue("guialpha", 0.8),
                   (double)SkinLoader::instance()->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;
        for (int i = 0; i < 9; i++)
        {
            mBorder.grid[i]->setAlpha(mAlpha);
        }
    }

}

void ProgressBar::draw(gcn::Graphics *graphics)
{
    updateAlpha();

    mColor.a = (int) (mAlpha * 255);

    gcn::Rectangle rect = getDimension();
    rect.x = 0;
    rect.y = 0;

    render(static_cast<Graphics*>(graphics), rect, mColor,
           mProgress, mText);
}

void ProgressBar::setProgress(float progress)
{
    const float p = std::min(1.0f, std::max(0.0f, progress));
    mProgressToGo = p;

    if (!mSmoothProgress)
        mProgress = p;
}

void ProgressBar::setColor(const gcn::Color &color)
{
    mColorToGo = color;

    if (!mSmoothColorChange)
        mColor = color;
}

void ProgressBar::render(Graphics *graphics, const gcn::Rectangle &area,
                         const gcn::Color &color, float progress,
                         const std::string &text)
{
    gcn::Font *oldFont = graphics->getFont();
    gcn::Color oldColor = graphics->getColor();

    graphics->drawImageRect(area, mBorder);

    // The bar
    if (progress > 0)
    {
        graphics->setColor(color);
        graphics->fillRectangle(gcn::Rectangle(area.x + 4, area.y + 4,
                               (int) (progress * (area.width - 8)),
                                area.height - 8));
    }

    // The label
    if (!text.empty())
    {
        const int textX = area.x + area.width / 2;
        const int textY = area.y + (area.height - boldFont->getHeight()) / 2;

        TextRenderer::renderText(graphics, text, textX, textY,
                                 gcn::Graphics::CENTER,
                                 guiPalette->getColor(Palette::PROGRESS_BAR),
                                 gui->getFont(), true, false);
    }

    graphics->setFont(oldFont);
    graphics->setColor(oldColor);
}
