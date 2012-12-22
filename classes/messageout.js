var MessageOut = new Class({
	id: 0,
	parts: [],
	data: null,
	dataView: null,
	length: 0,
	maxLength: 1000,
	
	//Konstruktor
	initialize: function(id) {
        this.id=id;		
		
		this.data=new ArrayBuffer(this.maxLength);
		this.dataView=new DataView(this.data);
		
		//ID schreiben
		this.addValueAsInt16(this.id);
    },
	
	//Methoden
    addValueAsInt8: function(value) {		
		if((this.length+1)>=this.maxLength)
		{
			log.error('Maximum byte count for MessageOut is reached.');
			return;
		}
		
		this.dataView.setInt8(length, value, true);
		length+=1;
    },
	
    addValueAsInt16: function(value) {		
		if((this.length+2)>=this.maxLength)
		{
			log.error('Maximum byte count for MessageOut is reached.');
			return;
		}
		
		this.dataView.setInt16(length, value, true);
		length+=2;
    },
	
    addValueAsString: function(value) {		
		if((this.length+2)>=this.maxLength)
		{
			log.error('Maximum byte count for MessageOut is reached.');
			return;
		}
		
		//String umwandeln
		var tmp=Utf8Utils.encode(value);
		
		//Länge setzen (in Zeichen)
		this.addValueAsInt16(tmp.length, true);
		
		//In Arraybuffer schreiben
		var b = new Uint8Array(this.data);
		for(var i = 0; i < tmp.length; i++){
		    b[length+i] = tmp[i];
		}
		
		//Länge aktualisieren
		length+=tmp.length;
    },
	
	getBinary: function() {
		//Gibt das Datenarray zurück und schneidet es vorher auf die richtige Größe
		//alert(length);
		return this.data.slice(0, length);
	}
});