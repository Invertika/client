var AccountServerConnection = new Class({
	Implements: [Events],
	
	connectionString: "",
	ip: "127.0.0.1",
	socket: null,
	username: "",
	password: "",
	
	//Konstruktor
	initialize: function(ip, port) {
        this.ip = ip;
		
        if(port) this.port=port;
        else this.port=9601;
		
        this.connectionString=sprintf("ws://%s:%s", ip, port)
    },
		
	//MessageHandler
	onMessage: function(message) 
	{
		var responseMessage=new MessageIn(message.data);
		log.debug(sprintf("Recieve message: %s", getEnumFromInteger(Protocol, responseMessage.id)));

		switch(responseMessage.id)
		{
			case Protocol.APMSG_REGISTER_RESPONSE:
			{
				var registerReturnCode=responseMessage.getInt8();
				alert("Return code from register is: " + registerReturnCode);
				
				break;
			}
			case Protocol.APMSG_LOGIN_RNDTRGR_RESPONSE: //Login Response
			{
				var token=responseMessage.getString();
			
				//Login Kommando zusammenbauen
				var msg=new MessageOut(Protocol.PAMSG_LOGIN);
				msg.addValueAsInt32(PROTOCOL_VERSION); //Client Version
				msg.addValueAsString(window.accountServer.username);
				msg.addValueAsString(sha256_digest(sha256_digest(sha256_digest(window.accountServer.username + window.accountServer.password))+token));
							
				msg.send(this);
			
				break;
			}
			case Protocol.APMSG_LOGIN_RESPONSE:
			{
				var errMsg=responseMessage.getInt8();
			
				if(errMsg==ErrorMessage.ERRMSG_OK) //alles okay
				{
					var updateHost = responseMessage.getString();
					var clientDataUrl = responseMessage.getString();
				
					var charSlots = responseMessage.getInt8();
					
					//Event feuern damit der Client entscheiden kann
					window.accountServer.fireEvent('charSelectionNeeded');
				}
				else
				{
					//Login fehlgeschlagen
				}

				break;
			}
			case Protocol.APMSG_CHAR_SELECT_RESPONSE:
			{
				var errMsg=responseMessage.getInt8();
			
				if(errMsg==ErrorMessage.ERRMSG_OK) //alles okay
				{
					var netToken=responseMessage.getString();
					var gameAdress=responseMessage.getString();
					var gamePort=responseMessage.getInt16(2);
					var chatAdress=responseMessage.getString();
					var chatPort=responseMessage.getInt16();
				
					//connectGameServer=true;
					//connectChatServer=true;
				}
				else
				{
					//Problem aufgetreten
				}
			
				break;
			}
			case Protocol.APMSG_CHAR_INFO:
			{
				log.warn(sprintf("Unimplemented command: %s", "APMSG_CHAR_INFO"));
				break;
			}
			default:
			{
				log.warn(sprintf("Unknown message: %s", getEnumFromInteger(Protocol, responseMessage.id)));
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
		loginMsg.addValueAsString(username);
	
		this.username=username;
		this.password=password;

		// when the connection is established, this method is called		
		this.socket.onopen = function () {
			//this ist in diesem Block das Websocket selbst
			loginMsg.send(this);
		};
	
		// when data is comming from the server, this method is called
		this.socket.onmessage = this.onMessage;
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
		this.socket.onopen = function () {
			//this ist in diesem Block das Websocket selbst
			registerMsg.send(this);
		};
	
		// when data is comming from the server, this metod is called
		this.socket.onmessage = this.onMessage;
	},
	
	createCharacter: function(name, slot, gender, hairStyle, hairColor, stats)
	{
		//Kommando zusammenbauen (PAMSG_CHAR_CREATE)
		var msg=new MessageOut(Protocol.PAMSG_CHAR_CREATE);
		msg.addValueAsString(name);
		msg.addValueAsInt8(hairStyle);
		msg.addValueAsInt8(hairColor);
		msg.addValueAsInt8(gender);
		msg.addValueAsInt8(slot);
		
		//Charakterwerte in Nachricht schreiben
		stats.each(function(statValue){
		  msg.addValueAsInt16(statValue);
		});

		//Senden
		msg.send(this.socket);
	},
	
	/// mit dieser Funktion wird der Charakter ausgewählt
	selectCharacter: function(charNumber)
	{				
		var msg=new MessageOut(Protocol.PAMSG_CHAR_SELECT);
		msg.addValueAsInt8(charNumber);
				
		msg.send(this.socket);
	}
});