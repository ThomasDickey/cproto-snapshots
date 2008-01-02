#!/bin/sh
# $Id: buildall.sh,v 4.1 2008/01/01 23:08:40 tom Exp $
# build/test cproto with several combinations of lex/yacc
for LEX in flex /usr/local/bin/*lex*
do
	test -f "$LEX" || continue
	for YACC in 'bison -y' /usr/local/bin/*yacc*
	do
		case $YACC in
		bison*)
			;;
		*)
			test -f "$YACC" || continue
			;;
		esac

		test -f Makefile && make distclean
		export LEX
		export YACC
		echo BUILD $LEX $YACC
		cfg-normal && make check || echo OOPS $LEX $YACC
	done
done
