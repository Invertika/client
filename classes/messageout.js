var MessageOut = new Class({
	id: 0,
	parts: [],
	data,
	dataView,
	length,
	maxLength,
	
	//Konstruktor
	initialize: function(id) {
        this.id = id;		
		
		this.length=0;
		this.maxLength=1000;
		
		this.data=new ArrayBuffer(this.maxLength);
		this.dataView=new DataView(this.data);
		
		//ID schreiben
		this.dataView.setInt16(length, this.id);
		length+=2;
    },
	
	//Methoden
    addValueAsInt8: function(value) {
		//if(typeOf(value)=='string')
		//{
		//	var maskedValue = value.replace(/:/g, "::");
		//	this.parts.include(maskedValue);
		//}
		//else
		//{
		//	this.parts.include(value);
		//}
		
		//this.parts.include(value);
		
		if((this.length+1)>=this.maxLength)
		{
			log.error('Maximum byte count for MessageOut is reached.');
			return;
		}
		
		this.dataView.setInt8(length, value);
		length+=1;
    },
	
	getBinary: function() {
		var ret=this.id.toString(16)+":";
		var length=this.parts.length;

		this.parts.each(function(part, index){
			if(index == length-1) ret+=part;
			else ret+=part+":";
		});
		
		return ret;
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