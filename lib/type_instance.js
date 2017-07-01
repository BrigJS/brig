var util = require('util');
var events = require('events');

var TypeInstance = module.exports = function(_parent, id, meta) {
	this._parent = _parent;
	this.id = id;
	this.data = {
		property: Object.assign({}, meta.property),
		method: meta.method || {}
	};

	// Wrap value of property
	Object.keys(this.data.property).forEach(function(propName) {
		this.data.property[propName] = { t: 0, v: this.data.property[propName] };
	}.bind(this));
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
//	console.log('_getProperty', propName, this.data.property[propName]);
	return this.data.property[propName];
};

TypeInstance.prototype.getProperty = function(propName) {
	return this.brigValue2Data(this.data.property[propName]);
	/*
	var prop = this.data.property[propName];

	// It's a QQuickItem
	if (prop.t === 39) {
		// Wrap value
		var engine = this._parent.manager._parent;
		return engine.itemManager.getItem(this.data.property[propName].v);
	}


	return this.data.property[propName].v;
	*/
};

TypeInstance.prototype._setProperty = function(propName, value) {
//	console.log('_setProperty', propName, value);
	this.data.property[propName] = value;
};

TypeInstance.prototype.setProperty = function(propName, value) {
	// TODO: detect type
	this._setProperty(propName, { t: 0, v: value });
	this.emit(propName + 'Changed');
};

TypeInstance.prototype._invokeMethod = function(methodName) {
	var args = Array.prototype.slice.call(arguments);
	args.shift();

	// Convert javascript value to brig value
	args = args.map(function(arg) {
		return this.brigValue2Data(arg);
	}.bind(this));

	var ret = this.data.method[methodName].handler.apply(this, args);
	if (ret instanceof Buffer) {
		return new Uint8Array(ret);
	}

	return ret;
};

TypeInstance.prototype.invokeMethod = function() {

	// call method via native addon
	var args = Array.prototype.slice.call(arguments);
	args.unshift(this.id);

	return this._parent.invokeMethod.apply(this._parent, args);
};

TypeInstance.prototype.brigValue2Data = function(value) {

	// It's a QQuickItem
	if (value.t === 39) {
		// Wrap value
		var engine = this._parent.manager._parent;
		return engine.itemManager.getItem(value.v);
	}

	return value.v;
};
