#!/bin/bash

NODE_QT_MOD=../../node_modules/qt-darwin/Frameworks
QMAKE=`which qmake`
if [ "$QMAKE" == "" ]; then
	QT_LIBS_PATH=$NODE_QT_MOD
else
	QT_LIBS_PATH=`qmake -query QT_INSTALL_LIBS`
fi

if [ "$1" == "--include-dirs" ]; then
	shift 1

	for i in $@; do
		INCPATH=$QT_LIBS_PATH/${i}.framework/Versions/Current/Headers

		if [ -d "$INCPATH" ]; then
			echo $INCPATH
		fi
	done

elif [ "$1" == "--cflags" ]; then

	if [ -d "$NODE_QT_MOD" ]; then
		QT_LIBS_PATH=$NODE_QT_MOD
	fi

	echo -F$QT_LIBS_PATH

elif [ "$1" == "--libs" ]; then
	shift 1

	if [ -d "$NODE_QT_MOD" ]; then
		QT_LIBS_PATH=$NODE_QT_MOD
	fi

	echo -F$QT_LIBS_PATH

	for i in $@; do
		echo $QT_LIBS_PATH/${i}.framework
	done

elif [ "$1" == '--internal' ]; then
	OUT=build/src
	QTVERSION=`qmake -query QT_VERSION`
	FILES="
		$QT_LIBS_PATH/QtGui.framework/Versions/Current/Headers/$QTVERSION/QtGui/qpa/qwindowsysteminterface.h
		$QT_LIBS_PATH/QtCore.framework/Versions/Current/Headers/$QTVERSION/QtCore/private/qmetaobjectbuilder_p.h
	"

	mkdir -p $OUT/internal

	for file in $FILES; do

		if [ -e "$file" ]; then
			basepath=$(basename $file)
			out=$OUT/internal/"$basepath"
			cp -a "$file" $out
			echo "$out"
		fi

	done
fi
