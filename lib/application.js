"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('./core');
var Engine = require('./engine');
var Context = require('./context');

var Application = module.exports = function() {
	var self = this;

	self.app = new BrigCore.QApplication();
	self.engine = new Engine();
	self.context = self.engine.rootContext;
	self.windowCount = 0;
};

util.inherits(Application, events.EventEmitter);

Application.prototype.setStyle = function(style) {
	this.app.setStyle(style);
};

Application.prototype.setApplicationName = function(name) {
	this.app.setApplicationName(name);
};

Application.prototype.open = function(filename, callback) {
	var self = this;

	self.engine.load(filename, function(err, window) {
		if (!err) {
			window.on('closing', function() {
				self.windowCount--;

				if (self.windowCount === 0)
					self.emit('window-all-closed');
			});

			self.windowCount++;
		}

		callback(arguments[0], arguments[1]);
	});
};

Application.prototype.createComponent = function() {
	return this.engine.createComponent();
};

Application.prototype.createType = function(typeName, meta) {
	return this.engine.createType(typeName, meta);
};
