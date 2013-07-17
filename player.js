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
		this.doWalk(true);
		return true;
		
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