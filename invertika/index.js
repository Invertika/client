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

function debug()
{
	connect("seeseekey", "geheim");
	//registerAccount("seeseekey", "geheim", "seeseekey@gmail.com", "IGNORE");
	//login("seeseekey", "geheim");
}

function connect(username, password)
{
	accountServer = new WebSocket(accountServerConnectionString);
	
	// when the connection is established, this method is called
	accountServer.onopen = function () {
		login("seeseekey", "geheim");
	};
	
	// when data is comming from the server, this metod is called
	accountServer.onmessage = function (message) {
		var responseMessage=new MessageIn(message.data);
	
		switch(responseMessage.id)
		{
			case Protocol.APMSG_LOGIN_RESPONSE:
			{
				break;
			}
			default:
			{
				break;
			}
		}
	
		//Debug
		alert(message.data);
		alert(responseMessage.getPart(0));
		alert(responseMessage.getPart(1));
		alert(responseMessage.parts.length);
	};
}

function login(username, password)
{
	//Login Kommando zusammenbauen
	// var loginMsg=new MessageOut(Protocol.PAMSG_LOGIN);
	// loginMsg.addValue(PROTOCOL_VERSION); //Client Version
	// loginMsg.addValue(username);
	// loginMsg.addValue(password);
	// loginMsg.addValue(3);
	
	var loginMsg=new MessageOut(Protocol.PAMSG_LOGIN_RNDTRGR);
	loginMsg.addValue(username);
	
	accountServer.send(loginMsg.getString());
}

function registerAccount(username, password, email, captchaResponse)
{
	accountServer = new WebSocket(accountServerConnectionString);
	
	//Register Kommando zusammenbauen
	var registerMsg=new MessageOut(Protocol.PAMSG_REGISTER);
	registerMsg.addValue(PROTOCOL_VERSION); //Client Version
	registerMsg.addValue(username);
	registerMsg.addValue(sha256_digest(username + password)); // Use a hashed password for privacy reasons
	registerMsg.addValue(email);
	registerMsg.addValue(captchaResponse);
	
	accountServer.send(registerMsg.getString());
}
