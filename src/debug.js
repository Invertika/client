// Debug.Client.Repository

function getEnumFromInteger(enumWithValues, integer)
{
	var keys = Object.keys(enumWithValues);
	
	for (var i=0; i<keys.length; i++)
	{
		if(enumWithValues[keys[i]]==integer) return keys[i];
	}
	
	return null;
}

function _TinyTMXParser() {

	var parserObj = {

		tmxDoc : null,


		// parse a TMX XML file
		setData : function(data) {
			this.tmxDoc = data;
		},

		getFirstElementByTagName : function(name) {
			return this.tmxDoc ? this.tmxDoc.getElementsByTagName(name)[0] : null;
		},

		getAllTagElements : function() {
			return this.tmxDoc ? this.tmxDoc.getElementsByTagName('*') : null;
		},

		getStringAttribute : function(elt, str, val) {
			var ret = elt.getAttribute(str);
			return ret ? ret.trim() : val;
		},

		getIntAttribute : function(elt, str, val) {
			var ret = this.getStringAttribute(elt, str, val);
			return ret ? parseInt(ret) : val;
		},

		getFloatAttribute : function(elt, str, val) {
			var ret = this.getStringAttribute(elt, str, val);
			return ret ? parseFloat(ret) : val;
		},

		getBooleanAttribute : function(elt, str, val) {
			var ret = this.getStringAttribute(elt, str, val);
			return ret ? (ret == "true") : val;
		},

		// free the allocated parser
		free : function() {
			this.tmxDoc = null;
		}
	}

	return parserObj;
}

function getUsedResources(tmx)
{
	var Parser = new _TinyTMXParser();
	
	// init the parser
	Parser.setData(tmx);

	// retreive all the elements of the XML file
	var xmlElements = Parser.getAllTagElements();
	var imageURLS=new Array();
	// parse all tags
	for ( var i = 0; i < xmlElements.length; i++) {
	
		// check each Tag
		switch (xmlElements.item(i).nodeName) 
		{
			case 'tileset':
			{		
				//Get URLs of tileset images
				var urls =getTilesetImageURL(xmlElements.item(i));
				for(var j=0;j<urls.length;j++) imageURLS[imageURLS.length]=urls[j];	
				break;
			}
		}	
	}	
	
	//Debug URL Ausgabe
	//for(var i =0;i<imageURLS.length;i++)alert(imageURLS[i]); 
	
	return imageURLS;
}

//Returns an array of strings which contains the image urls for the specified tileset(xmlNode)
function getTilesetImageURL(xmlNode)
{
	var retArray=new Array();
	var j=0;
	for(var i=0;i<xmlNode.childNodes.length;i++)
	{
		if(xmlNode.childNodes.item(i).nodeName=='image')
		{			
			retArray[j++]=xmlNode.childNodes.item(i).getAttribute('source');			
		}			
	}	
	return retArray;
}
