#!/bin/sh
# $Id: run_test.sh,v 3.0 1990/03/30 09:29:58 ste_cm Rel $
echo '** '`date`
CPROTO=../cp8
for i in 01 02 03 04 05 06 07 09 10 11 12 13 14
do
echo '** Case '$i
rm -f case$i.out
( . ./case$i.com ) 2>/dev/null
#?diff  case$i.out case$i.ref
diff  -b case$i.out case$i.ref
#?rm -f case$i.out
done
