#!/bin/sh
# $Id: testunix.sh,v 3.2 1994/07/30 22:09:04 tom Exp $
#
# Test one or more given cases by number, creating the VMS test script
# as a side-effect.
#
CPROTO=../cproto
for i in $*
do
	echo '** Case '$i
	I="case$i"
	./make_dcl.sh $i

	rm -f $I.c $I.out $I.err $I.tmp
	cp syntax.c $I.c
	chmod +w $I.c

	( . ./$I.dcl ) 2>$I.err
	if [ -f $I.ref ]
	then
		if [ -f $I.out ]
		then
			diff -b -c $I.ref $I.out >$I.tmp
			if [ -s $I.tmp ]
			then
				cat $I.tmp
			else
				echo '... ok'
				rm -f $I.out $I.tmp $I.err
			fi
		elif ( cmp -s $I.c syntax.c )
		then
			echo '?? no output'
			cat $I.err
		else
			echo edited $I.c
		fi
	else
		echo '...saving reference for '$i
		mv $I.out $I.ref
	fi
done
