/*!
 *
 *   Invertika.js - Web-based 2D MMORPG
 *   http://invertika.org
 *
 */

function init(username, password)
{
	//alert("Testverbindung CK")
	
    //Websocketverbindung aufbauen
    var accountserver = new WebSocket("ws://127.0.0.1:9601");
	
	//Testlogin
	var login="0010:seeseekey:geheim:3";
	accountserver.send(login);

	return;
	
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

//accountserver.onopen = function(evt) {
//    console.log("Connection made!");
//    //var dataBuffer = new ArrayBuffer(4);
//    //var dataView = new DataView(dataBuffer);
//    //dataView.setInt32(0, 0x1234ABCD);
//    var msg = new MessageOut(0x0010); // PAMSG_LOGIN
//    console.log(msg.getData().byteLength);
//    accountserver.send(msg.getData());
//};

//accountserver.onmessage = function(evt) {
//    console.log( "Received Message: "  +  evt.data);
//    console.log(new DataView(evt.data).getUint16(0));
//};