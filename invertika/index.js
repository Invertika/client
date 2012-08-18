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
			case Protocol.APMSG_LOGIN_RNDTRGR_RESPONSE:
			{
				var token=responseMessage.getPart(0);
				
				//Login Kommando zusammenbauen
				var msg=new MessageOut(Protocol.PAMSG_LOGIN);
				msg.addValue(PROTOCOL_VERSION); //Client Version
				msg.addValue(username);
				msg.addValue(sha256_digest(sha256_digest(sha256_digest(username + password)) + token));
								
				accountServer.send(msg.getString());
				
				break;
			}
			default:
			{
				alert("Unbekannte Nachricht: " + message.data);
				break;
			}
		}
	};
}

function login(username, password)
{	
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
