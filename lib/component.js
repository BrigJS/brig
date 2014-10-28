"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('../build/Release/brig.node');

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

	self.emit('loaded');
};

Component.prototype.create = function() {
	var self = this;

	return self.create();
};
