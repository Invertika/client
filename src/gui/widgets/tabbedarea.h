/*
 *  The Mana Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef TABBEDAREA_H
#define TABBEDAREA_H

#include <guichan/widget.hpp>
#include <guichan/widgetlistener.hpp>
#include <guichan/widgets/container.hpp>
#include <guichan/widgets/tabbedarea.hpp>

#include <string>

class Tab;

/**
 * A tabbed area, the same as the guichan tabbed area in 0.8, but extended
 */
class TabbedArea : public gcn::TabbedArea, public gcn::WidgetListener
{
    public:
        /**
         * Constructor.
         */
        TabbedArea();

        /**
         * Draw the tabbed area.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Return how many tabs have been created.
         *
         * @todo Remove this method when upgrading to Guichan 0.9.0
         */
        int getNumberOfTabs() const;

        /**
         * Return tab with specified name as caption.
         */
        Tab *getTab(const std::string &name) const;

        /**
         * Returns the widget with the tab that has specified caption
         */
        gcn::Widget *getWidget(const std::string &name) const;

        /**
         * Returns the widget for the current tab
         */
        gcn::Widget *getCurrentWidget();

        using gcn::TabbedArea::addTab;

        /**
         * Add a tab. Overridden since it needs to size the widget.
         *
         * @param tab The tab widget for the tab.
         * @param widget The widget to view when the tab is selected.
         */
        void addTab(gcn::Tab* tab, gcn::Widget* widget);

        /**
         * Add a tab. Overridden since it needs to create an instance of Tab
         * instead of gcn::Tab.
         *
         * @param caption The Caption to display
         * @param widget The widget to show when tab is selected
         */
        void addTab(const std::string &caption, gcn::Widget *widget);

        /**
         * Overload the remove tab function as it's broken in guichan 0.8.
         */
        void removeTab(Tab *tab);

        /**
         * Overload the logic function since it's broken in guichan 0.8.
         */
        void logic();

        int getContainerHeight() const
        { return mWidgetContainer->getHeight(); }

        using gcn::TabbedArea::setSelectedTab;

        void setSelectedTab(gcn::Tab *tab);

        void widgetResized(const gcn::Event &event);

        // Inherited from MouseListener

        void mousePressed(gcn::MouseEvent &mouseEvent);
    private:
        typedef std::vector< std::pair<gcn::Tab*, gcn::Widget*> > TabContainer;
};

#endif
