// Class: Messageout
    //
    // TODO: Debug mode (see the commit in manaserv master)
     
    function MessageOut(id)
    {
        // Create buffer with some initial data capacity
        this.data = new ArrayBuffer(16);
     
        // Create DataView object that points at buffer
        this.dataView = new DataView(this.data);
     
        // initial cursor position is 0
        this.pos = 0;
     
        // Write the message id
        this.writeUInt16(id);
    }
     
    MessageOut.prototype.getData = function()
    {
        return this.data.slice(0, this.pos);
    }
     
    /* expand buffer to size if necessary (not BY size) */
    MessageOut.prototype.expand = function(size)
    {
        if(this.data.byteLength < size) {
            // TODO: resize by some extra bytes to resize more often
            var newsize = size;
            var newdata = new ArrayBuffer(newsize);
            var i;
            for(i=0;i<this.data.byteLength;i++)
            {
                newdata[i] = this.data[i];
            }
            this.data = newdata;
        }
    }
     
    MessageOut.prototype.writeUInt8 = function(value)
    {
        this.expand(this.realsize + 1);
        this.dataView.setUint8(this.pos, value);
        this.pos++;
    }
     
    MessageOut.prototype.writeUInt16 = function(value)
    {
        this.expand(this.realsize + 2);
        this.dataView.setUint16(this.pos, value);
        this.pos += 2;
    } 