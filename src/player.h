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

#ifndef PLAYER_H
#define PLAYER_H

#include "being.h"

class Graphics;
class Map;
class Guild;

enum Gender
{
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_UNSPECIFIED = 2
};

/**
 * A player being. Players have their name drawn beneath them. This class also
 * implements player-specific loading of base sprite, hair sprite and equipment
 * sprites.
 */
class Player : public Being
{
    public:
        enum Sprite
        {
            BASE_SPRITE = 0,
            SHOE_SPRITE,
            BOTTOMCLOTHES_SPRITE,
            TOPCLOTHES_SPRITE,
#ifdef EATHENA_SUPPORT
            MISC1_SPRITE,
            MISC2_SPRITE,
#endif
            HAIR_SPRITE,
            HAT_SPRITE,
#ifdef EATHENA_SUPPORT
            CAPE_SPRITE,
            GLOVES_SPRITE,
#endif
            WEAPON_SPRITE,
#ifdef EATHENA_SUPPORT
            SHIELD_SPRITE,
#endif
            VECTOREND_SPRITE
        };

        /**
         * Constructor.
         */
        Player(int id, int job, Map *map, bool isNPC = false);

        ~Player();

#ifdef EATHENA_SUPPORT
        virtual void logic();
#endif

        virtual Type getType() const { return PLAYER; }

        /**
         * Sets the gender of this being.
         */
        virtual void setGender(Gender gender);

        Gender getGender() const { return mGender; }

        /**
         * Whether or not this player is a GM.
         */
        bool isGM() const { return mIsGM; }

        /**
         * Triggers whether or not to show the name as a GM name.
         */
        virtual void setGM(bool gm);

        /**
         * Sets visible equipments for this player.
         */
        virtual void setSprite(unsigned int slot, int id,
                               const std::string &color = "");

        virtual void setSpriteID(unsigned int slot, int id);

        virtual void setSpriteColor(unsigned int slot,
                                    const std::string &color = "");

        /**
         * Adds a guild to the player.
         */
        Guild *addGuild(short guildId, short rights);

        /**
         * Removers a guild from the player.
         */
        void removeGuild(int id);

        /**
         * Returns a pointer to the specified guild.
         */
        Guild *getGuild(const std::string &guildName) const;

        /**
         * Returns a pointer to the guild with matching id.
         */
        Guild *getGuild(int id) const;

        /**
         * Get number of guilds the player belongs to.
         */
        short getNumberOfGuilds();

        /**
         * Set whether the player in the LocalPlayer's party. Players that are
         * in the same party as the local player get their name displayed in
         * a different color.
         */
        void setInParty(bool inParty);

        bool isInParty() const { return mInParty; }

        /**
         * Gets the way the character is blocked by other objects.
         */
        virtual unsigned char getWalkMask() const
        { return Map::BLOCKMASK_WALL | Map::BLOCKMASK_MONSTER; }

        /**
         * Called when a option (set with config.addListener()) is changed
         */
        virtual void optionChanged(const std::string &value);

    protected:
        /**
         * Gets the way the monster blocks pathfinding for other objects.
         */
        virtual Map::BlockType getBlockType() const
        { return Map::BLOCKTYPE_CHARACTER; }

        virtual void updateColors();

        Gender mGender;
        std::vector<int> mSpriteIDs;
        std::vector<std::string> mSpriteColors;

        // Character guild information
        std::map<int, Guild*> mGuilds;

        bool mIsGM;

    private:
        bool mInParty;
};

#endif
