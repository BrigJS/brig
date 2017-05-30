"use strict";

var util = require('util');
var events = require('events');
var BrigCore = require('./core');

var Type = module.exports = function(index, typeName, meta) {

	this.index = index;
	this.typeName = typeName;
	this.meta = Object.assign({
		signal: {},
		method: {},
		property: {}
	}, meta || {});
	this.metaObject = {
		signal: {},
		method: {},
		property: {}
	};
	this.native = new BrigCore.QmlTypeBuilder(typeName);

	this.signal = new events.EventEmitter();
};

util.inherits(Type, events.EventEmitter);

Type.prototype.buildMetaObject = function() {

	var meta = this.meta;

	if (meta.signal) {
		for (var signature in this.meta.signal) {
			this._addSignal(signature, this.meta.signal[signature]);
			console.log('Added signal:', signature);
		}
	}

	if (this.meta.method) {
		for (var signature in this.meta.method) {
			this._addMethod(signature, this.meta.method[signature]);
			console.log('Added method:', signature);
		}
	}

	if (meta.property) {
		for (var propName in meta.property) {
			this._addProperty(propName);
			console.log('Added property:', propName);
		}
	}
};

Type.prototype.build = function(uri, versionMajor, versionMinor) {
	var major = 1;
	var minor = 0

	if (versionMajor != undefined)
		major = versionMajor;

	if (versionMinor != undefined)
		minor = versionMinor;

	this.native.build(uri, major, minor, this.index);
};

Type.prototype._addSignal = function(signature, callback) {

	var parts = signature.split('(');
	var name = parts[0];
	var parameters;
	var definitions = parts[1].replace(')', '');
	if (definitions) {
		parameters = definitions.split(/\s*,\s*/);
		definitions = parameters.map(function() {
			return 'QVariant';
		}).join(',');
	} else {
		parameters = [];
	}
	
console.log(name + '(' + definitions + ')');
	this.native.addSignal(name, name + '(' + definitions + ')', parameters, this._signalHandler.bind(this));

	// Create meta-object
	this.metaObject.method[name] = {
		name: name,
		signature: signature,
		handler: callback
	};

	// Add event listener
	this.signal.on(name, callback);
};

Type.prototype._signalHandler = function() {
	this.signal.emit.apply(this, arguments);
};

Type.prototype._addMethod = function(signature, callback) {

	var parts = signature.split('(');
	var name = parts[0];
	var parameters;
	var definitions = parts[1].replace(')', '');
	if (definitions) {
		parameters = definitions.split(/\s*,\s*/);
		definitions = parameters.map(function() {
			return 'QVariant';
		}).join(',');
	} else {
		parameters = [];
	}

	this.native.addMethod(name, name + '(' + definitions + ')', parameters, this._methodHandler.bind(this));

	// Create meta-object
	this.metaObject.method[name] = {
		name: name,
		signature: signature,
		handler: callback
	};
};

Type.prototype._methodHandler = function() {
	var args = Array.prototype.slice.call(arguments);
	var name = args.shift();
	return this.metaObject.method[name].handler.apply(this, args);
};

Type.prototype._addProperty = function(propName) {
	this.native.addProperty(propName, this._readProperty.bind(this), this._writeProperty.bind(this));
};

Type.prototype._readProperty = function(propName) {
	return this.meta.property[propName];
};

Type.prototype._writeProperty = function(propName, value) {
	this.meta.property[propName] = value;
};
