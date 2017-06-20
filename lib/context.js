"use strict";

var BrigCore = require('./core');

var Context = module.exports = function(engine) {
	var self = this;

	self.engine = engine || null;
	self.native = new BrigCore.QmlContext(self.engine.native);
};

Context.prototype.append = function() {
};

Context.prototype.setContextProperty = function(propName, value) {
	if (!propName)
		throw new Error('first argument is string');

	this.native.setContextProperty(propName, value);
};

Context.prototype.getContextProperty = function(propName) {
	if (!propName)
		throw new Error('first argument is string');

	return this.native.getContextProperty(propName);
};
