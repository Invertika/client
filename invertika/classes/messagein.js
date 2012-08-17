var MessageIn = new Class({
	id: 0,
	parts: [],
	
	//Konstruktor
	initialize: function(message) {
        this.parts = splitCommand(message);
    },
	
	//Methoden
	getPart: function(index) {		
		return this.parts[index];
	},
	
	//Private Methoden
	splitCommand: function(command) {
		ret=new Array();
		
		cur="";
		
		for(var i=0; i<command.Length; i++)
		{
			if(command[i]==':')
			{
				if(i+1<command.Length&&command[i+1]==':')
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

		if(cur.Length!=0) ret.include(cur);

		return ret;
	}.protect();			
});