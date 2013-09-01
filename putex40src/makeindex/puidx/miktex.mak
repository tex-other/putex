## miktex.mak (MakeIndex docs)
## Time-stamp: "2000-09-09 11:49:15 mik"

!include ..\..\miktex.inc

cc_options = $(cflags) $(cdebug) $(cvarsdll) $(miktexvars) -DUSE_MIKTEX_EXIT \
		-DOS_WIN32 -DIBM_PC_MICROSOFT -I. -I$(libdir) \
		-DDBCS

.c{$(outdir)\}.obj:
	$(cc) $(cc_options) -Fo$(outdir)\ $<

objects = \
	$(outdir)\genind.obj \
	$(outdir)\mkind.obj \
	$(outdir)\qsort.obj \
	$(outdir)\scanid.obj \
	$(outdir)\scanst.obj \
	$(outdir)\sortid.obj \
	$(outdir)\dbcs.obj \

binaries = $(outdir)\puidx.exe

all: common-all

install: common-install install-binaries

qrt: common-qrt

# _____________________________________________________________________________
#
# makeindex
# _____________________________________________________________________________

makeindex_objects = $(objects) $(miktex_lib) $(outdir)\makeindex.res

$(outdir)\puidx.exe: $(outdir) $(makeindex_objects)
	$(link) $(conlflags) $(ldebug) -out:$@ $(makeindex_objects) $(conlibsdll)

$(outdir)\makeindex.res: $(outdir) makeindex.rc $(libdir)\miktex.version \
				$(libdir)\version.h
	$(rc) $(rcflags) $(rcvars) -I$(libdir) -Fo$@ makeindex.rc

# _____________________________________________________________________________
#
# clean-up
# _____________________________________________________________________________

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________

!include ..\..\common-dependencies.inc

depend: genind.c mkind.c qsort.c scanid.c scanst.c sortid.c
	$(MAKEDEPEND2) -DOS_WIN32 -DIBM_PC_MICROSOFT $**

# DO NOT DELETE

$(outdir)\genind.obj: mkind.h ..\..\lib\miktex.version ..\..\lib\version.h
$(outdir)\genind.obj: genind.h
$(outdir)\mkind.obj: mkind.h ..\..\lib\miktex.version ..\..\lib\version.h
$(outdir)\mkind.obj: ..\..\lib\miktex.h ..\..\lib\c4plib.h
$(outdir)\qsort.obj: mkind.h ..\..\lib\miktex.version ..\..\lib\version.h
$(outdir)\scanid.obj: mkind.h ..\..\lib\miktex.version ..\..\lib\version.h
$(outdir)\scanid.obj: scanid.h
$(outdir)\scanst.obj: mkind.h ..\..\lib\miktex.version ..\..\lib\version.h
$(outdir)\scanst.obj: scanst.h
$(outdir)\sortid.obj: mkind.h ..\..\lib\miktex.version ..\..\lib\version.h
$(outdir)\dbcs.obj: dbcs.h
