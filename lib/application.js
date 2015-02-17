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
	self.context = new Context(self.engine);
};

util.inherits(Application, events.EventEmitter);

Application.prototype.open = function(filename, callback) {
	var self = this;

	self.engine.load(filename, callback);
};

Application.prototype.createComponent = function() {
	return this.engine.createComponent();
};

Application.prototype.createType = function(typeName, meta) {
	return this.engine.createType(typeName, meta);
};
