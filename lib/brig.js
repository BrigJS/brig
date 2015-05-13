"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('./core');
var Application = require('./application');

var app = null;

var Brig = module.exports = function() {
	var self = this;

	// Singleton
	app = app || new Application();
	self.app = app;

	setImmediate(function() {
		self.emit('ready', self);
	});
};

util.inherits(Brig, events.EventEmitter);

Brig.prototype.createApp = function() {
};

Brig.prototype.open = function(filename, callback) {
	var self = this;

	self.app.open(filename, callback);
};

Brig.prototype.createComponent = function() {
	return this.app.createComponent();
};

Brig.prototype.createType = function(typeName, meta) {
	return this.app.createType(typeName, meta);
};
