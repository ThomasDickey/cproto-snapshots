#!/bin/sh
# $Id: make_dcl.sh,v 3.5 1994/08/13 00:22:18 tom Exp $
#
# This makes a special ".dcl" file that can be executed from either UNIX or
# VMS.  We auto-generate the script (with this name) to avoid confusion on
# MSDOS, which thinks ".com" files are even more special.
#
for i in $*
do
	i="`echo $i | sed -e 's/[A-Za-z_.]//g'`"
	I="case$i"
	if [ -n "$i" -a ! -f $I.dcl ]
	then
		echo '** making '$I.dcl
		grep "CASE[ 	]$i" run_test.txt | \
			sed -e 's/^.*=/$CPROTO /' \
				-e s/\$/\ -o$I.out\ $I.c/ >$I.dcl
		chmod 755 $I.dcl
	fi
done
