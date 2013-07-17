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
		this.gravity=0; //player in a 2d adventure has no gravity
		this.setVelocity(3, 3);

        // set the display to follow our position on both axis
        me.game.viewport.follow(this.pos, me.game.viewport.AXIS.BOTH);
		
  	    me.input.bindKey(me.input.KEY.LEFT,	"left");
  	    me.input.bindKey(me.input.KEY.RIGHT,	"right");
  	    me.input.bindKey(me.input.KEY.UP,	"up");
  	    me.input.bindKey(me.input.KEY.DOWN,	"down");

    },

    /* -----

    update the player pos

    ------ */
    update: function() {
		
		var updated=false;
		
		if (me.input.isKeyPressed('left'))
		{
			//console.log("left");
			// update the entity velocity
			this.vel.x -= this.accel.x * me.timer.tick;
			//this.directionString = "left";
			updated=true;
		}
		else if (me.input.isKeyPressed('right'))
		{
			console.log("right");
			// update the entity velocity
			this.vel.x += this.accel.x * me.timer.tick;
			//this.directionString = "right";
			update=true;
		}
		else
		{
			this.vel.x = 0;
		}
		
		
		if (me.input.isKeyPressed('up'))
		{
			//console.log("up");
			//alert("up");
			// update the entity velocity
			this.vel.y -= this.accel.y * me.timer.tick;
			//console.log(this.accel.y );
			//console.log(this.vel.y );
			//this.directionString = "up";
			update=true;
		}
		else if (me.input.isKeyPressed('down'))
		{
			//console.log("down");
			//alert("down");
			// update the entity velocity
			this.vel.y += this.accel.y * me.timer.tick;
			//this.directionString = "down";
			update=true;
		}
		else
		{
			//this.vel.y = 0;
			this.vel.y = 0;
		}
		
		//console.log(this.vel.x );
		//console.log(this.vel.y );
		
        // check & update player movement
       this.updateMovement();
		//return true;
		
		if(updated)
		{
            gameServer.walk(this.pos.x, this.pos.y);
		}
		
        // update animation if necessary
        if (this.vel.x!=0 || this.vel.y!=0) {
            // update object animation
			//alert("update");
            this.parent();
			//this.setCurrentAnimation(this.directionString + "walk");
            return true;
        }
		//alert("update");
		// else false the engine we did not perform
		// any update (e.g. position, animation)
        return false;
    }
    

});