/*!
 *
 *   Invertika.js - Web-based 2D MMORPG
 *   http://invertika.org
 *
 */

var ip = "127.0.0.1";
var accountServerConnectionString=sprintf("ws://%s:9601", ip);

var accountServer;

var PROTOCOL_VERSION=1;

function init(username, password)
{
	accountServer = new WebSocket(accountServerConnectionString);
	
	//Login Kommando zusammenbauen
	var loginMsg=new Message(Protocol.PAMSG_LOGIN);
	loginMsg.addValue(PROTOCOL_VERSION); //Client Version
	loginMsg.addValue(username);
	loginMsg.addValue(password);
	loginMsg.addValue(3);
	
	// when the connection is established, this method is called
	accountServer.onopen = function () {
		accountServer.send(loginMsg.getString());
	};
	
	// when data is comming from the server, this metod is called
	accountServer.onmessage = function (message) {
		alert(message.data);	
	};

    //Testlogin
    //MessageOut outMsg(0x0064);
    //outMsg.writeInt32(0); // client version
    //outMsg.writeString(username, 24);
    //outMsg.writeString(password, 24);

    /*
     * eAthena calls the last byte "client version 2", but it isn't used at
     * at all. We're retasking it, as a bit mask:
     *  0 - can handle the 0x63 "update host" packet
     *  1 - defaults to the first char-server (instead of the last)
     */
    //outMsg.writeInt8(0x03);
}

function registerAccount(username, password, email, captchaResponse)
{
	//Register Kommando zusammenbauen
	var registerMsg=new Message(Protocol.PAMSG_REGISTER);
	registerMsg.addValue(PROTOCOL_VERSION); //Client Version
	registerMsg.addValue(username);
	registerMsg.addValue(sha256_digest(username + password)); // Use a hashed password for privacy reasons
	registerMsg.addValue(email);
	registerMsg.addValue(captchaResponse);

	// when the connection is established, this method is called
	accountServer.onopen = function () {
		accountServer.send(registerMsg.getString());
	};
	
	// when data is comming from the server, this metod is called
	accountServer.onmessage = function (message) {
		alert(message.data);	
	};
}
