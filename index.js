/*!
 *
 *   Invertika.js - Web-based 2D MMORPG
 *   http://invertika.org
 *
 */

// Variablen
var ip = "127.0.0.1";

var accountServer;

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
