#!/bin/sh
# $Id: run_test.sh,v 4.6 2020/07/16 21:02:07 tom Exp $
#
# This test-script assumes that we can reproduce (except for minor whitespace)
# the reference files listed in run_test.txt
#
echo "** `date`"
grep 'CASE.*unix' run_test.txt | sed -e 's/^[^0-9]*//'  -e 's/[ 	].*//' | while read -r item
do
	$SHELL ./testunix.sh "$@" "$item"
done
