
var child_process = require('child_process');

function setupAddon() {
	try {
		var BrigCore = require('./lib/core');
	} catch(err) {
		console.log('There is no bulit-in binary, starting to build immediately.');
		var gyp = child_process.spawn('node-gyp', [ 'rebuild' ], { stdio: 'inherit' });
		gyp.on('exit', function(code) {
			process.exit(code);
		});
	}
}
console.log('PLATFORM', process.platform);
if (process.platform === 'darwin') {
	console.log('Downloading Darwin backend ...');
	var downloadBackend = child_process.spawn('npm', [ 'install', 'qt-darwin' ], { stdio: 'inherit' });
	downloadBackend.on('error', function() {
		console.error('Failed to download qt-darwin, using Qt installed on your system instead.');
		setupAddon();
	});

	downloadBackend.on('exit', function(code) {
		setupAddon();
	});
} else {
	setupAddon();
}
