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

utils: dvidump.exe checktfm.exe

# For dvidump.
dvidump.exe: dvidump.obj dvidump.res
	$(LINK) $(LFLAGS) dvidump.obj dvidump.res

dvidump.obj: dvidump.c
	$(CC) $(CFLAGS) dvidump.c

dvidump.res: dvidump.rc
	$(RC) $(RFLAGS) dvidump.rc

# For checktfm.
checktfm.exe: checktfm.obj checktfm.res
	$(LINK) $(LFLAGS) checktfm.obj checktfm.res

checktfm.obj: checktfm.c
	$(CC) $(CFLAGS) checktfm.c

checktfm.res: checktfm.rc
	$(RC) $(RFLAGS) checktfm.rc

