var AccountServerConnection = new Class({
	connectionString: "",
	ip: "127.0.0.1",
	socket,
	username: "",
	passwort,
	
	//Konstruktor
	initialize: function(ip) {
        this.ip = ip;
		this.connectionString=sprintf("ws://%s:9601", ip)
    },
		
	//MessageHandler
	onMessage: function(message) 
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
				msg.addValue(sha256_digest(sha256_digest(sha256_digest(username + password))+token));
							
				accountServer.send(msg.getString());
			
				break;
			}
			case Protocol.APMSG_LOGIN_RESPONSE:
			{
				var errMsg=responseMessage.getPart(0);
			
				if(errMsg==ErrorMessage.ERRMSG_OK) //alles okay
				{
					var updateHost = responseMessage.getPart(1);
					var clientDataUrl = responseMessage.getPart(2);
				
					var charSlots = parseInt(responseMessage.getPart(3));

					//chooseCharacter();
					var charNumber=1;
				
					var msg=new MessageOut(Protocol.PAMSG_CHAR_SELECT);
					msg.addValue(charNumber);
				
					this.socket.send(msg.getString());
				}
				else
				{
					//Login fehlgeschlagen
				}

				break;
			}
			case Protocol.APMSG_CHAR_SELECT_RESPONSE:
			{
				var errMsg=responseMessage.getPart(0);
			
				if(errMsg==ErrorMessage.ERRMSG_OK) //alles okay
				{
					var netToken = responseMessage.getPart(1);
					var gameAdress = responseMessage.getPart(2);
					var gamePort = parseInt(responseMessage.getPart(2));
					var chatAdress = responseMessage.getPart(3);
					var chatPort = parseInt(responseMessage.getPart(4));
				
					//connectGameServer=true;
					//connectChatServer=true;
				}
				else
				{
					//Problem aufgetreten
				}
			
				break;
			}
			default:
			{
				alert("Unbekannte Nachricht: " + message.data);
				break;
			}
		}
	},
		
	//Methoden
	login: function(username, password)
	{
		this.socket = new WebSocket(this.connectionString);
	
		//Login Paket zusammenbauen
		var loginMsg=new MessageOut(Protocol.PAMSG_LOGIN_RNDTRGR);
		loginMsg.addValue(username);
	
		this.username=username;
		this.password=password;

		// when the connection is established, this method is called
		accountServer.onopen = function () {
			this.socket.send(loginMsg.getString());
		};
	
		// when data is comming from the server, this metod is called
		this.socket.onmessage = onMessage;
	},
	
	register: function(username, password, email, captchaResponse)
	{	
		this.socket = new WebSocket(this.connectionString);

		//Register Kommando zusammenbauen
		var registerMsg=new MessageOut(Protocol.PAMSG_REGISTER);
		registerMsg.addValue(PROTOCOL_VERSION); //Client Version
		registerMsg.addValue(username);
		registerMsg.addValue(sha256_digest(username + password)); // Use a hashed password for privacy reasons
		registerMsg.addValue(email);
		registerMsg.addValue(captchaResponse);
	
		// when the connection is established, this method is called
		accountServer.onopen = function () {
			this.socket.send(registerMsg.getString());
		};
	
		// when data is comming from the server, this metod is called
		this.socket.onmessage = onMessage;
	}
});


