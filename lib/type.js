"use strict";

var BrigCore = require('./core');

var Type = module.exports = function(typeName) {
	var self = this;

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

	this.native.build(uri, major, minor);
};

Type.prototype.addSignal = function(signature, callback) {
	this.native.addSignal(signature, callback);
};
