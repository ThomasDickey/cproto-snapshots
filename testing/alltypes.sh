#!/bin/sh
# $Id: alltypes.sh,v 4.3 2023/02/28 11:55:02 tom Exp $

: "${top_srcdir:=..}"

PATH=..:$PATH

update_file() {
	if [ ! -f "$2" ]
	then
		echo "... missing $1"
	elif [ ! -f "$1" ]
	then
		echo "... initial $1"
		cp "$2" "$1"
	elif cmp -s "$1" "$2"
	then
		echo "... same as $1"
	else
		echo "... changed $1"
		diff -c "$2" "$1"
		[ -w "$2" ] && cp "$2" "$1"
	fi
}

mytemp=`mktemp -d`
trap 'rm -rf "$mytemp"; exit 1' INT QUIT TERM HUP
trap 'rm -rf "$mytemp"; exit 0' EXIT

TYPES=$top_srcdir/keywords.gperf

source=$mytemp/alltypes.c
header=$mytemp/alltypes.h
script=$mytemp/sedscript

rm -f $source
cat >$source <<EOF
#include <complex.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wchar.h>
#include <wctype.h>
EOF
cat $source >$header

cat >$script <<EOF
/^#/d
t
/,/s%,%%g
t data
d
b
:data
EOF
sed -f $script $TYPES | while read typename lex_name comment
do
	forward="fwd_$typename"
	backward="bak_$typename"
	case $typename in
	__ibm128)	continue ;;			# PowerPC only
	_Complex)	typename="float $typename" ;;
	_Imaginary)	typename="float $typename" ;;
	va_list)	typename="$typename *" ;;	# an array...
	esac

	# skip reserved words which are not types
	case $lex_name in
	T_ATTRIBUTE)	continue ;;
	T_AUTO)		continue ;;
	T_ENUM)		continue ;;
	T_EXTENSION)	continue ;;
	T_EXTERN)	continue ;;
	T_INLINE)	continue ;;
	T_REGISTER)	continue ;;
	T_STATIC)	continue ;;
	T_STRUCT)	continue ;;
	T_TYPEDEF)	continue ;;
	T_UNION)	continue ;;
	T_VA_DCL)	continue ;;
	T_VOID)		continue ;;
	T_Imaginary)	continue ;;	# not supported by gcc or clang
	*)
		;;
	esac

	# echo "$typename = $lex_name"
	cat >>$source <<-EOF

	$typename $forward(
		$typename a,
		$typename b)
	{
		return $backward(a, b);
	}

	$typename $backward(a, b)
		$typename a;
		$typename b;
	{
		return $forward(a, b);
	}
	EOF
done

rm -f alltypes.[co]
update_file alltypes.in $source
cat $header >alltypes.c
cproto $source | sed -e "s,$mytemp/,," >>alltypes.c
update_file alltypes.ref alltypes.c

if make -f $top_srcdir/Makefile alltypes.o
then
	echo success
else
	echo failure
fi

rm -f alltypes.[co]
