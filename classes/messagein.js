var MessageIn = new Class({
	id: 0,
	data: null,
	dataView: null,
	position: 0,

	//Konstruktor
	initialize: function(message) {
        this.data=new ArrayBuffer(message.length);;
		
		//In Arraybuffer schreiben
		var b = new Uint8Array(this.data);
		for(var i = 0; i < message.length; i++){
		    b[i] = message.charCodeAt(i);
		}
		
		this.dataView=new DataView(this.data);
		this.id=this.getInt16();
    },
	
	//Methoden	
    getInt16: function() {				
		var ret=this.dataView.getInt16(this.position, true);
		this.position+=2;
		return ret;
    },
	
    getString: function() {		
		//Länge ermitteln
		var lengthOfString=this.getInt16();
		
		//String umwandeln
		var bytesOfString=this.data.slice(this.position, lengthOfString);
		
		//Arraybuffer in normales JavaScript Array konvertieren
		var array=new Array();

		for(var i = 0; i < lengthOfString; i++){
		    array[i] = this.dataView.getUint8(this.position+i);
		}
		
		//Array dekodieren
		var ret=Utf8Utils.decode(array);
		
		//Position aktualisieren
		this.position+=lengthOfString;
		
		return ret;
    }
});