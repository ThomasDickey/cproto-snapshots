#!/bin/sh
# $Id: make_dcl.sh,v 3.3 1994/07/30 22:10:18 tom Exp $
#
# This makes a special ".dcl" file that can be executed from either UNIX or
# VMS.  We auto-generate the script (with this name) to avoid confusion on
# MSDOS, which thinks ".com" files are even more special.
#
for i in $*
do
	i="`echo $i | sed -e 's/[a-z_.]//g'`"
	I="case$i"
	if [ ! -f $I.dcl ]
	then
		grep "CASE[ 	]$i" run_test.txt | \
			sed -e 's/^.*=/$CPROTO /' \
				-e s/\$/\ -o$I.out\ $I.c/ >$I.dcl
		chmod 755 $I.dcl
	fi
done
