# $Id: makefile,v 3.1 1992/10/16 14:35:34 dickey Exp $
# make-file for CPROTO test directory

THIS	= cproto
TOP	= ../../../../src
include $(TOP)/td_lib/support/td_lib.mk

####### (Standard Lists) #######################################################
COM_FILES=\
	case1.com	case2.com	case3.com	case4.com\
	case5.com	case6.com	case7.com	case8.com\
	case9.com	case10.com	case11.com	case12.com\
	case13.com	case14.com

REF_FILES=\
	case1.ref	case2.ref	case3.ref	case4.ref\
	case5.ref	case6.ref	case7.ref	case8.ref\
	case9.ref	case10.ref	case11.ref	case12.ref\
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
clean:				; rm -f $(CLEAN)
clobber:	clean		; rm -f $(ALL)
destroy:			; $(DESTROY)
sources:	$(SOURCES)
run_test:	$(SOURCES)	; $(RUN_TESTS)
lint.out:
lincnt.out:

####### (Details of Productions) ###############################################

$(SOURCES):		; checkout $@
