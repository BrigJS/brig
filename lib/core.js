var os = require('os');
var path = require('path');
var binary = require('node-pre-gyp');
var binding_path = binary.find(path.resolve(path.join(__dirname, '..', 'package.json')));

var binding;
try {
	binding = require(binding_path);
} catch(e) {
	binding = require(path.join(__dirname, '..', 'build', 'Release', 'brig.node'));
}

module.exports = binding;
