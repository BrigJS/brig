"use strict";

var BrigCore = require('../build/Release/brig.node');

var Type = module.exports = function(typeName) {
	var self = this;

	self.native = new BrigCore.QmlTypeBuilder();
};

Type.prototype.build = function() {
	this.native.build();
};

Type.prototype.addSignal = function(signature, callback) {
	this.native.addSignal(signature, callback);
};
