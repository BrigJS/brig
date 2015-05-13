"use strict";

var BrigCore = require('./core');

var Context = module.exports = function(engine) {
	var self = this;

	self.engine = engine || null;
	self.native = new BrigCore.QmlContext(self.engine.native);
};

Context.prototype.append = function() {
};
