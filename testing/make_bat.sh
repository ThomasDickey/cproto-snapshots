#!/bin/sh
# $Id: make_bat.sh,v 3.4 1994/08/13 13:26:23 tom Exp $
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
@echo on
echo Testing case $i
if exist $I.out erase $I.out
copy syntax.c $I.c
EOF
		grep "CASE[ 	]$i" run_test.txt | \
			sed -e 's/^.*=/..\\CPROTO /' \
				-e s/\$/\ -o$I.out\ $I.c/ >>$I.bat
		cat >>$I.bat <<EOF
if not exist $I.ref goto saveit
    fc/L/t/n/w $I.out $I.ref
    if not errorlevel 0 erase $I.out
    goto end
:saveit
    ren $I.out $I.ref
:end
    erase $I.c
EOF
		flip -m $I.bat
	fi
done
