var Message = new Class({
	id: 0,
	parts: [],
	
	//Konstruktor
	initialize: function(id) {
        this.id = id;
    },
	
	//Methoden
    addValue: function(value) {
		var maskedValue = value.replace(/:/g, "::");
		this.parts.include(maskedValue);
    },
	
	getString: function() {
		var ret=this.id.toString(16)+":";
		var length=this.parts.length;

		this.parts.each(function(part, index){
			if(index == length-1) ret+=part;
			else ret+=part+":";
		});
		
		return ret;
	}
});