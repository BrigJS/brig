{
	'targets': [
	{
		'target_name': 'brig',
		'sources': [
			'<!@(tools/genmoc.sh)',
			'src/brig.cpp',
			'src/utils.cpp',
			'src/eventdispatcher/eventdispatcher.cpp',
			'src/qapplication.cpp',
			'src/eventloop.cpp',
			'src/callback.cpp',
			'src/signal_handler.cpp',
			'src/QmlEngine.cpp',
			'src/QmlComponent.cpp',
			'src/QmlContext.cpp',
			'src/QuickItem.cpp',
			'src/DynamicQMetaObjectBuilder.cpp',
			'src/DynamicQObject.cpp',
			'src/QmlTypeBuilder.cpp'
		],
		'include_dirs': [
			"<!(node -e \"require('nan')\")"
		],
		'conditions': [
			['OS=="linux"', {
				'sources': [
					'<!@(tools/internal.sh)',
				],
				'cflags': [
					'-std=c++11',
					'-fPIC',
					'-I./src',
					'<!@(pkg-config --cflags Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				],
				'ldflags': [
					'<!@(pkg-config --libs-only-L --libs-only-other Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				],
				'libraries': [
					'<!@(pkg-config --libs-only-l Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				]
			}],      
			['OS=="mac"', {
				'sources': [
					'<!@(tools/mac-config.sh --internal)',
					'src/eventdispatcher/platform/mac.mm'
				],
				'xcode_settings': {
					'DYLIB_INSTALL_NAME_BASE': '@loader_path/../../node_modules/qt-darwin/Frameworks',
					'LD_RUNPATH_SEARCH_PATHS': [ '@loader_path/../../node_modules/qt-darwin/Frameworks', ],
					'OTHER_CPLUSPLUSFLAGS': [
						'-stdlib=libc++',
						'-std=c++11',
						'-mmacosx-version-min=10.7',
						'-Wno-inconsistent-missing-override',
						'-Woverloaded-virtual',
						'<!@(tools/mac-config.sh --cflags)'
					],
					'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
				},
				'defines': [
					'__MACOSX_CORE__'
				],
				'include_dirs': [
					'build/src',
					'<!@(tools/mac-config.sh --include-dirs QtGui QtCore QtQuick QtQml QtMultimedia QtWidgets)'
				],
				'libraries': [
					'-undefined dynamic_lookup',
					'<!@(tools/mac-config.sh --libs QtGui QtCore QtQuick QtQml QtMultimedia QtWidgets)'
				]
			}]
		]
	}
	]
}
