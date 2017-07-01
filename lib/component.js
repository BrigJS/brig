"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('./core');
var Item = require('./item');

var Component = module.exports = function(engine) {
	var self = this;

	self.engine = engine || null;
	self.native = null;
};

util.inherits(Component, events.EventEmitter);

Component.prototype.ensureInstance = function() {

	if (!this.native) {
		this.native = new BrigCore.QmlComponent();
		this.native.setEngine(this.engine.native);
	}
};

Component.prototype.loadQmlFile = function(filename) {
	var self = this;

	this.ensureInstance();

	// Loading specific URL
	self.native.loadUrl(filename);

	self.native.on('progressChanged', function(progress) {
		self.emit('progressChanged', progress.v);
	});

	self.native.on('statusChanged', function(status) {

		switch(status.v) {
		case 1:
			// Ready
			self.emit('loaded');
			break;

		case 3:
			// Error
			try {
				self.emit('error', self.native.errors());
			} catch(e) {}
			break;
		}
	});
};

Component.prototype.setData = function(data) {
	this.ensureInstance();
	this.native.setData(data);
};

Component.prototype.create = function() {
	var item = new Item(this);
	item.setEngine(this.engine);

	return item;
};
