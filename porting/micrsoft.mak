# $Id: micrsoft.mak,v 3.1 1994/07/25 23:11:37 tom Exp $
#
# Microsoft C makefile for C prototype generator

# Define MSDOS for MS-DOS compatibility.
# Define TURBO_CPP to pipe the input through the Turbo C preprocessor.
DEFINES = -DMSDOS

LEX = lex
YACC = yacc
CC = cl
CFLAGS = -AC $(DEFINES)
LIBS = \c7\lib\setargv.obj
LDFLAGS = /F 1000

DIST1 = README CHANGES cproto.man
DIST2 = cproto.1 borland.mak micrsoft.mak makefile.in lex.l grammar.y
DIST3 = system.h cproto.h patchlev.h semantic.h symbol.h
DIST4 = cproto.c lintlibs.c popen.c semantic.c strkey.c strstr.c symbol.c

OBJECTS = cproto.obj lintlibs.obj getopt.obj semantic.obj strkey.obj symbol.obj y_tab.obj popen.obj

all: cproto.exe

cproto.exe: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) $(LDFLAGS)

y_tab.obj: y_tab.c lex_yy.c system.h cproto.h symbol.h semantic.h
	$(CC) $(CFLAGS) -c $*.c

y_tab.c: grammar.y
	$(YACC) grammar.y

lex_yy.c: lex.l
	$(LEX) lex.l

cproto.man: cproto.1
	cawf -man $*.1 | bsfilt - >$*.man

clean:
	erase *.obj
	erase *.bak
	erase *.log
	erase lex_yy.c
	erase y_tab.c
	erase cproto.exe

ci:
	ci -u3 $(DIST2) $(DIST3)
	ci -u3 $(DIST4)

shar:
	shar $(DIST1) $(DIST2) >cproto.sh1
	shar $(DIST3) $(DIST4) >cproto.sh2

# DO NOT DELETE THIS LINE -- make depend depends on it.

cproto.obj: system.h cproto.h
lintlibs.obj: system.h cproto.h symbol.h semantic.h
popen.obj: system.h cproto.h
semantic.obj: system.h cproto.h symbol.h semantic.h
strkey.obj: cproto.h system.h
strstr.obj: system.h
symbol.obj: system.h cproto.h symbol.h
