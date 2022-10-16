#!/bin/sh
# $Id: testunix.sh,v 4.5 2022/10/16 22:01:32 tom Exp $
#
# Test one or more given cases by number, creating the VMS test script
# as a side-effect.
#
CPROTO=../cproto
errors=

[ -z "$TEST_REF" ] && TEST_REF=.
mkdir -p $TEST_REF

save_errs() {
	if [ "$TEST_REF" != . ]
	then
		cp "$ERR_FILE" "$TEST_REF/"
	fi
	rm -f "$ERR_FILE"
}

for i in "$@"
do
	case $i in
	check)
		continue
		;;
	check_errors)
		errors=yes
		continue
		;;
	esac
	echo "** Case $i"
	I="case$i"
	./make_dcl.sh "$i"

	TST_FILE="$I.c"
	OUT_FILE="$I.out"
	ERR_FILE="$I.err"
	REF_FILE="$TEST_REF/$I.ref"
	TMP_FILE="$I.tmp"

	rm -f "$TST_FILE" "$OUT_FILE" "$ERR_FILE" "$TMP_FILE"
	cp syntax.c "$TST_FILE"
	chmod +w "$TST_FILE"

	. "./$I.dcl" 2>"$ERR_FILE"

	# When we select either the -a or -t options, CPROTO will edit
	# the input file.  It also writes to the standard output the
	# prototypes that it is extracting.  Since there's only one
	# reference for each test, I simply concatenate the edited file
	# after the test output, for use as a combined reference.
	if ! cmp -s "$TST_FILE" syntax.c
	then
		echo "... edited $TST_FILE ..." >>"$OUT_FILE"
		cat "$TST_FILE" >>"$OUT_FILE"
	fi
	rm -f "$TST_FILE"

	if [ -f "$REF_FILE" ]
	then
		if [ -f "$OUT_FILE" ]
		then
			diff -b -c "$REF_FILE" "$OUT_FILE" |grep -v 'No diff' >"$TMP_FILE"
			if [ -s "$TMP_FILE" ]
			then
				echo '... error'
				cat "$ERR_FILE"
				cat "$TMP_FILE"
			else
				echo '... ok'
				if [ -n "$errors" ]
				then
					if [ "$TEST_REF" != "." ]
					then
						cmp -s "$ERR_FILE" "$TEST_REF/$ERR_FILE" || cat "$ERR_FILE"
					else
						cat "$ERR_FILE"
					fi
				fi
				rm -f "$OUT_FILE" "$TMP_FILE" "$ERR_FILE"
			fi
		else
			echo "? no output $I"
		fi
	else
		echo "...saving reference for $i"
		mv "$OUT_FILE" "$REF_FILE"
		save_errs
		rm -f "$OUT_FILE" "$TMP_FILE" "$ERR_FILE"
	fi
done
# vile: ts=4 sw=4
