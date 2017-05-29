"use strict";

var BrigCore = require('./core');

var Type = module.exports = function(index, typeName, meta) {

	this.index = index;
	this.typeName = typeName;
	this.meta = Object.assign({
		signal: {},
		method: {},
		property: {}
	}, meta || {});
	this.native = new BrigCore.QmlTypeBuilder(typeName);
};

Type.prototype.buildMetaObject = function() {

	var meta = this.meta;

	if (meta.signal) {
		for (var signature in meta.signal) {
			this._addSignal(signature, meta.signal[signature]);
			console.log('Added signal:', signature);
		}
	}

	if (meta.method) {
		for (var signature in meta.method) {
			this._addMethod(signature, meta.method[signature]);
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
	this.native.addSignal(name, name + '(' + definitions + ')', parameters, callback);
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

	this.native.addMethod(name, name + '(' + definitions + ')', parameters, callback);
};

Type.prototype._addProperty = function(propName) {
	console.log(propName, this._readProperty.bind(this));
	this.native.addProperty(propName, this._readProperty.bind(this), this._writeProperty.bind(this));
};

Type.prototype._readProperty = function(propName) {
	return this.meta.property[propName];
};

Type.prototype._writeProperty = function(propName, value) {
	console.log('WRITE', propName, value);
	this.meta.property[propName] = value;
};
