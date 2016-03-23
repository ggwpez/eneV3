#!/bin/sh
set -me

for f in `find . -name '*.out'`
do
	echo "* Test: " $f

	if $f | grep 'ERR'; then
		echo "*** FAILED"
		exit 255
	else
		echo "* Passed"
	fi

done

exit 0
