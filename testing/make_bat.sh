#!/bin/sh
# $Id: make_bat.sh,v 3.10 1994/08/16 23:52:52 tom Exp $
#
# This makes a special ".bat" file for testing CPROTO on MSDOS.
# It won't work properly if /bin/echo tries to expand the backslash sequences.
# (This is the case on SunOS 4.1.x).
#
# The MSDOS 'fc' program doesn't return an exit code for differing files.
# We have to examine the output log...
if [ -f /bin/echo ]
then
	ECHO=/bin/echo
else
	ECHO=echo
fi
for i in $*
do
	i="`echo $i | sed -e 's/[A-Za-z_.]//g'`"
	I="case$i"
	if [ -n "$i" -a ! -f $I.bat ]
	then
		echo '** making '$I.bat
		cat >$I.bat <<EOF
@echo off
echo Testing case $i
echo Testing case $i >>run_test.out
if exist $I.out erase $I.out
if exist $I.dif erase $I.dif
copy syntax.c $I.c
EOF
		CASE="`grep \"CASE[ 	]$i\" run_test.txt`"
		OPTS=`$ECHO "$CASE"|sed -e 's/^.*=/CPROTO/'`
		$ECHO "..\\$OPTS -o$I.out -O$I.err $I.c" >>$I.bat
		TEST=`$ECHO $OPTS | sed -e 's/-[at]//'`
		if [ ".$OPTS" !=  ".$TEST" ]
		then
		cat >>$I.bat <<EOF
echo ... edited $I.c ... >>$I.out
type $I.c >>$I.out
EOF
		fi
		cat >>$I.bat <<EOF
if not exist $I.ref goto saveit
    fc/L/n/w $I.out $I.ref >>run_test.out
    if exist $I.out erase $I.out
    if exist $I.err erase $I.err
    goto end
:saveit
    ren $I.out $I.ref
:end
    erase $I.c
EOF
		chmod 644 $I.bat
		flip -m $I.bat
	fi
done
