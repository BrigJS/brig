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
	// TODO: Fire event with notifying QML object
};

TypeInstance.prototype.getProperty = function(propName) {
	return this.data.property[propName];
};

TypeInstance.prototype.setProperty = function(propName, value) {
	this.data.property[propName] = value;
};

TypeInstance.prototype._invokeMethod = function(methodName) {
	var args = Array.prototype.slice.call(arguments);
	args.shift();
	
	return this.data.method[methodName].handler.apply(this, args);
};

TypeInstance.prototype.invokeMethod = function() {

	// TODO: call method via native addon
	var args = Array.prototype.slice.call(arguments);
	args.unshift(this.id);

	return this._parent.invokeMethod.apply(this._parent, args);
};
