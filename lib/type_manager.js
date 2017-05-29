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
	var type = new Type(index, typeName);

	if (meta.signal) {
		for (var signature in meta.signal) {
			type.addSignal(signature, meta.signal[signature]);
			console.log('Added signal:', signature);
		}
	}

	if (meta.method) {
		for (var signature in meta.method) {
			type.addMethod(signature, meta.method[signature]);
			console.log('Added method:', signature);
		}
	}
/*
	if (meta.property) {
		for (var propName in meta.property) {
			type.addProperty(propName);
			console.log('Added property:', propName);
		}
	}
*/
	//type.build('brig.' + typeName.toLowerCase());
	type.build('Brig.' + typeName);

	this.types[typeName] = type;
	this.registered[index] = type;

	return type;
};
