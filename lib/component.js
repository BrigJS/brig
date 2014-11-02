"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('../build/Release/brig.node');
var Item = require('./item');

var Component = module.exports = function(engine) {
	var self = this;

	self.engine = engine || null;
	self.native = null;
};

util.inherits(Component, events.EventEmitter);

Component.prototype.loadQmlFile = function(filename) {
	var self = this;

	self.native = new BrigCore.QmlComponent();

	// Loading specific URL
	self.native.setEngine(self.engine.native);
	self.native.loadUrl(filename);

	self.native.on('progressChanged', function() {
		console.log('progressChanged 1', arguments[0]);
	});

	self.native.on('progressChanged', function() {
		console.log('progressChanged 2', arguments[0]);
	});

	self.native.on('statusChanged', function(status) {

		console.log('statusChanged', arguments[0]);

		// Ready
		if (status == 1) {
			self.emit('loaded');
		}
	});
/*
	setImmediate(function() {
		self.emit('loaded');
	});
*/
};

Component.prototype.create = function() {
	var self = this;

	return new Item(self);
};
