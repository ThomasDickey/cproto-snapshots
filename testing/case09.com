$CPROTO -o case09.out -c -f3 -v -P"int func(\n\ta,\n\tb\n\t)" syntax.c
