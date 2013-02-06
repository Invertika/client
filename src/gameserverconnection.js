var GameServerConnection = new Class({
	Implements: [Events],
	
	connectionString: "",
	ip: "127.0.0.1",
	socket: null,
	username: "",
	password: "",
	connected: false,
	
	//Konstruktor
	initialize: function(ip, port) {
        this.ip = ip;
		
        if(port) this.port=port;
        else this.port=9604;
		
        this.connectionString=sprintf("ws://%s:%s", ip, port)
    },
		
	//MessageHandler
	onMessage: function(message) 
	{
		var responseMessage=new MessageIn(message.data);
		log.debug(sprintf("Recieve message from game server: %s", getEnumFromInteger(Protocol, responseMessage.id)));

		switch(responseMessage.id)
		{
			case Protocol.GPMSG_CONNECT_RESPONSE:
			{
				var errMsg=responseMessage.getInt8();
			
				if(errMsg==ErrorMessage.ERRMSG_OK) //alles okay
				{
					log.debug("Login on game server successful");
					window.gameServer.fireEvent('gameServerLoginComplete');
					
					this.connected=true;
					//TODO Action
				}
				else
				{
					//TODO entsprechene Fehlermeldung ausgeben
					log.warn(sprintf("Errorcode from game server (GPMSG_CONNECT_RESPONSE): %s", getEnumFromInteger(ErrorMessage, errMsg)));
				}
				
				break;
			}
			case Protocol.GPMSG_PLAYER_MAP_CHANGE:
			{
				var mapName=responseMessage.getString();
				var posX=responseMessage.getInt16();
				var posY=responseMessage.getInt16();
			
				//Event feuern damit der Client entscheiden kann
				window.gameServer.fireEvent('gameServerMapChange', [mapName, posX, posY]);
			
				log.debug(sprintf("GPMSG_PLAYER_MAP_CHANGE recieved from game server with map name %s and position %s, %s", mapName, posX, posY));
			
				break;
			}
			default:
			{
				log.warn(sprintf("Unknown message from game server: %s", getEnumFromInteger(Protocol, responseMessage.id)));
				break;
			}
		}
	},
		
	//Methoden
	login: function(token)
	{
		this.socket = new WebSocket(this.connectionString);
		this.socket.binaryType = 'arraybuffer';
	
		//Login Paket zusammenbauen
		var loginMsg=new MessageOut(Protocol.PGMSG_CONNECT);
		loginMsg.addValueAsString(token);

		// when the connection is established, this method is called		
		this.socket.onopen = function () {
			//this ist in diesem Block das Websocket selbst
			loginMsg.send(this);
		};
	
		// when data is comming from the server, this method is called
		this.socket.onmessage = this.onMessage;
	}
});