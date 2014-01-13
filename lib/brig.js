"use strict";

var BrigCore = require('../build/Release/brig.node');

var app;

var Brig = module.exports = function() {
	var self = this;

	if (!app)
		app = self.app = new BrigCore.QApplication();
	else
		self.app = app;

	for (var className in BrigCore) {
		self[className] = BrigCore[className];
	}
};

Brig.prototype.main = function() {
	var self = this;

	self.app.test();
	self.app.exec();
};
