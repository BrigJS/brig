"use strict";

var BrigCore = require('../build/Release/brig.node');
var Context = require('./context');
var Component = require('./component');

var Engine = module.exports = function() {
	var self = this;

	self.native = new BrigCore.QmlEngine();
	self.rootContext = new Context(self);
};

Engine.prototype.load = function(filename, callback) {
	var self = this;

	var component = new Component(self);
	component.loadQmlFile(filename);
	component.on('loaded', function() {
		var item = component.create();

		callback(null, item);
	});

//	return component.create();
};
