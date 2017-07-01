var Item = require('./item');

var ItemManager = module.exports = function(engine) {
	this.engine = engine || null;
	this.items = {};
};

ItemManager.prototype.getItem = function(id) {

	if (this.items[id] === undefined) {

		var item = new Item(id);
		item.setEngine(this.engine);
		this.items[id] = item;

		return item;
	}

	return this.items[id];
};
