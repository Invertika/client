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

#include "net/ea/beinghandler.h"

#include "being.h"
#include "beingmanager.h"
#include "effectmanager.h"
#include "game.h"
#include "localplayer.h"
#include "log.h"
#include "npc.h"
#include "playerrelations.h"

#include "gui/partywindow.h"

#include "net/ea/protocol.h"

#include "resources/colordb.h"

#include <iostream>

namespace EAthena {

const int EMOTION_TIME = 150;    /**< Duration of emotion icon */

BeingHandler::BeingHandler(bool enableSync):
   mSync(enableSync)
{
    static const Uint16 _messages[] = {
        SMSG_BEING_VISIBLE,
        SMSG_BEING_MOVE,
        SMSG_BEING_MOVE2,
        SMSG_BEING_REMOVE,
        SMSG_SKILL_DAMAGE,
        SMSG_BEING_ACTION,
        SMSG_BEING_SELFEFFECT,
        SMSG_BEING_EMOTION,
        SMSG_BEING_CHANGE_LOOKS,
        SMSG_BEING_CHANGE_LOOKS2,
        SMSG_BEING_NAME_RESPONSE,
        SMSG_BEING_CHANGE_DIRECTION,
        SMSG_PLAYER_UPDATE_1,
        SMSG_PLAYER_UPDATE_2,
        SMSG_PLAYER_MOVE,
        SMSG_PLAYER_STOP,
        SMSG_PLAYER_MOVE_TO_ATTACK,
        SMSG_PLAYER_STATUS_CHANGE,
        SMSG_BEING_STATUS_CHANGE,
        SMSG_BEING_RESURRECT,
        0
    };
    handledMessages = _messages;
}

Being *createBeing(int id, short job)
{
    Being::Type type = Being::UNKNOWN;
    if (job <= 25 || (job >= 4001 && job <= 4049))
        type = Being::PLAYER;
    else if (job >= 46 && job <= 1000)
        type = Being::NPC;
    else if (job > 1000 && job <= 2000)
    {
        type = Being::MONSTER;
        job -= 1002;
    }

    Being *being = beingManager->createBeing(id, type, job);

    if (type == Being::PLAYER || type == Being::NPC)
    {
        MessageOut outMsg(0x0094);
        outMsg.writeInt32(id);//readLong(2));
    }

    return being;
}

void BeingHandler::handleMessage(Net::MessageIn &msg)
{
    int id;
    short job, speed, gender;
    Uint16 headTop, headMid, headBottom;
    Uint16 shoes, gloves;
    Uint16 weapon, shield;
    Uint16 gmstatus;
    int param1;
    int stunMode;
    Uint32 statusEffects;
    int type;
    Uint16 status;
    Being *srcBeing, *dstBeing;
    Player *player;
    int hairStyle, hairColor, flag;
    std::string player_followed;

    switch (msg.getId())
    {
        case SMSG_BEING_VISIBLE:
        case SMSG_BEING_MOVE:
            if (!beingManager)
                return;

            // Information about a being in range
            id = msg.readInt32();
            speed = msg.readInt16();
            stunMode = msg.readInt16();  // opt1
            statusEffects = msg.readInt16();  // opt2
            statusEffects |= ((Uint32)msg.readInt16()) << 16;  // option
            job = msg.readInt16();  // class

            dstBeing = beingManager->findBeing(id);

            if (!dstBeing)
            {
                // Being with id >= 110000000 and job 0 are better
                // known as ghosts, so don't create those.
                if (job == 0 && id >= 110000000)
                {
                    break;
                }

                dstBeing = createBeing(id, job);
            }

            player = dynamic_cast<Player*>(dstBeing);

            // Fix monster jobs
            if (dstBeing->getType() == Being::MONSTER)
            {
                job -= 1002;
            }

            if (msg.getId() == 0x0078)
            {
                dstBeing->clearPath();
                dstBeing->setFrame(0);
                dstBeing->setWalkTime(tick_time);
                dstBeing->setAction(Being::STAND);
            }


            // Prevent division by 0 when calculating frame
            if (speed == 0) { speed = 150; }

            dstBeing->setWalkSpeed(speed);
            dstBeing->setJob(job);
            hairStyle = msg.readInt16();
            weapon = msg.readInt16();
            headBottom = msg.readInt16();

            if (msg.getId() == SMSG_BEING_MOVE)
            {
                msg.readInt32(); // server tick
            }

            shield = msg.readInt16();
            headTop = msg.readInt16();
            headMid = msg.readInt16();
            hairColor = msg.readInt16();
            shoes = msg.readInt16();  // clothes color - "abused" as shoes
            gloves = msg.readInt16();  // head dir - "abused" as gloves
            msg.readInt16();  // guild
            msg.readInt16();  // unknown
            msg.readInt16();  // unknown
            msg.readInt16();  // manner
            dstBeing->setStatusEffectBlock(32, msg.readInt16());  // opt3
            msg.readInt8();   // karma
            gender = msg.readInt8();

            if (player)
            {
                player->setGender((gender == 0)
                                  ? GENDER_FEMALE : GENDER_MALE);
                // Set these after the gender, as the sprites may be gender-specific
                player->setSprite(SPRITE_HAIR, hairStyle * -1, ColorDB::get(hairColor));
                player->setSprite(SPRITE_BOTTOMCLOTHES, headBottom);
                player->setSprite(SPRITE_TOPCLOTHES, headMid);
                player->setSprite(SPRITE_HAT, headTop);
                player->setSprite(SPRITE_SHOE, shoes);
                player->setSprite(SPRITE_GLOVES, gloves);
                player->setSprite(SPRITE_WEAPON, weapon, "", true);
                player->setSprite(SPRITE_SHIELD, shield);
            }

            if (msg.getId() == SMSG_BEING_MOVE)
            {
                Uint16 srcX, srcY, dstX, dstY;
                msg.readCoordinatePair(srcX, srcY, dstX, dstY);
                dstBeing->setAction(Being::STAND);
                dstBeing->setTileCoords(srcX, srcY);
                dstBeing->setDestination(dstX, dstY);
            }
            else
            {
                Uint8 dir;
                Uint16 x, y;
                msg.readCoordinates(x, y, dir);
                dstBeing->setTileCoords(x, y);
                dstBeing->setDirection(dir);
            }

            msg.readInt8();   // unknown
            msg.readInt8();   // unknown
            msg.readInt8();   // unknown / sit

            dstBeing->setStunMode(stunMode);
            dstBeing->setStatusEffectBlock(0, (statusEffects >> 16) & 0xffff);
            dstBeing->setStatusEffectBlock(16, statusEffects & 0xffff);
            break;

        case SMSG_BEING_MOVE2:
            /*
             * A simplified movement packet, used by the
             * later versions of eAthena for both mobs and
             * players
             */
            dstBeing = beingManager->findBeing(msg.readInt32());

            Uint16 srcX, srcY, dstX, dstY;
            msg.readCoordinatePair(srcX, srcY, dstX, dstY);
            msg.readInt32();  // Server tick

            /*
             * This packet doesn't have enough info to actually
             * create a new being, so if the being isn't found,
             * we'll just pretend the packet didn't happen
             */

            if (dstBeing)
            {
                dstBeing->setAction(Being::STAND);
                dstBeing->setTileCoords(srcX, srcY);
                dstBeing->setDestination(dstX, dstY);
            }

            break;

        case SMSG_BEING_REMOVE:
            // A being should be removed or has died
            id = msg.readInt32();

            dstBeing = beingManager->findBeing(id);

            if (!dstBeing)
                break;

            player_followed = player_node->getFollow();

            if (!player_followed.empty())
            {
                if (dstBeing->getName() == player_followed)
                {
                    player_node->setDestination(player_node->getNextDestX(), player_node->getNextDestY());
                }
            }

            // If this is player's current target, clear it.
            if (dstBeing == player_node->getTarget())
                player_node->stopAttack();

            if (msg.readInt8() == 1)
                dstBeing->setAction(Being::DEAD);
            else
                beingManager->destroyBeing(dstBeing);

            break;

        case SMSG_BEING_RESURRECT:
            // A being changed mortality status
            id = msg.readInt32();

            dstBeing = beingManager->findBeing(id);

            if (!dstBeing)
                break;

            // If this is player's current target, clear it.
            if (dstBeing == player_node->getTarget())
                player_node->stopAttack();

            if (msg.readInt8() == 1)
                dstBeing->setAction(Being::STAND);

            break;

        case SMSG_SKILL_DAMAGE:
            msg.readInt16(); // Skill Id
            srcBeing = beingManager->findBeing(msg.readInt32());
            dstBeing = beingManager->findBeing(msg.readInt32());
            msg.readInt32(); // Server tick
            msg.readInt32(); // src speed
            msg.readInt32(); // dst speed
            param1 = msg.readInt32(); // Damage
            msg.readInt16(); // Skill level
            msg.readInt16(); // Div
            msg.readInt8(); // Skill hit/type (?)
            if (dstBeing)
                dstBeing->takeDamage(srcBeing, param1, Being::HIT); // Perhaps a new skill attack type should be created and used?
            if (srcBeing)
                srcBeing->handleAttack(dstBeing, param1, Being::HIT);
            break;

        case SMSG_BEING_ACTION:
            srcBeing = beingManager->findBeing(msg.readInt32());
            dstBeing = beingManager->findBeing(msg.readInt32());
            msg.readInt32();   // server tick
            msg.readInt32();   // src speed
            msg.readInt32();   // dst speed
            param1 = msg.readInt16();
            msg.readInt16();  // param 2
            type = msg.readInt8();
            msg.readInt16();  // param 3

            switch (type)
            {
                case Being::HIT: // Damage
                case Being::CRITICAL: // Critical Damage
                case Being::MULTI: // Critical Damage
                case Being::REFLECT: // Reflected Damage
                case Being::FLEE: // Lucky Dodge
                    if (dstBeing)
                        dstBeing->takeDamage(srcBeing, param1,
                                (Being::AttackType)type);
                    if (srcBeing)
                        srcBeing->handleAttack(dstBeing, param1,
                                (Being::AttackType)type);
                    break;

                case 0x02: // Sit
                    if (srcBeing)
                    {
                        srcBeing->setFrame(0);
                        srcBeing->setAction(Being::SIT);
                    }
                    break;

                case 0x03: // Stand up
                    if (srcBeing)
                    {
                        srcBeing->setFrame(0);
                        srcBeing->setAction(Being::STAND);
                    }
                    break;
            }
            break;

        case SMSG_BEING_SELFEFFECT: {
            id = (Uint32)msg.readInt32();
            if (!beingManager->findBeing(id))
                break;

            int effectType = msg.readInt32();
            Being* being = beingManager->findBeing(id);

            effectManager->trigger(effectType, being);

            break;
        }

        case SMSG_BEING_EMOTION:
            if (!(dstBeing = beingManager->findBeing(msg.readInt32())))
            {
                break;
            }

            if (player_relations.hasPermission(dstBeing, PlayerRelation::EMOTE))
            {
                // only set emote if one doesnt already exist
                if (!dstBeing->getEmotion())
                    dstBeing->setEmote(msg.readInt8(), EMOTION_TIME);
            }

            break;

        case SMSG_BEING_CHANGE_LOOKS:
        case SMSG_BEING_CHANGE_LOOKS2:
        {
            /*
             * SMSG_BEING_CHANGE_LOOKS (0x00c3) and
             * SMSG_BEING_CHANGE_LOOKS2 (0x01d7) do basically the same
             * thing.  The difference is that ...LOOKS carries a single
             * 8 bit value, where ...LOOKS2 carries two 16 bit values.
             *
             * If type = 2, then the first 16 bit value is the weapon ID,
             * and the second 16 bit value is the shield ID.  If no
             * shield is equipped, or type is not 2, then the second
             * 16 bit value will be 0.
             */

            if (!(dstBeing = beingManager->findBeing(msg.readInt32())))
            {
                break;
            }

            player = dynamic_cast<Player*>(dstBeing);

            int type = msg.readInt8();
            int id = 0;
            int id2 = 0;

            if (msg.getId() == SMSG_BEING_CHANGE_LOOKS)
            {
                id = msg.readInt8();
            }
            else
            {        // SMSG_BEING_CHANGE_LOOKS2
                id = msg.readInt16();
                id2 = msg.readInt16();
            }

            switch (type)
            {
                case 1:     // eAthena LOOK_HAIR
                    player->setSpriteID(SPRITE_HAIR, id *-1);
                    break;
                case 2:     // Weapon ID in id, Shield ID in id2
                    player->setSprite(SPRITE_WEAPON, id, "", true);
                    player->setSprite(SPRITE_SHIELD, id2);
                    break;
                case 3:     // Change lower headgear for eAthena, pants for us
                    player->setSprite(SPRITE_BOTTOMCLOTHES, id);
                    break;
                case 4:     // Change upper headgear for eAthena, hat for us
                    player->setSprite(SPRITE_HAT, id);
                    break;
                case 5:     // Change middle headgear for eathena, armor for us
                    player->setSprite(SPRITE_TOPCLOTHES, id);
                    break;
                case 6:     // eAthena LOOK_HAIR_COLOR
                    player->setSpriteColor(SPRITE_HAIR, ColorDB::get(id));
                    break;
                case 8:     // eAthena LOOK_SHIELD
                    player->setSprite(SPRITE_SHIELD, id);
                    break;
                case 9:     // eAthena LOOK_SHOES
                    player->setSprite(SPRITE_SHOE, id);
                    break;
                case 10:   // LOOK_GLOVES
                    player->setSprite(SPRITE_GLOVES, id);
                    break;
                case 11:  // LOOK_CAPE
                    player->setSprite(SPRITE_CAPE, id);
                    break;
                case 12:
                    player->setSprite(SPRITE_MISC1, id);
                    break;
                case 13:
                    player->setSprite(SPRITE_MISC2, id);
                    break;
                default:
                    logger->log("SMSG_BEING_CHANGE_LOOKS: unsupported type: "
                            "%d, id: %d", type, id);
                    break;
            }
        }
            break;

        case SMSG_BEING_NAME_RESPONSE:
            if ((dstBeing = beingManager->findBeing(msg.readInt32())))
            {
                dstBeing->setName(msg.readString(24));
            }
            break;

        case SMSG_BEING_CHANGE_DIRECTION:
            if (!(dstBeing = beingManager->findBeing(msg.readInt32())))
            {
                break;
            }

            msg.readInt16(); // unused

            dstBeing->setDirection(msg.readInt8());

            break;

        case SMSG_PLAYER_UPDATE_1:
        case SMSG_PLAYER_UPDATE_2:
        case SMSG_PLAYER_MOVE:
            // An update about a player, potentially including movement.
            id = msg.readInt32();
            speed = msg.readInt16();
            stunMode = msg.readInt16();  // opt1; Aethyra use this as cape
            statusEffects = msg.readInt16();  // opt2; Aethyra use this as misc1
            statusEffects |= ((Uint32) msg.readInt16())
                << 16; // status.options; Aethyra uses this as misc2
            job = msg.readInt16();

            dstBeing = beingManager->findBeing(id);

            if (!dstBeing)
            {
                dstBeing = createBeing(id, job);
            }

            player = dynamic_cast<Player*>(dstBeing);

            {
                PartyMember *member = partyWindow->findMember(id);
                if (member && member->getAvatar()->getOnline())
                {
                    player->setInParty(true);
                }
            }

            dstBeing->setWalkSpeed(speed);
            dstBeing->setJob(job);
            hairStyle = msg.readInt16();
            weapon = msg.readInt16();
            shield = msg.readInt16();
            headBottom = msg.readInt16();

            if (msg.getId() == SMSG_PLAYER_MOVE)
            {
                msg.readInt32(); // server tick
            }

            headTop = msg.readInt16();
            headMid = msg.readInt16();
            hairColor = msg.readInt16();
            shoes = msg.readInt16();
            gloves = msg.readInt16();
            msg.readInt32();  // guild
            msg.readInt16();  // emblem
            msg.readInt16();  // manner
            dstBeing->setStatusEffectBlock(32, msg.readInt16());  // opt3
            msg.readInt8();   // karma
            player->setGender((msg.readInt8() == 0)
                              ? GENDER_FEMALE : GENDER_MALE);

            // Set these after the gender, as the sprites may be gender-specific
            player->setSprite(SPRITE_WEAPON, weapon, "", true);
            player->setSprite(SPRITE_SHIELD, shield);
            //player->setSprite(SPRITE_SHOE, shoes);
            player->setSprite(SPRITE_BOTTOMCLOTHES, headBottom);
            player->setSprite(SPRITE_TOPCLOTHES, headMid);
            player->setSprite(SPRITE_HAT, headTop);
            //player->setSprite(SPRITE_GLOVES, gloves);
            //player->setSprite(SPRITE_CAPE, cape);
            //player->setSprite(SPRITE_MISC1, misc1);
            //player->setSprite(SPRITE_MISC2, misc2);
            player->setSprite(SPRITE_HAIR, hairStyle * -1, ColorDB::get(hairColor));

            if (msg.getId() == SMSG_PLAYER_MOVE)
            {
                Uint16 srcX, srcY, dstX, dstY;
                msg.readCoordinatePair(srcX, srcY, dstX, dstY);
                dstBeing->setTileCoords(srcX, srcY);
                dstBeing->setDestination(dstX, dstY);

                player_followed = player_node->getFollow();
                if (!player_followed.empty())
                {
                    if (dstBeing->getName() == player_followed)
                    {
                        player_node->setNextDest(dstX, dstY);
                        player_node->setDestination(srcX, srcY);
                    }
                }
            }
            else
            {
                Uint8 dir;
                Uint16 x, y;
                msg.readCoordinates(x, y, dir);
                dstBeing->setTileCoords(x, y);
                dstBeing->setDirection(dir);
            }

            gmstatus = msg.readInt16();
            if (gmstatus & 0x80)
                player->setGM(true);

            if (msg.getId() == SMSG_PLAYER_UPDATE_1)
            {
                switch (msg.readInt8())
                {
                    case 1:
                        dstBeing->setAction(Being::DEAD);
                        break;

                    case 2:
                        dstBeing->setAction(Being::SIT);
                        break;
                }
            }
            else if (msg.getId() == SMSG_PLAYER_MOVE)
            {
                msg.readInt8(); // unknown
            }

            msg.readInt8();   // Lv
            msg.readInt8();   // unknown

            dstBeing->setWalkTime(tick_time);
            dstBeing->setFrame(0);

            dstBeing->setStunMode(stunMode);
            dstBeing->setStatusEffectBlock(0, (statusEffects >> 16) & 0xffff);
            dstBeing->setStatusEffectBlock(16, statusEffects & 0xffff);
            break;

        case SMSG_PLAYER_STOP:
            /*
             *  Instruction from server to stop walking at x, y.
             *
             *  Some people like having this enabled.  Others absolutely
             *  despise it.  So I'm setting to so that it only affects the
             *  local player if the person has set a key "EnableSync" to "1"
             *  in their config.xml file.
             *
             *  This packet will be honored for all other beings, regardless
             *  of the config setting.
             */

            id = msg.readInt32();
            if (mSync || id != player_node->getId())
            {
                dstBeing = beingManager->findBeing(id);
                if (dstBeing)
                {
                    Uint16 x, y;
                    x = msg.readInt16();
                    y = msg.readInt16();
                    dstBeing->setTileCoords(x, y);
                    if (dstBeing->getCurrentAction() == Being::WALK)
                    {
                        dstBeing->setFrame(0);
                        dstBeing->setAction(Being::STAND);
                    }
                }
            }
            break;

        case SMSG_PLAYER_MOVE_TO_ATTACK:
            /*
             * This is an *advisory* message, telling the client that
             * it needs to move the character before attacking
             * a target (out of range, obstruction in line of fire).
             * We can safely ignore this...
             */
            break;

        case SMSG_PLAYER_STATUS_CHANGE:
            // Change in players' flags
            id = msg.readInt32();
            dstBeing = beingManager->findBeing(id);
            stunMode = msg.readInt16();
            statusEffects = msg.readInt16();
            statusEffects |= ((Uint32) msg.readInt16()) << 16;
            msg.readInt8();

            if (dstBeing)
            {
                dstBeing->setStunMode(stunMode);
                dstBeing->setStatusEffectBlock(0, (statusEffects >> 16) & 0xffff);
                dstBeing->setStatusEffectBlock(16, statusEffects & 0xffff);
            }
            break;

        case SMSG_BEING_STATUS_CHANGE:
            // Status change
            status = msg.readInt16();
            id = msg.readInt32();
            flag = msg.readInt8(); // 0: stop, 1: start

            dstBeing = beingManager->findBeing(id);
            if (dstBeing)
                dstBeing->setStatusEffect(status, flag);
            break;
    }
}

} // namespace EAthena
