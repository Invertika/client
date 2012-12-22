var MessageIn = new Class({
	id: 0,
	data: null,
	dataView: null,
	position: 0,

	//Konstruktor
	initialize: function(message) {
        this.data=message;
		this.dataView=new DataView(this.data);
		this.id=getInt16();
    },
	
	//Methoden	
    getInt16: function() {				
		this.dataView.getInt16(position, true);
		position+=2;
    }	
});