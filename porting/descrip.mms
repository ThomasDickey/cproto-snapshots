# $Id: descrip.mms,v 3.3 1994/08/12 23:47:49 tom Exp $
# VAX/VMS "mms" script for CPROTO

THIS = cproto

#### Start of system configuration section. ####

LEX	= flex
YACC	= bison /fixed_outfiles

DEFINES	=
CFLAGS	= /Include=([]) $(DEFINES)

#### End of system configuration section. ####

DOC_FILES = \
	README \
	CHANGES \
	$(THIS).1

H_FILES = \
	yyerror.c \
	system.h \
	$(THIS).h \
	patchlev.h \
	semantic.h \
	symbol.h

C_FILES = \
	$(THIS).c \
	lintlibs.c \
	semantic.c \
	strkey.c \
	strstr.c \
	symbol.c \

AUX_FILES = \
	mkdirs.sh \
	lex.l \
	grammar.y

LEX_YY	= yylex
Y_TAB	= y_tab

JUNK =	\
	$(LEX_YY).c \
	$(Y_TAB).c

OBJECTS = \
	$(THIS).obj \
	lintlibs.obj \
	semantic.obj \
	strkey.obj \
	strstr.obj \
	symbol.obj \
	$(Y_TAB).obj

SOURCES = $(DOC_FILES) $(H_FILES) $(C_FILES) $(AUX_FILES)

all : $(THIS).exe
	@ write sys$output "** produced $?"

$(THIS).exe : $(OBJECTS)
	$(LINK)/exec=$(THIS) $(OBJECTS),sys$library:vaxcrtl/lib

$(Y_TAB).c : grammar.y
	$(YACC) grammar.y

$(LEX_YY).c : lex.l
	$(LEX) lex.l

clean :
	- if f$search("*.obj").nes."" then dele/nolog *.obj;*
	- if f$search("*.lis").nes."" then dele/nolog *.lis;*
	- if f$search("*.log").nes."" then dele/nolog *.log;*
	- if f$search("*.map").nes."" then dele/nolog *.map;*
	- if f$search("$(Y_TAB).c").nes."" then dele/nolog $(Y_TAB).c;*
	- if f$search("$(LEX_YY).c").nes."" then dele/nolog $(LEX_YY).c;*

clobber : clean
	- if f$search("$(THIS).exe").nes."" then dele/nolog $(THIS).exe;*

cproto.obj   : cproto.h system.h patchlev.h
lintlibs.obj : cproto.h system.h semantic.h symbol.h
semantic.obj : cproto.h system.h semantic.h
strkey.obj   : cproto.h system.h
symbol.obj   : cproto.h system.h symbol.h
$(Y_TAB).obj : cproto.h system.h symbol.h semantic.h $(LEX_YY).c yyerror.c
