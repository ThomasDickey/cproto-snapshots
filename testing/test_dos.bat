@echo off
rem $Id: test_dos.bat,v 3.3 1994/08/15 00:39:54 tom Exp $
erase run_test.out
for %%n in ( case*.bat ) do call %%n
