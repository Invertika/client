var MessageIn = new Class({
	id: 0,
	data: null,
	dataView: null,
	position: 0,

	//Konstruktor
	initialize: function(message) {
        this.data=message;
		this.dataView=new DataView(this.data);
		this.id=this.getInt16();
    },
	
	//Methoden	
    getInt16: function() {				
		this.dataView.getInt16(position, true);
		position+=2;
    },
	
    getString: function() {		
		//Länge ermitteln
		var lengthOfString=this.getInt16();
		
		//String umwandeln
		var ret=Utf8Utils.decode(this.data.slice(position, lengthOfString));
		
		//Position aktualisieren
		length+=lengthOfString;
		
		return ret;
    }
});