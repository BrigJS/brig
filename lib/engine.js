"use strict";

var events = require('events');
var util = require('util');
var BrigCore = require('./core');
var Context = require('./context');
var Component = require('./component');
var ItemManager = require('./item_manager');
var TypeManager = require('./type_manager');

var Engine = module.exports = function() {
	var self = this;

	self.className = 'Engine';
	self.native = new BrigCore.QmlEngine();
/*
	self.native.on('warnings', function(x) {
		console.log('warnings');
	});
*/
	self.native.on('quit', function() {
		self.emit('quit');
	});

	self.rootContext = new Context(self);
	self.itemManager = new ItemManager(this);
	self.outputErrorToStandardError = true;

	// Customized types
	self.typeManager = new TypeManager(this);
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

	component.on('error', function(errs) {
		if (self.outputErrorToStandardError) {
			console.error(errs.join('\n'));
		}

		if (callback)
			callback(errs);
	});
};

Engine.prototype.createComponent = function() {

	return new Component(this);
};

Engine.prototype.createType = function(typeName, meta) {
	return this.typeManager.createType(typeName, meta);
};

Engine.prototype.addModulePath = function(modulePath) {
	this.native.addImportPath(modulePath);
	this.native.addPluginPath(modulePath);
};

Engine.prototype.loadTypePrototype= function(proto) {

	return this.typeManager.createType(proto.name, {
		property: proto.properties,
		method: proto.methods,
		signal: proto.signals
	});
};
