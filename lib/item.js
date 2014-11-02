"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('../build/Release/brig.node');

var Item = module.exports = function(component) {
	var self = this;

	self.native = new BrigCore.QuickItem();
	self.native.create(component.native);
};

util.inherits(Item, events.EventEmitter);

Item.prototype.getPropertyNames = function() {
	return this.native.getPropertyNames();
};

Item.prototype.getProperty = function(propName) {
	return this.native.getProperty(propName);
};

Item.prototype.setProperty = function(propName, value) {
	this.native.setProperty(propName, value);
};
