/*!
 *
 *   Invertika.js - Web-based 2D MMORPG
 *   http://invertika.org
 *
 */

// Variablen
var ip = "127.0.0.1";
var accountServer;
var gameServer;

function viewport()
{
	var e = window, a = 'inner';
	
	if ( !( 'innerWidth' in window ) )
	{
		a = 'client';
		e = document.documentElement || document.body;
	}
	
	return { width : e[ a+'Width' ] , height : e[ a+'Height' ] }
}

//jsApp
var jsApp = {
    onload: function() {
		//
        //if (!me.video.init('jsapp', viewport()["width"], viewport()["height"], false, 'auto', false))
		if (!me.video.init('jsapp', 720, 450, false, 'auto', false))
        {
            alert("Sorry but your browser does not support html 5 canvas.");
            return;
        }

		//make nice things
		// begrenze auf 30 FPS
		me.sys.fps = 30;
		
        // initialize the "audio"
        me.audio.init("mp3,ogg");

        // set all resources to be loaded
        me.loader.onload = this.loaded.bind(this);

        // set all resources to be loaded
        me.loader.preload(g_resources);

        // load everything & display a loading screen
        me.state.change(me.state.LOADING);
		
		log.debug( 'leave onload (jsApp)');
	},
	
    loaded: function() {
        // set the "Play/Ingame" Screen Object
        me.state.set(me.state.PLAY, new PlayScreen());

        // add our player entity in the entity pool
        //me.entityPool.add("mainPlayer", PlayerEntity);

        // enable the keyboard
        me.input.bindKey(me.input.KEY.LEFT,  "left");
        me.input.bindKey(me.input.KEY.RIGHT, "right");
        me.input.bindKey(me.input.KEY.UP,    "up");
        me.input.bindKey(me.input.KEY.DOWN,  "down");

        //me.debug.renderHitBox = true;

        // start the game
        me.state.change(me.state.PLAY);
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
    //alert(uki('TextField').attr('value'));
});

// Make label work as labels in browser
uki('Label').click(function() {
   if (this.prevView().checked) this.prevView().checked(!this.prevView().checked()).focus();
});

//Debug
var g_resources= [
    { name: "desert1",          type: "image", src: "data/desert1.png" },
    { name: "desert",           type: "tmx",   src: "data/desert.tmx" },
    { name: "player_male_base", type: "image", src: "data/player_male_base.png" },
    { name: "fog",              type: "image", src: "data/fog.png" }
];

/* the in game stuff*/
var PlayScreen = me.ScreenObject.extend({

    onResetEvent: function() {
        // stuff to reset on state change
        me.levelDirector.loadLevel("desert");
    },

    onDestroyEvent: function() {
    }

});

function debug()
{	
	//AccountServer initialisieren
	accountServer=new AccountServerConnection(ip, 9601);
	
    accountServer.addEvent('charSelectionNeeded', onCharSelectionNeeded);
	accountServer.addEvent('readyForGameAndChatServerConnect', onReadyForGameAndChatServerConnect)

	accountServer.login("seeseekey", "geheim");
}

function onReadyForGameAndChatServerConnect(netToken, gameAdress, gamePort, chatAdress, chatPort)
{	
	gameServer=new GameServerConnection(gameAdress, gamePort);
	
	gameServer.addEvent('gameServerLoginComplete', onGameServerLoginComplete);
	gameServer.addEvent('gameServerMapChange', onGameServerMapChange);
	
	gameServer.login(netToken);
}

function onGameServerLoginComplete()
{	
	log.debug("Event onGameServerLoginComplete called");
	jsApp.onload(); //melonJS aktivieren
}

function onGameServerMapChange(mapName, posX, posY)
{	
	log.debug("Event onGameServerMapChange called");
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