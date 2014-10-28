"use strict";

var BrigCore = require('../build/Release/brig.node');

var Context = module.exports = function(engine) {
	var self = this;

	self.engine = engine || null;
};

Context.prototype.append = function() {
};
