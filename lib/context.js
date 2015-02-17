"use strict";

var BrigCore = require('../build/Release/brig.node');

var Context = module.exports = function(engine) {
	var self = this;

	self.engine = engine || null;
	self.native = new BrigCore.QmlContext(self.engine.native);
};

Context.prototype.append = function() {
};
