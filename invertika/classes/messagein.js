var MessageIn = new Class({
	id: 0,
	parts: [],
	
	//Konstruktor
	initialize: function(message) {
        this.parts = this.splitCommand(message);
		this.id=parseInt(this.parts[0], 16);
		this.parts.splice(0, 1); //Erstes Element entfernen
    },
	
	//Methoden
	getPart: function(index) {		
		return this.parts[index];
	},
	
	//Private Methoden
	splitCommand: function(command) {
		ret=new Array();
		
		cur="";
		
		for(var i=0; i<command.length; i++)
		{
			if(command[i]==':')
			{
				if(i+1<command.length&&command[i+1]==':')
				{
					cur+=(':');
					i++;
				}
				else
				{
					ret.include(cur);
					cur="";
				}
				continue;
			}

			cur+=(command[i]);
		}

		if(cur.length!=0) ret.include(cur);

		return ret;
	}.protect()			
});