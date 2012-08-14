/*!
 *
 *   Invertika.js - Web-based 2D MMORPG
 *   http://invertika.org
 *
 */

var ip = "127.0.0.1";
var accountServerConnectionString=sprintf("ws://%s:9601", ip);

var accountServer;

//         // when data is comming from the server, this metod is called
//         accountServer.onmessage = function (evt) {
//			alert('message');	
//         };
//
//         // when the connection is established, this method is called
//         accountServer.onopen = function () {
//			alert('connection open');
//			// var login="0010:seeseekey:geheim:3";
//			// accountServer.send(login);
//         };
//
//         // when the connection is closed, this method is called
//         accountServer.onclose = function () {
//			alert('connection closed');
//         }

function init(username, password)
{
	accountServer = new WebSocket(accountServerConnectionString);
	
	//Login Kommando zusammenbauen
	var loginMsg=new Message(Protocol.PAMSG_LOGIN);
	loginMsg.addValue(0); //Client Version
	loginMsg.addValue(username);
	loginMsg.addValue(password);
	loginMsg.addValue(3);
	
	// when the connection is established, this method is called
	accountServer.onopen = function () {
		accountServer.send(loginMsg.getString());
	};
	
	// when data is comming from the server, this metod is called
	accountServer.onmessage = function (evt) {
		alert(evt);	
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