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
