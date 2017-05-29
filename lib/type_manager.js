var Type = require('./type');

var TypeManager = module.exports = function() {
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

	console.log('Register Type, index=' + index);
	var type = new Type(index, typeName, meta);

	type.buildMetaObject();
	type.build('Brig.' + typeName);

	this.types[typeName] = type;
	this.registered[index] = type;

	return type;
};
