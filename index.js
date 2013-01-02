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

		//make nice things
	}
}

//Uki

// controlls description
function views() {
    return [
        { view: 'TextField',rect: '400 10 100 24', anchors: 'right top', value: "", placeholder: 'Username' },
        { view: 'PasswordTextField',rect: '510 10 100 24', anchors: 'right top', value: "", placeholder: 'Password' },
        { view: 'Button', rect: '620 10 200 24', anchors: 'right top', text: 'Login'},
    ];
}

// page layout
uki(
    { view: 'Box', rect: '0 200 1000 400', minSize: '980 0', anchors: 'top left right width', childViews: [
        { view: 'Box', background: 'theme(panel)', rect: '0 0 1000 50', // controlls on panel background
            anchors: 'top left right width', childViews: views() },
    ]}
).attachTo( document.getElementById('gui'), '1000 300' );

// Bind alert to all buttons
uki('Button').bind('click', function() {
	//accountServer.login("seeseekey", "geheim");
    //alert(uki('TextField').attr('value'));
});

// Make label work as labels in browser
uki('Label').click(function() {
   if (this.prevView().checked) this.prevView().checked(!this.prevView().checked()).focus();
});

//Debug
function debug()
{
	accountServer=new AccountServerConnection(ip, 9601);
    accountServer.addEvent('charSelectionNeeded', onCharSelectionNeeded);
	accountServer.login("seeseekey", "geheim");
}

function onCharSelectionNeeded(object)
{	
	var button=uki({
	    view: 'Button',
	    rect: '200 40 200 80',
	    text: 'Character One'
	}).attachTo( document.getElementById('gui'), '600 -500' );
	
	button.bind('click', function() {
    	//alert("Character One is selected.");
		accountServer.selectCharacter(1);
		
		//Erzeuge Character
		//var stats = [5, 5, 6];
		//accountServer.createCharacter("Arbiter 902", 1, 0, 0, 0, stats);
	});
}

