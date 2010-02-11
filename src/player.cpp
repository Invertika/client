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

#include "animatedsprite.h"
#include "configuration.h"
#include "game.h"
#include "guild.h"
#include "localplayer.h"
#include "particle.h"
#include "party.h"
#include "player.h"
#include "text.h"

#include "gui/palette.h"
#include "gui/socialwindow.h"

#include "net/charhandler.h"
#include "net/net.h"

#include "resources/colordb.h"
#include "resources/itemdb.h"
#include "resources/iteminfo.h"

#include "utils/stringutils.h"

Player::Player(int id, int job, Map *map, bool isNPC):
    Being(id, job, map),
    mGender(GENDER_UNSPECIFIED),
    mParty(NULL),
    mIsGM(false)
{
    if (!isNPC)
    {
        for (unsigned int i = 0; i < Net::getCharHandler()->maxSprite(); i++)
        {
            mSprites.push_back(NULL);
            mSpriteIDs.push_back(0);
            mSpriteColors.push_back("");
        }

        /* Human base sprite. When implementing different races remove this
         * line and set the base sprite when setting the race of the player
         * character.
         */
        setSprite(Net::getCharHandler()->baseSprite(), -100);
    }
    mShowName = config.getValue("visiblenames", 1);
    config.addListener("visiblenames", this);

    updateColors();
}

Player::~Player()
{
    config.removeListener("visiblenames", this);
}

void Player::logic()
{
    if (Net::getNetworkType() == ServerInfo::EATHENA)
    {
        switch (mAction)
        {
            case STAND:
            case SIT:
            case DEAD:
            case HURT:
               break;

            case WALK:
                mFrame = (int) ((get_elapsed_time(mWalkTime) * 6)
                         / getWalkSpeed().x);
                if (mFrame >= 6)
                    nextTile();
                break;

            case ATTACK:
                int rotation = 0;
                std::string particleEffect = "";
                int frames = 4;

                if (mEquippedWeapon &&
                    mEquippedWeapon->getAttackType() == ACTION_ATTACK_BOW)
                {
                    frames = 5;
                }

                mFrame = (get_elapsed_time(mWalkTime) * frames) / mAttackSpeed;

                //attack particle effect
                if (mEquippedWeapon)
                    particleEffect = mEquippedWeapon->getParticleEffect();

                if (!particleEffect.empty() && Particle::enabled && mFrame == 1)
                {
                    switch (mDirection)
                    {
                        case DOWN: rotation = 0; break;
                        case LEFT: rotation = 90; break;
                        case UP: rotation = 180; break;
                        case RIGHT: rotation = 270; break;
                        default: break;
                    }
                    Particle *p;
                    p = particleEngine->addEffect("graphics/particles/" +
                                                  particleEffect, 0, 0, rotation);
                    controlParticle(p);
                }

                if (mFrame >= frames)
                    nextTile();

                break;
        }
    }

    Being::logic();
}

void Player::setGender(Gender gender)
{
    if (gender != mGender)
    {
        mGender = gender;

        // Reload all subsprites
        for (unsigned int i = 0; i < mSprites.size(); i++)
        {
            if (mSpriteIDs.at(i) != 0)
                setSprite(i, mSpriteIDs.at(i), mSpriteColors.at(i));
        }
    }
}

void Player::setGM(bool gm)
{
    mIsGM = gm;

    updateColors();
}

void Player::setSprite(unsigned int slot, int id, const std::string &color,
                       bool isWeapon)
{
    if (getType() == NPC)
        return;

    assert(slot < Net::getCharHandler()->maxSprite());

    // id = 0 means unequip
    if (id == 0)
    {
        delete mSprites[slot];
        mSprites[slot] = NULL;

        if (isWeapon)
            mEquippedWeapon = NULL;
    }
    else
    {
        std::string filename = ItemDB::get(id).getSprite(mGender);
        AnimatedSprite *equipmentSprite = NULL;

        if (!filename.empty())
        {
            if (!color.empty())
                filename += "|" + color;

            equipmentSprite = AnimatedSprite::load("graphics/sprites/" +
                                                   filename);
        }

        if (equipmentSprite)
            equipmentSprite->setDirection(getSpriteDirection());

        if (mSprites[slot])
            delete mSprites[slot];

        mSprites[slot] = equipmentSprite;

        if (isWeapon)
            mEquippedWeapon = &ItemDB::get(id);

        setAction(mAction);
    }

    mSpriteIDs[slot] = id;
    mSpriteColors[slot] = color;
}

void Player::setSpriteID(unsigned int slot, int id)
{
    setSprite(slot, id, mSpriteColors[slot]);
}

void Player::setSpriteColor(unsigned int slot, const std::string &color)
{
    setSprite(slot, mSpriteIDs[slot], color);
}

void Player::addGuild(Guild *guild)
{
    mGuilds[guild->getId()] = guild;

    if (this == player_node && socialWindow)
    {
        socialWindow->addTab(guild);
    }
}

void Player::removeGuild(int id)
{
    if (this == player_node && socialWindow)
    {
        socialWindow->removeTab(mGuilds[id]);
    }

    mGuilds.erase(id);
}

Guild *Player::getGuild(const std::string &guildName) const
{
    std::map<int, Guild*>::const_iterator itr, itr_end = mGuilds.end();
    for (itr = mGuilds.begin(); itr != itr_end; ++itr)
    {
        Guild *guild = itr->second;
        if (guild->getName() == guildName)
        {
            return guild;
        }
    }

    return NULL;
}

Guild *Player::getGuild(int id) const
{
    std::map<int, Guild*>::const_iterator itr;
    itr = mGuilds.find(id);
    if (itr != mGuilds.end())
    {
        return itr->second;
    }

    return NULL;
}

void Player::setParty(Party *party)
{
    if (party == mParty)
        return;

    Party *old = mParty;
    mParty = party;

    updateColors();

    if (this == player_node && socialWindow)
    {
        if (old)
            socialWindow->removeTab(old);

        if (party)
            socialWindow->addTab(party);
    }
}

void Player::optionChanged(const std::string &value)
{
    if (value == "visiblenames")
    {
        setShowName(config.getValue("visiblenames", 1));
    }
}

void Player::updateColors()
{
    mTextColor = &guiPalette->getColor(Palette::PLAYER);

    if (mIsGM)
    {
        mTextColor = &guiPalette->getColor(Palette::GM);
        mNameColor = &guiPalette->getColor(Palette::GM_NAME);
    }
    else if (mParty != NULL)
    {
        mNameColor = &guiPalette->getColor(Palette::PARTY);
    }
    else
    {
        mNameColor = &guiPalette->getColor(Palette::PC);
    }

    if (mDispName)
    {
        mDispName->setColor(mNameColor);
    }
}
