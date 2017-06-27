var Type = require('./type');

var TypeManager = module.exports = function(engine) {
	this._parent = engine;
	this.types = {};
	this.limit = 30;
	this.registered = [];

	for (var index = 0; index < this.limit; index++) {
		this.registered.push(null);
	}
};

TypeManager.prototype.getAvailableIndex = function() {

	for (var index in this.registered) {
		var type = this.registered[index];

		if (!type)
			return index;
	}

	return -1;
};

TypeManager.prototype.createType = function(typeName, meta) {

	var index = this.getAvailableIndex();
	if (index === -1)
		throw new Error('too many registered types.');

	console.log('Register Type typeName=' + typeName + ', index=' + index);
	var type = new Type(this, index, typeName, meta);

	type.buildMetaObject();
	type.build('Brig.' + typeName);

	this.registered[index] = type;

	return type;
};

TypeManager.prototype.getType = function(typeName) {
	return this.registered.find(function(type) {
		return (type.typeName === typeName)
	});
};
