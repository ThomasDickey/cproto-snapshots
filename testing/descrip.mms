# $Header: /users/source/archives/cproto.vcs/testing/RCS/descrip.mms,v 3.1 1991/06/07 17:32:06 tom Exp $
# VMS make-file for CPROTO test directory
#

####### (Development) ##########################################################

####### (Command-line Options) #################################################

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
	run_tests.sh	run_tests.com

SOURCES	= Makefile descrip.mms $(SCRIPTS) $(TESTDATA)

ALL	= $(SCRIPTS)

####### (Standard Productions) #################################################
all :	$(ALL) [-.bin]$(THIS).exe [-.bin]$(THIS)1.exe
	@ write sys$output "Type ""mms run_tests""

clean :
	@- write sys$output "** made $@"
clobber :	clean
	@- write sys$output "** made $@"
destroy :
	@- remove -vf *.*;*
run_tests :	$(SCRIPTS)
	@run_tests

####### (Details of Productions) ###############################################
