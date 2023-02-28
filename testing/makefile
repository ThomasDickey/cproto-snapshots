# $Id: makefile,v 4.11 2023/02/25 12:49:38 tom Exp $
# make-file for CPROTO test directory

THIS	= cproto
B	= ..
SHELL	= /bin/sh

####### (Standard Lists) #######################################################
TEST_REF=.

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
	make_dcl.sh\
	testunix.sh\
	run_test.sh	run_test.com

SOURCES	= makefile descrip.mms $(SCRIPTS) $(TESTDATA)

####### (Standard Productions) #################################################
all:	$(SOURCES) $B/$(THIS)
	@echo 'Type "make run_test"'

scripts : $(SOURCES)
	make_bat.sh `grep dos  run_test.txt |sed -e 's/=.*//'`
	make_dcl.sh `grep unix run_test.txt |sed -e 's/=.*//'`

clean:
	- rm -f *.log *.out *.err *.tmp case*.c *.dcl

distclean: clean

realclean: distclean
	- rm -f *.dcl case*.bat

check \
check_errors:	$(SOURCES)
	date >>$@.out
	$(SHELL) -c '(SHELL="$(SHELL)" TEST_REF="$(TEST_REF)" $(SHELL) ./run_test.sh $@ 2>&1)' |tee -a $@.out

check_types:	$(SOURCES)
	date >>$@.out
	$(SHELL) -c '(SHELL="$(SHELL)" TEST_REF="$(TEST_REF)" $(SHELL) ./alltypes.sh $@ 2>&1)' |tee -a $@.out
