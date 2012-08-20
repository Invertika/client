/*!
 *
 *   Invertika.js - Web-based 2D MMORPG
 *   http://invertika.org
 *
 */

// Variablen
var ip = "127.0.0.1";
var accountServerConnectionString=sprintf("ws://%s:9601", ip);

var accountServer;
var PROTOCOL_VERSION=1;

var _username;
var _password;

//Debug
function debug()
{
	//login("seeseekey", "geheim");
	login("schnee", "geheim");
	//register("schnee", "geheim", "se.eseekey@gmail.com", "IGNORE");
}

// Message Handler
function onMessage(message) 
{
	var responseMessage=new MessageIn(message.data);

	switch(responseMessage.id)
	{
		case Protocol.APMSG_REGISTER_RESPONSE:
		{
			var registerReturnCode=responseMessage.getPart(0);
			alert("Return code from register is: " + registerReturnCode);
			break;
		}
		case Protocol.APMSG_LOGIN_RNDTRGR_RESPONSE: //Login Response
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
}

// Funktionen
function login(username, password)
{
	accountServer = new WebSocket(accountServerConnectionString);
	
	//Login Paket zusammenbauen
	var loginMsg=new MessageOut(Protocol.PAMSG_LOGIN_RNDTRGR);
	loginMsg.addValue(username);
	
	this.username=username;
	this.password=password;

	// when the connection is established, this method is called
	accountServer.onopen = function () {
		accountServer.send(loginMsg.getString());
	};
	
	// when data is comming from the server, this metod is called
	accountServer.onmessage = onMessage;
}

function register(username, password, email, captchaResponse)
{	
	accountServer = new WebSocket(accountServerConnectionString);

	//Register Kommando zusammenbauen
	var registerMsg=new MessageOut(Protocol.PAMSG_REGISTER);
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
	accountServer.onmessage = onMessage;
}
