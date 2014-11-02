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

	if (!this.native) {
		this.native = new BrigCore.QmlComponent();
		this.native.setEngine(this.engine.native);
	}

	// Loading specific URL
	self.native.loadUrl(filename);

	self.native.on('progressChanged', function(progress) {
		self.emit('progressChanged', progress);
	});

	self.native.on('statusChanged', function(status) {

		// Ready
		if (status == 1) {
			self.emit('loaded');
		}
	});
};

Component.prototype.create = function() {
	var self = this;

	return new Item(self);
};

Component.prototype.setData = function(data) {

	if (!this.native) {
		this.native = new BrigCore.QmlComponent();
		this.native.setEngine(this.engine.native);
	}

	this.native.setData(data);
};
