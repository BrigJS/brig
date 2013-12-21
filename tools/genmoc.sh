#!/bin/bash

OUT=build/src
FILES=`grep -Rl ".*Q_OBJECT" src`

mkdir -p $OUT

for file in $FILES; do

	if [ -e "$file" ]; then
			basepath=$(basename $file)
			name=${basepath%.*}
			mocfile=moc_${name}.cpp
			mocpath=$OUT/"$mocfile"
			moc "$file" -o $mocpath

			echo "$mocpath"
	fi

done
