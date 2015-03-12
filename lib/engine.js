"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('../build/Release/brig.node');
var Context = require('./context');
var Component = require('./component');
var Type = require('./type');

var Engine = module.exports = function() {
	var self = this;

	self.className = 'Engine';
	self.native = new BrigCore.QmlEngine();
	self.native.on('warnings', function() {
		console.log('warnings');
	});
	self.native.on('quit', function() {
		self.emit('quit');
	});

	self.rootContext = new Context(self);
	self.types = {};
};

util.inherits(Engine, events.EventEmitter);

Engine.prototype.load = function(filename, callback) {
	var self = this;

	var component = new Component(self);
	component.loadQmlFile(filename);
	component.on('loaded', function() {
		var item = component.create();

		if (callback)
			callback(null, item);
	});

	component.on('error', function() {
		if (callback)
			callback(new Error('Cannot load file'));
	});
};

Engine.prototype.createComponent = function() {

	return new Component(this);
};

Engine.prototype.createType = function(typeName, meta) {
	var self = this;

	var type = new Type(typeName);

	if (meta.signals) {
		for (var signature in meta.signals) {
			type.addSignal(signature, meta.signals[signature]);
		}
	}

	type.build();

	self.types[typeName] = type;
};
