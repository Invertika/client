var MessageOut = new Class({
	id: 0,
	parts: [],
	data: null,
	dataView: null,
	length: 0,
	maxLength: 1000,
	
	//Konstruktor
	initialize: function(id) {
        this.id = id;		
		
		this.data=new ArrayBuffer(this.maxLength);
		this.dataView=new DataView(this.data);
		
		//ID schreiben
		this.addValueAsInt16(this.id);
		//this.dataView.setInt16(length, this.id);
		//length+=2;
    },
	
	//Methoden
    addValueAsInt8: function(value) {		
		if((this.length+1)>=this.maxLength)
		{
			log.error('Maximum byte count for MessageOut is reached.');
			return;
		}
		
		this.dataView.setInt8(length, value);
		length+=1;
    },
	
    addValueAsInt16: function(value) {		
		if((this.length+2)>=this.maxLength)
		{
			log.error('Maximum byte count for MessageOut is reached.');
			return;
		}
		
		this.dataView.setInt16(length, value);
		length+=2;
    },
	
    addValueAsString: function(value) {		
		if((this.length+2)>=this.maxLength)
		{
			log.error('Maximum byte count for MessageOut is reached.');
			return;
		}
		
		//Länge setzen (in Zeichen)
		this.addValueAsInt16(value.length);
		//this.dataView.setInt16(length, value);
		//length+=2;
		
	    //var buf = new ArrayBuffer(str.length*2); // 2 bytes for each char
	    var bufView = new Uint16Array(this.data);
   
	    for (var i=0; i<value.length; i++) 
		{
	      bufView[length+i] = value.charCodeAt(i);
	    }
    },
	
	getBinary: function() {
		//Gibt das Datenarray zurück und schneidet es vorher auf die richtige Größe
		return this.data.slice(0, length-1);
	}
});