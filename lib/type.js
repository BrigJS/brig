"use strict";

var BrigCore = require('./core');

var Type = module.exports = function(index, typeName) {
	var self = this;

	self.index = index;
	self.typeName = typeName;
	self.native = new BrigCore.QmlTypeBuilder(typeName);
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

Type.prototype.addSignal = function(signature, callback) {

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

Type.prototype.addMethod = function(signature, callback) {

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

Type.prototype.addProperty = function(propName) {
	this.native.addProperty(propName);
};
