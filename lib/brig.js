"use strict";

var BrigCore = require('../build/Release/brig.node');

var Brig = module.exports = function() {
	var self = this;

	self.app = new BrigCore.QApplication();

	for (var className in BrigCore) {
		self[className] = BrigCore[className];
	}
};

Brig.prototype.main = function() {
	var self = this;
	self.app.test();
	self.app.exec();
};

