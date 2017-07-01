"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('./core');
var Component = require('./component');

var Item = module.exports = function() {
	var self = this;

	this.engine = null;
	this.native = new BrigCore.QuickItem();

	if (arguments[0] >= 0) {
		self.native.create(arguments[0]);
	} else {
		var component = arguments[0];
		self.native.create(component.native);
	}

	var _emit = self.emit;
	self.emit = function(eventName) {

		var args = Array.prototype.slice.call(arguments);
		var success = self.native.emitEvent.apply(self.native, args);

		if (!success)
			_emit.apply(self, args);
	};

	var _on = self.on;
	self.on = function() {

		var args = Array.prototype.slice.call(arguments);
		args = args.map(function(arg) {
			return arg.v;
		});
		self.native.on.apply(self.native, args);
	};
};

util.inherits(Item, events.EventEmitter);

Item.prototype.setEngine = function(engine) {
	this.engine = engine || null;
};

Item.prototype.hasProperty = function(propName) {

	var names = this.getPropertyNames();
	for (var index in names) {

		if (names[index] === propName)
			return true;
	}

	return false;
};

Item.prototype.getPropertyNames = function() {
	return this.native.getPropertyNames();
};

Item.prototype.getProperty = function(propName) {
	return this.native.getProperty(propName).v;
};

Item.prototype.setProperty = function(propName, value) {
	this.native.setProperty(propName, value);
};

Item.prototype.setParent = function(item) {
	this.native.setParent(item.native);
};

Item.prototype.invokeMethod = function() {

	var ret = this.native.invokeMethod.apply(this.native, Array.prototype.slice.call(arguments));

	// It's a QQuickItem
	if (ret.t === 39) {
		return this.engine.itemManager.getItem(ret.v);
	}

	return ret.v;
};
