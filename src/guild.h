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

#ifndef GUILD_H
#define GUILD_H

#include <guichan/listmodel.hpp>

#include <map>
#include <string>
#include <vector>

class Guild;

class GuildMember
{
public:
    GuildMember(int guildId, int id, const std::string &name);

    GuildMember(int guildId, int id);

    GuildMember(int guildId, const std::string &name);

    int getID() const { return mId; }

    void setID(int id) { mId = id; }

    std::string getName() const { return mName; }

    void setName(std::string name) { mName = name; }

    Guild *getGuild() const { return mGuild; }

    bool getOnline() const { return mOnline; }

    void setOnline(bool online) { mOnline = online; }

protected:
    friend class Guild;

    std::string mName;
    int mId;
    Guild *mGuild;
    bool mOnline;
};

class Guild : public gcn::ListModel
{
public:

    /**
     * Set the guild's name.
     */
    void setName(const std::string &name)
    {
        mName = name;
    }

    /**
     * Adds member to the list.
     */
    void addMember(GuildMember *member);

    /**
     * Find a member by ID.
     *
     * @return the member with the given ID, or NULL if they don't exist.
     */
    GuildMember *getMember(int id);

    /**
     * Find a member by name.
     *
     * @return the member with the given name, or NULL if they don't exist.
     */
    GuildMember *getMember(std::string name);

    /**
     * Get the name of the guild.
     * @return returns name of the guild
     */
    const std::string &getName() const
    {
        return mName;
    }

    /**
     * Get the id of the guild.
     * @return Returns the id of the guild
     */
    short getId() const
    {
        return mId;
    }

    /**
     * Removes a member from the guild.
     */
    void removeMember(GuildMember *member);

    /**
     * Removes a member from the guild.
     */
    void removeMember(int id);

    /**
     * Removes a member from the guild.
     */
    void removeMember(const std::string &name);

    void clearMembers() { mMembers.clear(); }

    /**
     * Get size of members list.
     * @return Returns the number of members in the guild.
     */
    int getNumberOfElements() {
        return mMembers.size();
    }

    /**
     * Get member at \a index.
     * @return Returns the name of member.
     */
    std::string getElementAt(int index);

    /**
     * Get whether user can invite users to this guild.
     * @return Returns true if user can invite users
     */
    bool getInviteRights() const
    {
        return mCanInviteUsers;
    }

    void setRights(short rights);

    bool isMember(GuildMember *member) const;

    bool isMember(int id) const;

    bool isMember(const std::string &name) const;

    static Guild *getGuild(int id);

private:
    typedef std::map<int, Guild*> GuildMap;
    static GuildMap guilds;

    /**
     * Constructor with guild id passed to it.
     */
    Guild(short id);

    typedef std::vector<GuildMember*> MemberList;
    MemberList mMembers;
    std::string mName;
    short mId;
    bool mCanInviteUsers;
};

#endif // GUILD_H
