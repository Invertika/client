/*!
 *
 *   Invertika.js - Web-based 2D MMORPG
 *   http://invertika.org
 *
 */

// Variablen
var ip = "127.0.0.1";
var accountServer;

//jsApp
var jsApp = {
    onload: function() {
        if (!me.video.init('jsapp', 640, 384, false, 1.0))
        {
            alert("Sorry but your browser does not support html 5 canvas.");
            return;
        }
		
		var dialog = new DialogObject(10, 10, dialog, dialog[0].background.width - OFFSET_SIZE_TEXT_X, dialog[0].background.width - OFFSET_SIZE_TEXT_Y, OFFSET_DIALOG_X, OFFSET_DIALOG_Y, new me.Font("acmesa",20,"#880D0D", "center"), "enter", activateControls);
		dialog.draw();
		
		var X=50;
		var Y=50;
		var dialog="Hallo Welt";
		var widthText=10;
		var heightText=10;
		var offsetTextX=10;
		var offsetTextY=10;
		var font=new me.Font("Verdana", 14, "white");
		var tagKey;
		var callback;
		var offsetAvatarX=10;
		var offsetAvatarY=10;
		
		dialog = new DialogObject(X, Y, dialog, widthText, heightText,
		offsetTextX, offsetTextY, font, tagKey, callback, offsetAvatarX,
		offsetAvatarY); 
		
		dialog.draw();
	}
	
}

//Debug
function debug()
{
	accountServer=new AccountServerConnection(ip, 9601);
	//accountServer.login("florian", "geheim");
	
	//login("seeseekey", "geheim");
	//login("schnee", "geheim");
	//login("florian", "geheim");

	//register("florian", "geheim", "sees.eekey@gmail.com", "IGNORE");
}
