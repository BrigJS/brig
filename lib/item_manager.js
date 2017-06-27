var Item = require('./item');

var ItemManager = module.exports = function() {
	this.items = {};
};

ItemManager.prototype.getItem = function(id) {

	if (this.items[id] === undefined)
		this.items[id] = new Item(id);

	return this.items[id];
};
