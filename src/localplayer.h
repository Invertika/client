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

#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include <memory>
#include <vector>

#include "player.h"
#include "gui/palette.h"

class Equipment;
class FloorItem;
class ImageSet;
class Inventory;
class Item;
class Map;


struct Special
{
    int currentMana;
    int neededMana;
    int recharge;
};


/**
 * Attributes used during combat. Available to all the beings.
 */
enum
{
BASE_ATTR_BEGIN = 0,
    BASE_ATTR_PHY_ATK_MIN = BASE_ATTR_BEGIN,
    BASE_ATTR_PHY_ATK_DELTA,
                       /**< Physical attack power. */
    BASE_ATTR_MAG_ATK, /**< Magical attack power. */
    BASE_ATTR_PHY_RES, /**< Resistance to physical damage. */
    BASE_ATTR_MAG_RES, /**< Resistance to magical damage. */
    BASE_ATTR_EVADE,   /**< Ability to avoid hits. */
    BASE_ATTR_HIT,     /**< Ability to hit stuff. */
    BASE_ATTR_HP,      /**< Hit Points (Base value: maximum, Modded value: current) */
    BASE_ATTR_HP_REGEN,/**< number of HP regenerated every 10 game ticks */
    BASE_ATTR_END,
    BASE_ATTR_NB = BASE_ATTR_END - BASE_ATTR_BEGIN,

    BASE_ELEM_BEGIN = BASE_ATTR_END,
    BASE_ELEM_NEUTRAL = BASE_ELEM_BEGIN,
    BASE_ELEM_FIRE,
    BASE_ELEM_WATER,
    BASE_ELEM_EARTH,
    BASE_ELEM_AIR,
    BASE_ELEM_SACRED,
    BASE_ELEM_DEATH,
    BASE_ELEM_END,
    BASE_ELEM_NB = BASE_ELEM_END - BASE_ELEM_BEGIN,

    NB_BEING_ATTRIBUTES = BASE_ELEM_END
};

/**
 * Attributes of characters. Used to derive being attributes.
 */
enum
{
    CHAR_ATTR_BEGIN = NB_BEING_ATTRIBUTES,
    CHAR_ATTR_STRENGTH = CHAR_ATTR_BEGIN,
    CHAR_ATTR_AGILITY,
    CHAR_ATTR_DEXTERITY,
    CHAR_ATTR_VITALITY,
    CHAR_ATTR_INTELLIGENCE,
    CHAR_ATTR_WILLPOWER,
    CHAR_ATTR_END,
    CHAR_ATTR_NB = CHAR_ATTR_END - CHAR_ATTR_BEGIN,

    NB_CHARACTER_ATTRIBUTES = CHAR_ATTR_END
};

/**
 * The local player character.
 */
class LocalPlayer : public Player
{
    public:
        /**
         * Constructor.
         */
        LocalPlayer(int id= 65535, int job = 0, Map *map = NULL);

        /**
         * Destructor.
         */
        ~LocalPlayer();

        virtual void logic();

        virtual void setAction(Action action, int attackType = 0);

        /**
         * Adds a new step when walking before calling super. Also, when
         * specified it picks up an item at the end of a path.
         */
        virtual void nextStep()
        { nextStep(0); }

        virtual void nextStep(unsigned char dir);

        /**
         * Returns the player's inventory.
         */
        Inventory *getInventory() const { return mInventory; }

        /**
         * Returns the player's storage
         */
        Inventory *getStorage() const { return mStorage; }

        /**
         * Check the player has permission to invite users to specific guild
         */
        bool checkInviteRights(const std::string &guildName);

        /**
         * Invite a player to join guild
         */
        void inviteToGuild(Being *being);

        void clearInventory();
        void setInvItem(int index, int id, int amount);

        void pickUp(FloorItem *item);

        /**
         * Sets the attack range.
         */
        void setAttackRange(int range) { mAttackRange = range; }

        /**
         * Gets the attack range.
         */
        int getAttackRange();

        /**
         * Returns true when the player is ready to accept a trade offer.
         * Returns false otherwise.
         */
        bool tradeRequestOk() const { return !mTrading; }

        /**
         * Sets the trading state of the player, i.e. whether or not he is
         * currently involved into some trade.
         */
        void setTrading(bool trading) { mTrading = trading; }

        void useSpecial(int id);

        void setSpecialStatus(int id, int current, int max, int recharge);

        const std::map<int, Special> &getSpecialStatus() const
        { return mSpecials; }

        void attack(Being *target = NULL, bool keep = false);

        /**
         * Triggers whether or not to show the name as a GM name.
         */
        virtual void setGM(bool gm);

        void setGMLevel(int level);

        void stopAttack();

        /**
         * Overridden to do nothing. The attacks of the local player are
         * displayed as soon as the player attacks, not when the server says
         * the player does.
         *
         * @param victim the victim being
         * @param damage the amount of damage dealt (0 means miss)
         * @param type the attack type
         */
        //virtual void handleAttack(Being *victim, int damage, AttackType type) {}
        virtual void handleAttack() {}

        /**
         * Returns the current target of the player. Returns 0 if no being is
         * currently targeted.
         */
        Being* getTarget() const;

        /**
         * Sets the target being of the player.
         */
        void setTarget(Being* target);

        /**
         * Sets a new destination for this being to walk to.
         */
#ifdef MANASERV_SUPPORT
        void setDestination(int x, int y);
#else
        virtual void setDestination(Uint16 x, Uint16 y);
#endif

        /**
         * Sets a new direction to keep walking in.
         */
        void setWalkingDir(int dir);

