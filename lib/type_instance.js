var util = require('util');
var events = require('events');

var TypeInstance = module.exports = function(_parent, id, meta) {
	this._parent = _parent;
	this.id = id;
	this.data = {
		property: Object.assign({}, meta.property),
		method: meta.method || {}
	};
};

util.inherits(TypeInstance, events.EventEmitter);

TypeInstance.prototype._emit = TypeInstance.prototype.emit;
TypeInstance.prototype.emit = function() {

	// emit signal via native addon
	var args = Array.prototype.slice.call(arguments);
	args.unshift(this.id);

	return this._parent.emitSignal.apply(this._parent, args);
};

TypeInstance.prototype._getProperty = function(propName) {
	return this.data.property[propName];
};

TypeInstance.prototype.getProperty = function(propName) {
	return this.data.property[propName];
};

TypeInstance.prototype._setProperty = function(propName, value) {
	this.data.property[propName] = value;
};

TypeInstance.prototype.setProperty = function(propName, value) {
	this._setProperty(propName, value);
	this.emit(propName + 'Changed');
};

TypeInstance.prototype._invokeMethod = function(methodName) {
	var args = Array.prototype.slice.call(arguments);
	args.shift();
	
	return this.data.method[methodName].handler.apply(this, args);
};

TypeInstance.prototype.invokeMethod = function() {

	// call method via native addon
	var args = Array.prototype.slice.call(arguments);
	args.unshift(this.id);

	return this._parent.invokeMethod.apply(this._parent, args);
};
