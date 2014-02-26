# Copyright 2014 Clerk Ma.
#
# You may freely use, modify and/or distribute this file.

# Makefile for Y&Y TeX's utils

CC = cl
LINK = link
RC = rc

CFLAGS=/nologo /c /MD /Gy /O2 /W4
LFLAGS=/nologo
RFLAGS=/nologo


dvidump.exe: dvidump.obj dvidump.res
	$(LINK) $(LFLAGS) dvidump.obj dvidump.res

dvidump.obj: dvidump.c
	$(CC) $(CFLAGS) dvidump.c

dvidump.res: dvidump.rc
	$(RC) $(RFLAGS) dvidump.rc
