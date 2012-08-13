var Message = new Class({
	id: 0,
	parts: [],
	
	//Konstruktor
	initialize: function(id) {
        this.id = id;
    },
	
	//Methoden
    addValue: function(value) {
		this.parts.include(value);
    },
	
	getString: function() {
		var ret=this.id+":";
		var length=this.parts.length;

		this.parts.each(function(part, index){
			if(index == length-1) ret+=part;
			else ret+=part+":";
		});
		
		return ret;
	}
});