$! $Id: run_test.com,v 3.0 1991/09/12 09:17:49 ste_cm Rel $
$! Perform all regression-tests for CPROTO on VAX/VMS
$
$	verify	= F$VERIFY(0)
$	path	:= 'F$ENVIRONMENT("DEFAULT")
$	path	:= "''F$EXTRACT(0,F$LENGTH(path)-1,path)'.-.BIN]"
$	CPROTO	:== "$''path'cproto.exe"
$	CPROTO1	:== "$''path'cproto1.exe"
$
$ loop:
$	ref_name = F$SEARCH("*.ref;",1)
$	if ref_name .eqs. ""
$	then
$		verify = F$VERIFY(verify)
$		exit
$	endif
$
$	com_name = F$EXTRACT(0, F$LOCATE(".REF;", ref_name), ref_name)
$	case_num = F$PARSE(ref_name,,, "NAME", "SYNTAX_ONLY")
$	case_num = F$EXTRACT(4, F$LENGTH(case_num), case_num)
$	write sys$output "** ",ref_name
$
$	if F$SEARCH("''com_name'.com;", 2) .eqs. ""
$	then
$		write sys$output "?? no script for ", ref_name
$	else
$		if F$SEARCH("''com_name'.out") .nes. ""
$		then
$			delete 'com_name.out;*
$		endif
$		@'com_name
$		if F$SEARCH("''com_name'.out") .eqs. ""
$		then
$			write sys$output "?? no output from test"
$		else
$			diff/out='com_name 'com_name.out 'ref_name
$			if $severity .ne. 1
$			then
$				write sys$output "?? test failed"
$				type 'com_name.dif
$			endif
$			delete 'com_name.out;,'com_name.dif;
$		endif
$	endif
$	goto loop