        /**
         * Gets the walking direction
         */
        int getWalkingDir() const
        { return mWalkingDir; }

        /**
         * Sets going to being to attack
         */
        void setGotoTarget(Being *target);

        /**
         * Returns whether the target is in range to attack
         */
        bool withinAttackRange(Being *target);

        /**
         * Stops the player dead in his tracks
         */
        void stopWalking(bool sendToServer = true);

        /**
         * Uses a character point to raise an attribute
         */
        void raiseAttribute(size_t attr);

        /**
         * Uses a correction point to lower an attribute
         */
        void lowerAttribute(size_t attr);

        void toggleSit();
        void emote(Uint8 emotion);

        /**
         * Shows item pickup notifications.
         */
        void pickedUp(const ItemInfo &itemInfo, int amount);

        /**
         * Accessors for mInStorage
         */
        bool getInStorage() { return mInStorage; }
        void setInStorage(bool inStorage);

        int getHp() const
        { return mHp; }

        int getMaxHp() const
        { return mMaxHp; }

        void setHp(int value);

        void setMaxHp(int value);

        int getLevel() const
        { return mLevel; }

        void setLevel(int value);

        void setExp(int value, bool notify = true);

        int getExp() const
        { return mExp; }

        void setExpNeeded(int value);

        int getExpNeeded() const
        { return mExpNeeded; }

        void setMP(int value);

        int getMP() const
        { return mMp; }

        void setMaxMP(int value);

        int getMaxMP() const
        { return mMaxMp; }

        int getMoney() const
        { return mMoney; }

        void setMoney(int value);

        int getTotalWeight() const
        { return mTotalWeight; }

        void setTotalWeight(int value)
        { mTotalWeight = value; }

        int getMaxWeight() const
        { return mMaxWeight; }

        void setMaxWeight(int value)
        { mMaxWeight = value; }

        int getAttributeBase(int num)
        { return mAttributeBase[num]; }

        void setAttributeBase(int num, int value);

        int getAttributeEffective(int num)
        { return mAttributeEffective[num]; }

        void setAttributeEffective(int num, int value);

        int getCharacterPoints() const
        { return mCharacterPoints; }

        void setCharacterPoints(int n);

        int getCorrectionPoints() const
        { return mCorrectionPoints; }

        void setCorrectionPoints(int n);

        int getSkillPoints() const
        { return mSkillPoints; }

        void setSkillPoints(int points);

        void setExperience(int skill, int current, int next);

        std::pair<int, int> getExperience(int skill);

        /** Tells that the path has been set by mouse. */
        void pathSetByMouse()
        { mPathSetByMouse = true; }

        /** Tells if the path has been set by mouse. */
        bool isPathSetByMouse() const
        { return mPathSetByMouse; }

        bool mUpdateName;     /** Whether or not the name settings have changed */

        bool mMapInitialized; /** Whether or not the map is available yet */

        const std::auto_ptr<Equipment> mEquipment;

        void addMessageToQueue(const std::string &message,
                               Palette::ColorType color = Palette::EXP_INFO);

        /**
         * Called when a option (set with config.addListener()) is changed
         */
        void optionChanged(const std::string &value);

    protected:
        virtual void handleStatusEffect(StatusEffect *effect, int effectId);

        // Colors don't change for local player
        virtual void updateColors() {}

        void startWalking(unsigned char dir);

        bool mInStorage;      /**< Whether storage is currently accessible */

        Uint16 mAttackRange;

        int mTargetTime;      /** How long the being has been targeted **/
        int mLastTarget;      /** Time stamp of last targeting action, -1 if none. */

        // Character status:
        typedef std::map<int, int> IntMap;
        IntMap mAttributeBase;
        IntMap mAttributeEffective;
        std::map<int, std::pair<int, int> > mSkillExp;
        int mCharacterPoints;
        int mCorrectionPoints;
        int mLevelProgress;
        std::map<int, Special> mSpecials;
        char mSpecialRechargeUpdateNeeded;
        int mLevel;
        int mExp, mExpNeeded;
        int mMp, mMaxMp;
        int mMoney;
        int mTotalWeight;
        int mMaxWeight;
        int mHp;
        int mMaxHp;
        int mSkillPoints;

        int mGMLevel;

        Being *mTarget;
        FloorItem *mPickUpTarget;

        bool mTrading;
        bool mGoingToTarget;
        bool mKeepAttacking;  /** Whether or not to continue to attack */
        int mLastAction;      /**< Time stamp of the last action, -1 if none. */
        int mWalkingDir;      /**< The direction the player is walking in. */
        bool mPathSetByMouse; /**< Tells if the path was set using mouse */
        int mDestX;           /**< X coordinate of destination. */
        int mDestY;           /**< Y coordinate of destination. */

        std::vector<int> mStatusEffectIcons;

        Inventory *mInventory;

#ifdef MANASERV_SUPPORT
        int mLocalWalkTime;   /**< Timestamp used to control keyboard walk
                                  messages flooding */
#endif

        Inventory *mStorage;

        // Load the target cursors into memory
        void initTargetCursor();

        /**
         * Helper function for loading target cursors
         */
        void loadTargetCursor(const std::string &filename,
                              int width, int height,
                              bool outRange, Being::TargetCursorSize size);

        /** Images of the target cursor. */
        ImageSet *mTargetCursorImages[2][NUM_TC];

        /** Animated target cursors. */
        SimpleAnimation *mTargetCursor[2][NUM_TC];

        typedef std::pair<std::string, Palette::ColorType> MessagePair;
        /** Queued exp messages*/
        std::list<MessagePair> mMessages;
        int mMessageTime;
};

extern LocalPlayer *player_node;

#endif
