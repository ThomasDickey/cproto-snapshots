# $Id: makefile,v 3.3 1994/07/31 00:30:54 tom Exp $
# make-file for CPROTO test directory

THIS	= cproto
TOP	= ../../../../src
include $(TOP)/td_lib/support/td_lib.mk

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

ALL	=

####### (Standard Productions) #################################################
all:	$(SOURCES) $(ALL) $B/$(THIS) ; @echo 'Type "make run_test"'
clean:				; rm -f $(CLEAN) *.dcl *.err case*.c
clobber:	clean		; rm -f $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
run_test:	$(SOURCES)	; $(RUN_TESTS)
lint.out:
lincnt.out:

####### (Details of Productions) ###############################################

$(SOURCES):		; checkout $@
