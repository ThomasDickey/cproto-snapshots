# $Id: makefile,v 3.2 1994/07/30 21:59:25 tom Exp $
# make-file for CPROTO test directory

THIS	= cproto
TOP	= ../../../../src
include $(TOP)/td_lib/support/td_lib.mk

####### (Standard Lists) #######################################################
REF_FILES=\
	case01.ref	case02.ref	case03.ref	case04.ref\
	case05.ref	case06.ref	case07.ref	case08.ref\
	case09.ref	case10.ref	case11.ref	case12.ref\
	case13.ref	case14.ref

TESTDATA=\
	syntax.c\
	$(COM_FILES)\
	$(REF_FILES)

SCRIPTS	=\
	run\
	run_test.sh	run_test.com

SOURCES	= Makefile descrip.mms $(SCRIPTS) $(TESTDATA)

ALL	=

####### (Standard Productions) #################################################
all:	$(SOURCES) $(ALL) $B/$(THIS) ; @echo 'Type "make run_test"'
clean:				; rm -f $(CLEAN) *.dcl *.err
clobber:	clean		; rm -f $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
run_test:	$(SOURCES)	; $(RUN_TESTS)
lint.out:
lincnt.out:

####### (Details of Productions) ###############################################

$(SOURCES):		; checkout $@
