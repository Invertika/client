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
			//case Protocol.APMSG_CHAR_INFO:
			//{
			//	log.warn(sprintf("Unimplemented command: %s", "APMSG_CHAR_INFO"));
			//	break;
			//}
			default:
			{
				log.warn(sprintf("Unknown message from game server: %s", getEnumFromInteger(Protocol, Protocol, responseMessage.id)));
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