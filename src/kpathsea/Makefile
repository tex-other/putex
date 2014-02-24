# makefile for Y&Y TeX's kpathsea library
# Copyright (C) 2014 Clerk Ma
#
# run: nmake

CC=cl

LIBNAME=libkpathsea.lib
SONAME=libhpdf.dll

CFLAGS= -nologo -O2 -I"../"
CFLAGS_EXE=-Fe

OBJS= \
	kpathsea\absolute.obj \
	kpathsea\access.obj \
	kpathsea\atou.obj \
	kpathsea\cnf.obj \
	kpathsea\concat.obj \
	kpathsea\concat3.obj \
	kpathsea\concatn.obj \
	kpathsea\db.obj \
	kpathsea\debug.obj \
	kpathsea\dir.obj \
	kpathsea\elt-dirs.obj \
	kpathsea\expand.obj \
	kpathsea\extern-fname.obj \
	kpathsea\file-p.obj \
	kpathsea\find-suffix.obj \
	kpathsea\fn.obj \
	kpathsea\fontmap.obj \
	kpathsea\getopt1.obj \
	kpathsea\hash.obj \
	kpathsea\kdefault.obj \
	kpathsea\knj.obj \
	kpathsea\kpathsea.obj \
	kpathsea\kpsestat.obj \
	kpathsea\kpasewhich.obj \
	kpathsea\line.obj \
	kpathsea\magstep.obj \
	kpathsea\make-suffix.obj \
	kpathsea\path-elt.obj \
	kpathsea\pathsearch.obj \
	kpathsea\proginit.obj \
	kpathsea\progname.obj \
	kpathsea\readable.obj \
	kpathsea\readlink.obj \
	kpathsea\rm-suffix.obj \
	kpathsea\str-list.obj \
	kpathsea\str-llist.obj \
	kpathsea\tex-file.obj \
	kpathsea\tex-glyph.obj \
	kpathsea\tex-hush.obj \
	kpathsea\tex-make.obj \
	kpathsea\tilde.obj \
	kpathsea\uppercasify.obj \
	kpathsea\variable.obj \
	kpathsea\vesion.obj \
	kapthsea\win32lib.obj \
	kpathsea\xbasename.obj \
	kpathsea\xcalloc.obj \
	kpathsea\xdirname.obj \
	kpathsea\xfopen.obj \
	kpathsea\xfseek.obj \
	kpathsea\xftell.obj \
	kpathsea\xgetcwd.obj \
	kpathsea\xmalloc.obj \
	kpathsea\xopendir.obj \
	kpathsea\xputenv.obj \
	kpathsea\xrelloc.obj \
	kpathsea\xstat.obj \
	kpathsea\xstrdup.obj

PROGRAMS = \
	kpathsea\kpsewhich.exe \
	kpathsea\kpsestat.exe \
	kpathsea\kpseaccess.exe \
	kpathsea\kpsereadlink.exe

clean:
	if exist kpathsea\*.obj del kpathsea\*.obj
	if exist kpathsea\*.exe del kpathsea\*.exe
	if exist kpathsea\*.lib del kpathsea\*.lib
	if exist kpathsea\*.dll del kpathsea\*.dll
