var Protocol =
{ 
		/**
		 * Enumerated type for communicated messages:
		 *
		 * - PAMSG_*: from client to account server
		 * - APMSG_*: from account server to client
		 * - PCMSG_*: from client to chat server
		 * - CPMSG_*: from chat server to client
		 * - PGMSG_*: from client to game server
		 * - GPMSG_*: from game server to client
		 * - GAMSG_*: from game server to account server
		 *
		 * Components: B byte, W word, D double word, S variable-size string
		 *             C tile-based coordinates (B*3)
		 *
		 * Hosts:      P (player's client), A (account server), C (char server),
		 *             G (game server)
		 *
		 * TODO - Document specific error codes for each packet
		 */

		// Login/Register
		PAMSG_REGISTER:0x0000, // D version, S username, S password, S email, S captcha response
		APMSG_REGISTER_RESPONSE:0x0002, // B error, S updatehost, S Client data URL, B Character slots
		PAMSG_UNREGISTER:0x0003, // S username, S password
		APMSG_UNREGISTER_RESPONSE:0x0004, // B error
		PAMSG_REQUEST_REGISTER_INFO:0x0005, //
		APMSG_REGISTER_INFO_RESPONSE:0x0006, // B byte registration Allowed, byte minNameLength, byte maxNameLength, string captchaURL, string captchaInstructions
		PAMSG_LOGIN:0x0010, // D version, S username, S password
		APMSG_LOGIN_RESPONSE:0x0012, // B error, S updatehost, S Client data URL, B Character slots
		PAMSG_LOGOUT:0x0013, // -
		APMSG_LOGOUT_RESPONSE:0x0014, // B error
		PAMSG_LOGIN_RNDTRGR:0x0015, // S username
		APMSG_LOGIN_RNDTRGR_RESPONSE:0x0016, // S random seed
		PAMSG_CHAR_CREATE:0x0020, // S name, B hair style, B hair color, B gender, B slot, {W stats}*
		APMSG_CHAR_CREATE_RESPONSE:0x0021, // B error
		PAMSG_CHAR_DELETE:0x0022, // B slot
		APMSG_CHAR_DELETE_RESPONSE:0x0023, // B error
		// B slot, S name, B gender, B hair style, B hair color, W level,
		// W character points, W correction points,
		// {D attr id, D base value (in 1/256ths) D mod value (in 256ths) }*
		APMSG_CHAR_INFO:0x0024, // ^
		PAMSG_CHAR_SELECT:0x0026, // B slot
		APMSG_CHAR_SELECT_RESPONSE:0x0027, // B error, B*32 token, S game address, W game port, S chat address, W chat port
		PAMSG_EMAIL_CHANGE:0x0030, // S email
		APMSG_EMAIL_CHANGE_RESPONSE:0x0031, // B error
		PAMSG_PASSWORD_CHANGE:0x0034, // S old password, S new password
		APMSG_PASSWORD_CHANGE_RESPONSE:0x0035, // B error

		PGMSG_CONNECT:0x0050, // B*32 token
		GPMSG_CONNECT_RESPONSE:0x0051, // B error
		PCMSG_CONNECT:0x0053, // B*32 token
		CPMSG_CONNECT_RESPONSE:0x0054, // B error

		PGMSG_DISCONNECT:0x0060, // B reconnect account
		GPMSG_DISCONNECT_RESPONSE:0x0061, // B error, B*32 token
		PCMSG_DISCONNECT:0x0063, // -
		CPMSG_DISCONNECT_RESPONSE:0x0064, // B error

		PAMSG_RECONNECT:0x0065, // B*32 token
		APMSG_RECONNECT_RESPONSE:0x0066, // B error

		// Game
		GPMSG_PLAYER_MAP_CHANGE:0x0100, // S filename, W x, W y
		GPMSG_PLAYER_SERVER_CHANGE:0x0101, // B*32 token, S game address, W game port
		PGMSG_PICKUP:0x0110, // W*2 position
		PGMSG_DROP:0x0111, // B slot, B amount
		PGMSG_EQUIP:0x0112, // W inventory slot
		PGMSG_UNEQUIP:0x0113, // W item Instance id
		PGMSG_MOVE_ITEM:0x0114, // W slot1, W slot2, W amount
		GPMSG_INVENTORY:0x0120, // { W slot, W item id [, W amount] (if item id is nonzero) }*
		GPMSG_INVENTORY_FULL:0x0121, // W inventory slot count { W slot, W itemId, W amount }, { W equip slot, W item Id, W item Instance}*
		GPMSG_EQUIP:0x0122, // W item Id, W equip slot type count //{ W equip slot, W capacity used}*//<- When equipping, //{ W item instance, W 0}*//<- When unequipping
		GPMSG_PLAYER_ATTRIBUTE_CHANGE:0x0130, // { W attribute, D base value (in 1/256ths), D modified value (in 1/256ths)}*
		GPMSG_PLAYER_EXP_CHANGE:0x0140, // { W skill, D exp got, D exp needed }*
		GPMSG_LEVELUP:0x0150, // W new level, W character points, W correction points
		GPMSG_LEVEL_PROGRESS:0x0151, // B percent completed to next levelup
		PGMSG_RAISE_ATTRIBUTE:0x0160, // W attribute
		GPMSG_RAISE_ATTRIBUTE_RESPONSE:0x0161, // B error, W attribute
		PGMSG_LOWER_ATTRIBUTE:0x0170, // W attribute
		GPMSG_LOWER_ATTRIBUTE_RESPONSE:0x0171, // B error, W attribute
		PGMSG_RESPAWN:0x0180, // -
		GPMSG_BEING_ENTER:0x0200, // B type, W being id, B action, W*2 position, B direction

		// character: S name, B hair style, B hair color, B gender, B sprite layers changed, { B slot type, W item id }*
		// monster: W type id gender
		// npc: W type id gender

		GPMSG_BEING_LEAVE:0x0201, // W being id
		GPMSG_ITEM_APPEAR:0x0202, // W item id, W*2 position
		GPMSG_BEING_LOOKS_CHANGE:0x0210, // B sprite layers changed, { B slot type, W item id }*
		PGMSG_WALK:0x0260, // W*2 destination
		PGMSG_ACTION_CHANGE:0x0270, // B Action
		GPMSG_BEING_ACTION_CHANGE:0x0271, // W being id, B action
		PGMSG_DIRECTION_CHANGE:0x0272, // B Direction
		GPMSG_BEING_DIR_CHANGE:0x0273, // W being id, B direction
		GPMSG_BEING_HEALTH_CHANGE:0x0274, // W being id, W hp, W max hp
		GPMSG_BEINGS_MOVE:0x0280, // { W being id, B flags [, [W*2 position,] W*2 destination, B speed] }*
		GPMSG_ITEMS:0x0281, // { W item id, W*2 position }*
		PGMSG_ATTACK:0x0290, // W being id
		GPMSG_BEING_ATTACK:0x0291, // W being id, B direction, B attack Id
		PGMSG_USE_SPECIAL:0x0292, // B specialID
		GPMSG_SPECIAL_STATUS:0x0293, // { B specialID, D current, D max, D recharge }
		PGMSG_SAY:0x02A0, // S text
		GPMSG_SAY:0x02A1, // W being id, S text
		GPMSG_NPC_CHOICE:0x02B0, // W being id, { S text }*
		GPMSG_NPC_MESSAGE:0x02B1, // W being id, B* text
		PGMSG_NPC_TALK:0x02B2, // W being id
		PGMSG_NPC_TALK_NEXT:0x02B3, // W being id
		PGMSG_NPC_SELECT:0x02B4, // W being id, B choice
		GPMSG_NPC_BUY:0x02B5, // W being id, { W item id, W amount, W cost }*
		GPMSG_NPC_SELL:0x02B6, // W being id, { W item id, W amount, W cost }*
		PGMSG_NPC_BUYSELL:0x02B7, // W item id, W amount
		GPMSG_NPC_ERROR:0x02B8, // B error
		GPMSG_NPC_CLOSE:0x02B9, // W being id
		GPMSG_NPC_POST:0x02D0, // W being id
		PGMSG_NPC_POST_SEND:0x02D1, // W being id, { S name, S text, W item id }
		GPMSG_NPC_POST_GET:0x02D2, // W being id, { S name, S text, W item id }
		PGMSG_NPC_NUMBER:0x02D3, // W being id, D number
		PGMSG_NPC_STRING:0x02D4, // W being id, S string
		GPMSG_NPC_NUMBER:0x02D5, // W being id, D max, D min, D default
		GPMSG_NPC_STRING:0x02D6, // W being id
		PGMSG_TRADE_REQUEST:0x02C0, // W being id
		GPMSG_TRADE_REQUEST:0x02C1, // W being id
		GPMSG_TRADE_START:0x02C2, // -
		GPMSG_TRADE_COMPLETE:0x02C3, // -
		PGMSG_TRADE_CANCEL:0x02C4, // -
		GPMSG_TRADE_CANCEL:0x02C5, // -
		PGMSG_TRADE_AGREED:0x02C6, // -
		GPMSG_TRADE_AGREED:0x02C7, // -
		PGMSG_TRADE_CONFIRM:0x02C8, // -
		GPMSG_TRADE_CONFIRM:0x02C9, // -
		PGMSG_TRADE_ADD_ITEM:0x02CA, // B slot, B amount
		GPMSG_TRADE_ADD_ITEM:0x02CB, // W item id, B amount
		PGMSG_TRADE_SET_MONEY:0x02CC, // D amount
		GPMSG_TRADE_SET_MONEY:0x02CD, // D amount
		GPMSG_TRADE_BOTH_CONFIRM:0x02CE, // -
		PGMSG_USE_ITEM:0x0300, // B slot
		GPMSG_USE_RESPONSE:0x0301, // B error
		GPMSG_BEINGS_DAMAGE:0x0310, // { W being id, W amount }*
		GPMSG_CREATE_EFFECT_POS:0x0320, // W effect id, W*2 position
		GPMSG_CREATE_EFFECT_BEING:0x0321, // W effect id, W BeingID
		GPMSG_SHAKE:0x0330, // W intensityX, W intensityY, [W decay_times_10000, [W duration]]

		// Guild
		PCMSG_GUILD_CREATE:0x0350, // S name
		CPMSG_GUILD_CREATE_RESPONSE:0x0351, // B error, W guild, B rights, W channel
		PCMSG_GUILD_INVITE:0x0352, // W id, S name
		CPMSG_GUILD_INVITE_RESPONSE:0x0353, // B error
		PCMSG_GUILD_ACCEPT:0x0354, // W id
		CPMSG_GUILD_ACCEPT_RESPONSE:0x0355, // B error, W guild, B rights, W channel
		PCMSG_GUILD_GET_MEMBERS:0x0356, // W id
		CPMSG_GUILD_GET_MEMBERS_RESPONSE:0x0357, // S names, B online
		CPMSG_GUILD_UPDATE_LIST:0x0358, // W id, S name, B event
		PCMSG_GUILD_QUIT:0x0360, // W id
		CPMSG_GUILD_QUIT_RESPONSE:0x0361, // B error
		PCMSG_GUILD_PROMOTE_MEMBER:0x0365, // W guild, S name, B rights
		CPMSG_GUILD_PROMOTE_MEMBER_RESPONSE:0x0366, // B error
		PCMSG_GUILD_KICK_MEMBER:0x0370, // W guild, S name
		CPMSG_GUILD_KICK_MEMBER_RESPONSE:0x0371, // B error

		CPMSG_GUILD_INVITED:0x0388, // S char name, S  guild name, W id
		CPMSG_GUILD_REJOIN:0x0389, // S name, W guild, W rights, W channel, S announce

		// Party
		PGMSG_PARTY_INVITE:0x03A0, // S name
		GPMSG_PARTY_INVITE_ERROR:0x03A1, // S name
		GCMSG_PARTY_INVITE:0x03A2, // S inviter, S invitee
		CPMSG_PARTY_INVITED:0x03A4, // S name
		PCMSG_PARTY_INVITE_ANSWER:0x03A5, // S name, B accept
		CPMSG_PARTY_INVITE_ANSWER_RESPONSE:0x03A6, // B error, { S name }
		CPMSG_PARTY_REJECTED:0x03A8, // S name, B error
		PCMSG_PARTY_QUIT:0x03AA, // -
		CPMSG_PARTY_QUIT_RESPONSE:0x03AB, // B error
		CPMSG_PARTY_NEW_MEMBER:0x03B0, // S name, S inviter
		CPMSG_PARTY_MEMBER_LEFT:0x03B1, // D character id

		// Chat
		CPMSG_ERROR:0x0401, // B error
		CPMSG_ANNOUNCEMENT:0x0402, // S text
		CPMSG_PRIVMSG:0x0403, // S user, S text
		CPMSG_PUBMSG:0x0404, // W channel, S user, S text
		PCMSG_CHAT:0x0410, // S text, W channel
		PCMSG_ANNOUNCE:0x0411, // S text
		PCMSG_PRIVMSG:0x0412, // S user, S text
		PCMSG_WHO:0x0415, // -
		CPMSG_WHO_RESPONSE:0x0416, // { S user }

		// -- Channeling
		CPMSG_CHANNEL_EVENT:0x0430, // W channel, B event, S info
		PCMSG_ENTER_CHANNEL:0x0440, // S channel, S password
		CPMSG_ENTER_CHANNEL_RESPONSE:0x0441, // B error, W id, S name, S topic, S userlist
		PCMSG_QUIT_CHANNEL:0x0443, // W channel id
		CPMSG_QUIT_CHANNEL_RESPONSE:0x0444, // B error, W channel id
		PCMSG_LIST_CHANNELS:0x0445, // -
		CPMSG_LIST_CHANNELS_RESPONSE:0x0446, // S names, W number of users
		PCMSG_LIST_CHANNELUSERS:0x0460, // S channel
		CPMSG_LIST_CHANNELUSERS_RESPONSE:0x0461, // S channel, { S user, B mode }
		PCMSG_TOPIC_CHANGE:0x0462, // W channel id, S topic

		// -- User modes
		PCMSG_USER_MODE:0x0465, // W channel id, S name, B mode
		PCMSG_KICK_USER:0x0466, // W channel id, S name

		// Inter-server
		GAMSG_REGISTER:0x0500, // S address, W port, S password, D items db revision, { W map id }*
		AGMSG_REGISTER_RESPONSE:0x0501, // C item version, C password response, { S globalvar_key, S globalvar_value }
		AGMSG_ACTIVE_MAP:0x0502, // W map id, W Number of mapvar_key mapvar_value sent, { S mapvar_key, S mapvar_value }, W Number of map items, { D item Id, W amount, W posX, W posY }
		AGMSG_PLAYER_ENTER:0x0510, // B*32 token, D id, S name, serialised character data
		GAMSG_PLAYER_DATA:0x0520, // D id, serialised character data
		GAMSG_REDIRECT:0x0530, // D id
		AGMSG_REDIRECT_RESPONSE:0x0531, // D id, B*32 token, S game address, W game port
		GAMSG_PLAYER_RECONNECT:0x0532, // D id, B*32 token
		GAMSG_PLAYER_SYNC:0x0533, // serialised sync data
		GAMSG_SET_VAR_CHR:0x0540, // D id, S name, S value
		GAMSG_GET_VAR_CHR:0x0541, // D id, S name
		AGMSG_GET_VAR_CHR_RESPONSE:0x0542, // D id, S name, S value
		//reserved GAMSG_SET_VAR_ACC           : 0x0543, // D charid, S name, S value
		//reserved GAMSG_GET_VAR_ACC           : 0x0544, // D charid, S name
		//reserved AGMSG_GET_VAR_ACC_RESPONSE  : 0x0545, // D charid, S name, S value
		GAMSG_SET_VAR_MAP:0x0546, // D mapid, S name, S value
		GAMSG_SET_VAR_WORLD:0x0547, // S name, S value
		AGMSG_SET_VAR_WORLD:0x0548, // S name, S value
		GAMSG_BAN_PLAYER:0x0550, // D id, W duration
		GAMSG_CHANGE_PLAYER_LEVEL:0x0555, // D id, W level
		GAMSG_CHANGE_ACCOUNT_LEVEL:0x0556, // D id, W level
		GAMSG_STATISTICS:0x0560, // { W map id, W thing nb, W monster nb, W player nb, { D character id }* }*
		CGMSG_CHANGED_PARTY:0x0590, // D character id, D party id
		GCMSG_REQUEST_POST:0x05A0, // D character id
		CGMSG_POST_RESPONSE:0x05A1, // D receiver id, { S sender name, S letter, W num attachments { W attachment item id, W quantity } }
		GCMSG_STORE_POST:0x05A5, // D sender id, S receiver name, S letter, { W attachment item id, W quantity }
		CGMSG_STORE_POST_RESPONSE:0x05A6, // D id, B error
		GAMSG_TRANSACTION:0x0600, // D character id, D action, S message
		GAMSG_CREATE_ITEM_ON_MAP:0x0601, // D map id, D item id, W amount, W pos x, W pos y
		GAMSG_REMOVE_ITEM_ON_MAP:0x0602, // D map id, D item id, W amount, W pos x, W pos y

		XXMSG_INVALID:0x7FFF,


		//Packets from client to server
		CMSG_SERVER_VERSION_REQUEST:0x7530,

		CMSG_CHAR_PASSWORD_CHANGE:0x0061, /**< Custom change password packet */
		CMSG_CHAR_SERVER_CONNECT:0x0065,
		CMSG_CHAR_SELECT:0x0066,
		CMSG_CHAR_CREATE:0x0067,
		CMSG_CHAR_DELETE:0x0068,

		CMSG_MAP_SERVER_CONNECT:0x0072,
		CMSG_MAP_LOADED:0x007d,
		CMSG_CLIENT_QUIT:0x018A,

		CMSG_CHAT_MESSAGE:0x008c,
		CMSG_CHAT_WHISPER:0x0096,

		CMSG_SKILL_LEVELUP_REQUEST:0x0112,
		CMSG_STAT_UPDATE_REQUEST:0x00bb,
		CMSG_SKILL_USE_BEING:0x0113,
		CMSG_SKILL_USE_POSITION:0x0116,
		// Variant of 0x116 with 80 char string at end (unsure of use)
		CMSG_SKILL_USE_POSITION_MORE:0x0190,
		CMSG_SKILL_USE_MAP:0x011b,

		CMSG_PLAYER_INVENTORY_USE:0x00a7,
		CMSG_PLAYER_INVENTORY_DROP:0x00a2,
		CMSG_PLAYER_EQUIP:0x00a9,
		CMSG_PLAYER_UNEQUIP:0x00ab,

		CMSG_ITEM_PICKUP:0x009f,
		CMSG_PLAYER_CHANGE_DIR:0x009b,
		CMSG_PLAYER_CHANGE_DEST:0x0085,
		CMSG_PLAYER_CHANGE_ACT:0x0089, // same as CMSG_PLAYER_ATTACK
		CMSG_PLAYER_RESTART:0x00b2,
		CMSG_PLAYER_EMOTE:0x00bf,
		CMSG_PLAYER_ATTACK:0x0089, // same as CMSG_PLAYER_CHANGE_ACT
		CMSG_WHO_REQUEST:0x00c1,

		CMSG_NPC_TALK:0x0090,
		CMSG_NPC_NEXT_REQUEST:0x00b9,
		CMSG_NPC_CLOSE:0x0146,
		CMSG_NPC_LIST_CHOICE:0x00b8,
		CMSG_NPC_INT_RESPONSE:0x0143,
		CMSG_NPC_STR_RESPONSE:0x01d5,
		CMSG_NPC_BUY_SELL_REQUEST:0x00c5,
		CMSG_NPC_BUY_REQUEST:0x00c8,
		CMSG_NPC_SELL_REQUEST:0x00c9,

		CMSG_TRADE_REQUEST:0x00e4,
		CMSG_TRADE_RESPONSE:0x00e6,
		CMSG_TRADE_ITEM_ADD_REQUEST:0x00e8,
		CMSG_TRADE_CANCEL_REQUEST:0x00ed,
		CMSG_TRADE_ADD_COMPLETE:0x00eb,
		CMSG_TRADE_OK:0x00ef,

		CMSG_PARTY_CREATE:0x00f9,
		CMSG_PARTY_INVITE:0x00fc,
		CMSG_PARTY_INVITED:0x00ff,
		CMSG_PARTY_LEAVE:0x0100,
		CMSG_PARTY_SETTINGS:0x0102,
		CMSG_PARTY_KICK:0x0103,
		CMSG_PARTY_MESSAGE:0x0108,

		CMSG_MOVE_TO_STORAGE:0x00f3, /** Move item to storage */
		CMSG_MOVE_FROM_STORAGE:0x00f5, /** Remove item from storage */
		CMSG_CLOSE_STORAGE:0x00f7, /** Request storage close */

		CMSG_ADMIN_ANNOUNCE:0x0099,
		CMSG_ADMIN_LOCAL_ANNOUNCE:0x019C,
		CMSG_ADMIN_HIDE:0x019D,
		CMSG_ADMIN_KICK:0x00CC,
		CMSG_ADMIN_MUTE:0x0149,

		CMSG_GUILD_CHECK_MASTER:0x014d,
		CMSG_GUILD_REQUEST_INFO:0x014f,
		CMSG_GUILD_REQUEST_EMBLEM:0x0151,
		CMSG_GUILD_CHANGE_EMBLEM:0x0153,
		CMSG_GUILD_CHANGE_MEMBER_POS:0x0155,
		CMSG_GUILD_LEAVE:0x0159,
		CMSG_GUILD_EXPULSION:0x015b,
		CMSG_GUILD_BREAK:0x015d,
		CMSG_GUILD_CHANGE_POS_INFO:0x0161,
		CMSG_GUILD_CREATE:0x0165,
		CMSG_GUILD_INVITE:0x0168,
		CMSG_GUILD_INVITE_REPLY:0x016b,
		CMSG_GUILD_CHANGE_NOTICE:0x016e,
		CMSG_GUILD_ALLIANCE_REQUEST:0x0170,
		CMSG_GUILD_ALLIANCE_REPLY:0x0172,
		CMSG_GUILD_MESSAGE:0x017e,
		CMSG_GUILD_OPPOSITION:0x0180,
		CMSG_GUILD_ALLIANCE_DELETE:0x0183
}