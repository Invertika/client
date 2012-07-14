/*!
 *
 *   Invertika.js - Web-based 2D MMORPG
 *   http://invertika.org
 *
 */

function init(username, password)
{
    //alert("Yeah.")
    
    //Websocketverbindung aufbauen
    var accountserver = new WebSocket("ws://127.0.0.1:9601");
    
    //mNetwork->connect(mServer);
    //MessageOut outMsg(CMSG_SERVER_VERSION_REQUEST);
    
    var msg = new MessageOut(0x7530); // CMSG_SERVER_VERSION_REQUEST
    //console.log(msg.getData().byteLength);
    accountserver.send(msg.getData());
}
//
////var accountserver = new WebSocket("ws://127.0.0.1:5328/echo");
//var accountserver = new WebSocket("ws://echo.websocket.org/");
////var accountserver = new WebSocket("ws://127.0.0.1:7681/");
//accountserver.binaryType = "arraybuffer";
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