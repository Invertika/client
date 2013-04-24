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

function viewport() //TODO Durch empfohlende melonJS FAQ Variante ersetzen
{
	var e = window, a = 'inner';
	
	if ( !( 'innerWidth' in window ) )
	{
		a = 'client';
		e = document.documentElement || document.body;
	}
	
	return { width : e[ a+'Width' ] , height : e[ a+'Height' ] }
}

//Application jsApp
var jsApp = {
	playScreen:null,
	resourcesCount:0,
	resourcesIndex:0,
	currentyMap:"",
	
    onload: function() {
		log.debug( 'Enter jsApp.onload()');
		
        //if (!me.video.init('jsapp', viewport()["width"], viewport()["height"], false, 'auto', false))
		if (!me.video.init('jsapp', 720, 450, false, 'auto', false))
        {
            alert("Sorry but your browser does not support html 5 canvas.");
            return;
        }

		// begrenze auf 30 FPS
		me.sys.fps = 30;
		
        // initialize the "audio"
        me.audio.init("mp3,ogg");

		//plugins registrieren
		me.plugin.register(debugPanel, "debug");
		
        // load everything & display a loading screen
        me.state.change(me.state.LOADING);
		
		log.debug( 'Leave jsApp.onload()');
	},
	
    loaded: function() {
		log.debug('Enter jsApp.loaded()');

        // add our player entity in the entity pool
        //me.entityPool.add("mainPlayer", PlayerEntity);

        // enable the keyboard
        me.input.bindKey(me.input.KEY.LEFT,  "left");
        me.input.bindKey(me.input.KEY.RIGHT, "right");
        me.input.bindKey(me.input.KEY.UP,    "up");
        me.input.bindKey(me.input.KEY.DOWN,  "down");

        // start the game
        me.state.change(me.state.PLAY);
		
		log.debug('Leave jsApp.loaded()');
    },
	
    mapLoaded: function() {
		var mapTmx=me.loader.getTMX(jsApp.currentyMap);
		var list=getUsedResources(mapTmx);
		
		jsApp.resourcesIndex=0;
		jsApp.resourcesCount=list.length;
		
		jsApp.resourceLoaded();
    },
	
	loadMap: function(mapname)
	{
		var mapFilename=MapsPath+mapname+".tmx";
		jsApp.currentyMap=mapname;
		me.loader.load({name: mapname,  type:"tmx",  src: mapFilename}, this.mapLoaded, this.error);
		//me.loader.load({name: "desert",  type:"tmx",  src: MapsPath+"desert.tmx"}, this.mapLoaded, this.error);
	},

	resourceLoaded: function()
	{
		if(jsApp.resourcesIndex==jsApp.resourcesCount)
		{
			jsApp.initMap();
			return;
		}
		
		var mapTmx=me.loader.getTMX(jsApp.currentyMap);
		var list=getUsedResources(mapTmx);
		
		var val=list[jsApp.resourcesIndex];
		me.loader.load({name: me.utils.getBasename(val),  type:"image",  src: MapsPath+val}, jsApp.resourceLoaded, jsApp.error);
		
		jsApp.resourcesIndex++;
	},
	
	initMap: function()
	{
        me.levelDirector.loadLevel(jsApp.currentyMap);

		if(this.playScreen==null)
		{
			this.playScreen=new PlayScreen();
		}
		
		this.playScreen.setLevel(jsApp.currentyMap);
	},
	
	error: function()
	{
		log.error('Error on loading data.');
	}
}

/* the in game stuff*/
var PlayScreen = me.ScreenObject.extend({

	levelName: "",

	setLevel: function(name)
	{
		this.levelName=name;
	},
	
	loadLevel: function()
	{
        // stuff to reset on state change
        //me.levelDirector.loadLevel(this.levelName);	
	},

    onResetEvent: function() {		
        // stuff to reset on state change
        //me.levelDirector.loadLevel("desert");	
		me.levelDirector.loadLevel(this.levelName);		
    },

    onDestroyEvent: function() {
    }

});

//Uki //TODO Replace with melonJS GUI Objects?

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
).attachTo( $('gui'), '1000 300' );

// Bind alert to all buttons
uki('Button').bind('click', function() {
    //alert(uki('TextField').attr('value'));
});

// Make label work as labels in browser
uki('Label').click(function() {
   if (this.prevView().checked) this.prevView().checked(!this.prevView().checked()).focus();
});

//Functions and Callbacks
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
	$('gui').hide('slow'); //GUI erst einmal ausblenden
}

function onGameServerMapChange(mapName, posX, posY)
{	
	log.debug("Event onGameServerMapChange called");
	jsApp.loadMap(mapName); //map laden
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