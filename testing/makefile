# $Id: makefile,v 3.5 1994/08/04 00:04:56 tom Exp $
# make-file for CPROTO test directory

THIS	= cproto
B	= ..

####### (Standard Lists) #######################################################
REF_FILES=\
	case01.ref	case02.ref	case03.ref	case04.ref\
	case05.ref	case06.ref	case07.ref	case08.ref\
	case09.ref	case10.ref	case11.ref	case12.ref\
	case13.ref	case14.ref	case15.ref	case16.ref\
	case17.ref	case18.ref	case19.ref	case20.ref\
	case21.ref

TESTDATA=\
	run_test.txt\
	syntax.c\
	$(COM_FILES)\
	$(REF_FILES)

SCRIPTS	=\
	run_test.sh	run_test.com

SOURCES	= Makefile descrip.mms $(SCRIPTS) $(TESTDATA)

####### (Standard Productions) #################################################
all:	$(SOURCES) $B/$(THIS)
	@echo 'Type "make run_test"'

clean:
	rm -f *.log *.out *.dcl *.err *.tmp case*.c

run_test:	$(SOURCES)
	date >>$@.out
	sh -c '(run_test.sh 2>&1)' |tee -a $@.out
