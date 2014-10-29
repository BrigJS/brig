"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('../build/Release/brig.node');
var Engine = require('./engine');
var Context = require('./context');

var Application = module.exports = function() {
	var self = this;

	self.app = new BrigCore.QApplication();
	self.engine = new Engine();
	self.context = new Context();
};

util.inherits(Application, events.EventEmitter);

Application.prototype.open = function(filename) {
	var self = this;

	return self.engine.load(filename);
};
