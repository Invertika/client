/*
 *  The Mana Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef EVENT_H
#define EVENT_H

#include <map>
#include <set>
#include <string>

class ActorSprite;
class Item;

enum Channels
{
    CHANNEL_ACTORSPRITE,
    CHANNEL_ATTRIBUTES,
    CHANNEL_BUYSELL,
    CHANNEL_CHAT,
    CHANNEL_CLIENT,
    CHANNEL_CONFIG,
    CHANNEL_GAME,
    CHANNEL_ITEM,
    CHANNEL_NOTICES,
    CHANNEL_NPC,
    CHANNEL_STATUS,
    CHANNEL_STORAGE
};

enum Events
{
    EVENT_ANNOUNCEMENT,
    EVENT_BEING,
    EVENT_CLOSE,
    EVENT_CLOSEALL,
    EVENT_CLOSESENT,
    EVENT_CONFIGOPTIONCHANGED,
    EVENT_CONSTRUCTED,
    EVENT_DBSLOADING,
    EVENT_DESTROYED,
    EVENT_DESTRUCTED,
    EVENT_DESTRUCTING,
    EVENT_DOCLOSEINVENTORY,
    EVENT_DODROP,
    EVENT_DOEQUIP,
    EVENT_DOMOVE,
    EVENT_DOSPLIT,
    EVENT_DOUNEQUIP,
    EVENT_DOUSE,
    EVENT_END,
    EVENT_ENGINESINITALIZED,
    EVENT_ENGINESINITALIZING,
    EVENT_GUIWINDOWSLOADED,
    EVENT_GUIWINDOWSLOADING,
    EVENT_GUIWINDOWSUNLOADED,
    EVENT_GUIWINDOWSUNLOADING,
    EVENT_INTEGERINPUT,
    EVENT_INTEGERINPUTSENT,
    EVENT_MAPLOADED,
    EVENT_MENU,
    EVENT_MENUSENT,
    EVENT_MESSAGE,
    EVENT_NEXT,
    EVENT_NEXTSENT,
    EVENT_NPCCOUNT,
    EVENT_PLAYER,
    EVENT_POST,
    EVENT_POSTCOUNT,
    EVENT_SENDLETTERSENT,
    EVENT_SERVERNOTICE,
    EVENT_STATECHANGE,
    EVENT_STORAGECOUNT,
    EVENT_STRINGINPUT,
    EVENT_STRINGINPUTSENT,
    EVENT_STUN,
    EVENT_TALKSENT,
    EVENT_TRADING,
    EVENT_UPDATEATTRIBUTE,
    EVENT_UPDATESTAT,
    EVENT_UPDATESTATUSEFFECT,
    EVENT_WHISPER,
    EVENT_WHISPERERROR
};

namespace Mana
{

// Possible exception that can be thrown
enum BadEvent {
    BAD_KEY,
    BAD_VALUE,
    KEY_ALREADY_EXISTS
};

class Listener;

typedef std::set<Listener *> ListenerSet;
typedef std::map<Channels, ListenerSet > ListenMap;

class VariableData;
typedef std::map<std::string, VariableData *> VariableMap;

#define SERVER_NOTICE(message) { \
Mana::Event event(EVENT_SERVERNOTICE); \
event.setString("message", message); \
event.trigger(CHANNEL_NOTICES, event); }

class Event
{
public:
    /**
     * Makes an event with the given name.
     */
    Event(Events name)
    { mEventName = name; }

    ~Event();

    /**
     * Returns the name of the event.
     */
    Events getName() const
    { return mEventName; }

// Integers

    /**
     * Sets the given variable to the given integer, if it isn't already set.
     */
    void setInt(const std::string &key, int value) throw (BadEvent);

    /**
     * Returns the given variable if it is set and an integer.
     */
    int getInt(const std::string &key) const throw (BadEvent);

    /**
     * Returns the given variable if it is set and an integer, returning the
     * given default otherwise.
     */
    inline int getInt(const std::string &key, int defaultValue) const
    { try { return getInt(key); } catch (BadEvent) { return defaultValue; }}

    /**
     * Returns true if the given variable exists and is an integer.
     */
    bool hasInt(const std::string &key) const;

