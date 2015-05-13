
var child_process = require('child_process');

try {
	var BrigCore = require('./lib/core');
} catch(err) {
	var gyp = child_process.spawn('node-gyp', [ 'rebuild' ], { stdio: 'inherit' });
	gyp.on('exit', function(code) {
		process.exit(code);
	});
}
