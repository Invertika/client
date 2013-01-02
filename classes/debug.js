function getEnumFromInteger(enumWithValues, integer)
{
	var keys = Object.keys(enumWithValues);
	
	for (var i=0; i<keys.length; i++)
	{
		if(enumWithValues[keys[i]]==integer) return keys[i];
	}
	
	return null;
}