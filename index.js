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

/*------------------- 
a player entity
-------------------------------- */
me.game.PlayerEntity = me.ObjectEntity.extend({

    /* -----

    constructor

    ------ */

    init: function(x, y, settings) {		 
		// define this here instead of tiled
		//settings.image = "player_male_base";
		//settings.image = "player_male_base";
		//settings.image = "player_male_base";
		
		settings.image = "player_male_base";
		settings.spritewidth = 64;
		settings.spriteheight = 64;
		
        // call the constructor
        //this.parent(x, y, settings);
		this.parent(x, y, settings); //Debug

		// set the default horizontal & vertical speed (accel vector)
        //this.setVelocity(3, 15);
		this.setVelocity(0, 0);

        // set the display to follow our position on both axis
        me.game.viewport.follow(this.pos, me.game.viewport.AXIS.BOTH);

    },

    /* -----

    update the player pos

    ------ */
    update: function() {
        if (me.input.isKeyPressed('left')) {
			//alert("left OHA");
            // flip the sprite on horizontal axis
			this.flipX(true);
			// update the entity velocity
			this.vel.x -= this.accel.x * me.timer.tick;
        } else if (me.input.isKeyPressed('right')) {
            // unflip the sprite
			this.flipX(false);
			// update the entity velocity
			this.vel.x += this.accel.x * me.timer.tick;
        } else {
            this.vel.x = 0;
        }
        if (me.input.isKeyPressed('jump')) {
			// make sure we are not already jumping or falling
            if (!this.jumping && !this.falling) {
				// set current vel to the maximum defined value
				// gravity will then do the rest
				this.vel.y = -this.maxVel.y * me.timer.tick;
				// set the jumping flag
				this.jumping = true;
			}

        }

        // check & update player movement
        this.updateMovement();

        // update animation if necessary
        if (this.vel.x!=0 || this.vel.y!=0) {
            // update object animation
            this.parent();
            return true;
        }
		
		// else inform the engine we did not perform
		// any update (e.g. position, animation)
        return false;
    }

});

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
		//me.plugin.register(debugPanel, "debug");
		
		//Set Preloads
		//me.loader.onload = this.onload.bind(this);
		me.loader.onload = this.preloadsLoaded;
		me.loader.preload(preloads);
		
        // load everything & display a loading screen
        me.state.change(me.state.LOADING);
		
		log.debug( 'Leave jsApp.onload()');
	},
	
	preloadsLoaded: function() {
		//Do nothing
		log.debug('Enter jsApp.preloadsLoaded()');
	},
	
    loaded: function() {
		log.debug('Enter jsApp.loaded()');

        // add our player entity in the entity pool
       // me.entityPool.add("mainPlayer", PlayerEntity);
	   //me.entityPool.add("mainPlayer", me.game.PlayerEntity);

        // enable the keyboard
	    // enable the keyboard
		//me.input.bindKey(me.input.KEY.X,	"left");
		
		//me.entityPool.add("mainPlayer", me.game.PlayerEntity);

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
	
	playerPosX:0,
	playerPosY:0,
	
	loadMap: function(mapname, initalPlayerPosX, initalPlayerPosY)
	{
		jsApp.playerPosX=initalPlayerPosX;
		jsApp.playerPosY=initalPlayerPosY;
		
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
		//me.entityPool.add("mainPlayer", me.game.PlayerEntity);
		
		//var ent=new ()
		//this.moles[i] = new MoleEntity((112 + (i * 310)), 127+40)
		//me.game.add ( me.game.PlayerEntity);
		
        me.levelDirector.loadLevel(jsApp.currentyMap);
		
		//init person
		//alert(jsApp.playerPosX);
		//alert(jsApp.playerPosY);
		
		var settings={};
		
		var myPlayer = new me.game.PlayerEntity(jsApp.playerPosX, jsApp.playerPosY, settings);
		// Add the laser to the game manager with z value 3
		me.game.add(myPlayer, 100);
		// And sort everything
		me.game.sort();
		
		//keys
	    me.input.bindKey(me.input.KEY.LEFT,	"left");
	    me.input.bindKey(me.input.KEY.RIGHT,	"right");

		if(this.playScreen==null)
		{
			this.playScreen=new PlayScreen();
		}

		this.playScreen.setLevel(jsApp.currentyMap);
		
		//me.entityPool.add("mainPlayer", me.game.PlayerEntity);
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
	jsApp.loadMap(mapName, posX, posY); //map laden
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