// Strings

    /**
     * Sets the given variable to the given string, if it isn't already set.
     */
    void setString(const std::string &key, const std::string &value) throw (BadEvent);

    /**
     * Returns the given variable if it is set and a string.
     */
    const std::string &getString(const std::string &key) const throw (BadEvent);

    /**
     * Returns the given variable if it is set and a string, returning the
     * given default otherwise.
     */
    inline std::string getString(const std::string &key,
                                 const std::string &defaultValue) const
    { try { return getString(key); } catch (BadEvent) { return defaultValue; }}

    /**
     * Returns true if the given variable exists and is a string.
     */
    bool hasString(const std::string &key) const;

// Floats

    /**
     * Sets the given variable to the given floating-point, if it isn't already
     * set.
     */
    void setFloat(const std::string &key, double value) throw (BadEvent);

    /**
     * Returns the given variable if it is set and a floating-point.
     */
    double getFloat(const std::string &key) const throw (BadEvent);

    /**
     * Returns the given variable if it is set and a floating-point, returning
     * the given default otherwise.
     */
    inline double getFloat(const std::string &key, float defaultValue) const
    { try { return getFloat(key); } catch (BadEvent) { return defaultValue; }}

    /**
     * Returns true if the given variable exists and is a floating-point.
     */
    bool hasFloat(const std::string &key) const;

// Booleans

    /**
     * Sets the given variable to the given boolean, if it isn't already set.
     */
    void setBool(const std::string &key, bool value) throw (BadEvent);

    /**
     * Returns the given variable if it is set and a boolean.
     */
    bool getBool(const std::string &key) const throw (BadEvent);

    /**
     * Returns the given variable if it is set and a boolean, returning the
     * given default otherwise.
     */
    inline bool getBool(const std::string &key, bool defaultValue) const
    { try { return getBool(key); } catch (BadEvent) { return defaultValue; }}

    /**
     * Returns true if the given variable exists and is a boolean.
     */
    bool hasBool(const std::string &key) const;

// Items

    /**
     * Sets the given variable to the given Item, if it isn't already set.
     */
    void setItem(const std::string &key, Item *value) throw (BadEvent);

    /**
     * Returns the given variable if it is set and an Item.
     */
    Item *getItem(const std::string &key) const throw (BadEvent);

    /**
     * Returns the given variable if it is set and an Item, returning the
     * given default otherwise.
     */
    inline Item *getItem(const std::string &key, Item *defaultValue) const
    { try { return getItem(key); } catch (BadEvent) { return defaultValue; }}

    /**
     * Returns true if the given variable exists and is an Item.
     */
    bool hasItem(const std::string &key) const;

// ActorSprites

    /**
     * Sets the given variable to the given actor, if it isn't already set.
     */
    void setActor(const std::string &key, ActorSprite *value) throw (BadEvent);

    /**
     * Returns the given variable if it is set and an actor.
     */
    ActorSprite *getActor(const std::string &key) const throw (BadEvent);

    /**
     * Returns the given variable if it is set and an actor, returning the
     * given default otherwise.
     */
    inline ActorSprite *getActor(const std::string &key,
                                 ActorSprite *defaultValue) const
    { try { return getActor(key); } catch (BadEvent) { return defaultValue; }}

    /**
     * Returns true if the given variable exists and is an actor.
     */
    bool hasActor(const std::string &key) const;

// Triggers

    /**
     * Sends this event to all classes listening to the given channel.
     */
    inline void trigger(Channels channel) const
    { trigger(channel, *this); }

    /**
     * Sends the given event to all classes listening to the given channel.
     */
    static void trigger(Channels channel, const Event &event);

    /**
     * Sends an empty event with the given name to all classes listening to the
     * given channel.
     */
    static inline void trigger(Channels channel, Events name)
    { trigger(channel, Mana::Event(name)); }

protected:
    friend class Listener;

    /**
     * Binds the given listener to the given channel. The listener will receive
     * all events triggered on the channel.
     */
    static void bind(Listener *listener, Channels channel);

    /**
     * Unbinds the given listener from the given channel. The listener will no
     * longer receive any events from the channel.
     */
    static void unbind(Listener *listener, Channels channel);

    /**
     * Unbinds the given listener from all channels.
     */
    static void remove(Listener *listener);

private:
    static ListenMap mBindings;

    Events mEventName;

    VariableMap mData;
};

} // namespace Mana

#endif
