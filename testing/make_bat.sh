#!/bin/sh
# $Id: make_bat.sh,v 3.7 1994/08/14 23:57:39 tom Exp $
#
# This makes a special ".bat" file for testing CPROTO on MSDOS.
#
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
if exist $I.out erase $I.out
if exist $I.dif erase $I.dif
copy syntax.c $I.c
EOF
		grep "CASE[ 	]$i" run_test.txt | \
			sed -e 's/^.*=/..\\CPROTO /' \
				-e s/\$/\ -o$I.out\ -O$I.err\ $I.c/ >>$I.bat
		cat >>$I.bat <<EOF
if not exist $I.ref goto saveit
    fc/L/n/w $I.out $I.ref >$I.dif
    if not errorlevel 0 goto end
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
