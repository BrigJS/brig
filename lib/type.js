"use strict";

var util = require('util');
var events = require('events');
var BrigCore = require('./core');
var TypeInstance = require('./type_instance');

var Type = module.exports = function(manager, index, typeName, meta) {

	this.manager = manager;
	this.index = index;
	this.typeName = typeName;
	this.counter = 0;
	this.instances = {};
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

	// Signal handler
	this.native.addSignalListener((function(instId, signalName) {

		// Select instance
		var instance = this.getInstance(instId);
		var args = Array.prototype.slice.call(arguments);
		args.shift();

		// Fire event
		instance._emit.apply(instance, args);
	}).bind(this));

	// Listen to QML type builder
	this.native.setListener((function(eventName, instId) {

		// A new instance was created
		var instance = new TypeInstance(this, instId, this.metaObject);
		this.instances[instId] = instance;

		this.emit('instance-created', instance);	

	}).bind(this));
};

util.inherits(Type, events.EventEmitter);

Type.prototype.getInstance = function(id) {
	return this.instances[id] || null;
};

Type.prototype.buildMetaObject = function() {

	var meta = this.meta;

	if (meta.signal) {
		for (var index in this.meta.signal) {
			var signature = this.meta.signal[index];
			this._addSignal(signature);
		}
	}

	if (this.meta.method) {
		for (var signature in this.meta.method) {
			this._addMethod(signature, this.meta.method[signature]);
		}
	}

	if (meta.property) {
		for (var propName in this.meta.property) {
			this._addProperty(propName, this.meta.property[propName]);
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

Type.prototype._addSignal = function(signature) {

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
	
	this.native.addSignal(name, name + '(' + definitions + ')', parameters);

	// Create meta-object
	this.metaObject.signal[name] = {
		name: name,
		signature: signature
	};
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
	var id = args.shift();

	// Select instance
	var instance = this.getInstance(id);

	return instance._invokeMethod.apply(instance, args);
};

Type.prototype._addProperty = function(propName, defaultValue) {

	// Create meta-object
	this.metaObject.property[propName] = defaultValue;

	var signalName = propName + 'Changed';
	this.metaObject.signal[signalName] = {
		name: signalName,
		signature: signalName + '(value)',
		handler: null
	};

	this.native.addProperty(propName,
		this._readProperty.bind(this),
		this._writeProperty.bind(this));
};

Type.prototype._readProperty = function(instId, propName) {
	return this.getInstance(instId)._getProperty(propName);
};

Type.prototype._writeProperty = function(instId, propName, value) {
	this.getInstance(instId)._setProperty(propName, value);
};

Type.prototype.invokeMethod = function() {

	try {
		return this.native.invokeMethod.apply(this.native, arguments);
	} catch(e) {
		console.log(e);
	}
};

Type.prototype.emitSignal = function() {

	try {
		return this.native.emitSignal.apply(this.native, arguments);
	} catch(e) {
		console.log(e);
	}
};
