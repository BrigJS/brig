{
	'targets': [
	{
		'target_name': 'brig',
		'sources': [
			'<!@(tools/internal.sh)',
			'<!@(tools/genmoc.sh)',
			'src/brig.cpp',
			'src/utils.cpp',
			'src/eventdispatcher/eventdispatcher.cpp',
			'src/qapplication.cpp',
			'src/eventloop.cpp',
			'src/qmlengine.cpp',
			'src/QmlComponent.cpp'
		],
		'conditions': [
			['OS=="linux"', {
				'cflags': [
					'-std=c++11',
					'-I./src',
					'<!@(pkg-config --cflags Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				],
				'ldflags': [
					'<!@(pkg-config --libs-only-L --libs-only-other Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				],
				'libraries': [
					'<!@(pkg-config --libs-only-l Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				]
			}]        
		]
	}
	]
}
