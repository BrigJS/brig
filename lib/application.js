"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('./core');
var Engine = require('./engine');
var Context = require('./context');

var Application = module.exports = function(brig) {
	var self = this;

	self.brig = brig;
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

Application.prototype.loadModule = function(m) {
	if (!m)
		throw new Error('require to give a module');

	this.engine.addModulePath(m.importPaths);

	// Create types
	if (m.types instanceof Array) {
		m.types.forEach(function(typePrototype) {
			var type = this.engine.loadTypePrototype(typePrototype);
			type.on('instance-created', function(instance) {
				typePrototype.emit('instance-created', instance);
			})
			typePrototype.emit('registered', type);
		}.bind(this));
	}
};

Application.prototype.addModulePath = function(modulePath) {
	this.engine.addModulePath(modulePath);
};
