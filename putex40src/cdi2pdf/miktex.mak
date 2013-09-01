## miktex.mak: dvipdfmx
##
## Copyright (C) 1998-2003 Christian Schenk <cs@miktex.org>
## 
## This file is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation; either version 2, or (at your
## option) any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this file; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

miktex_cc_no_runtime_checks = 1
miktex_cc_no_warnings = 1

!include ..\miktex.inc

objects =	\
	$(outdir)\agl.obj \
	$(outdir)\cff.obj \
	$(outdir)\cff_dict.obj \
	$(outdir)\cid.obj \
	$(outdir)\cidtype0.obj \
	$(outdir)\cidtype2.obj \
	$(outdir)\cmap.obj \
	$(outdir)\cmap_parse.obj \
	$(outdir)\colorsp.obj \
	$(outdir)\cs_type2.obj \
	$(outdir)\dpxcrypt.obj \
	$(outdir)\dpxutil.obj \
	$(outdir)\dvi.obj \
	$(outdir)\dvipdfmx.obj \
	$(outdir)\encodings.obj \
	$(outdir)\epdf.obj \
	$(outdir)\error.obj \
	$(outdir)\fontmap.obj \
	$(outdir)\htex.obj \
	$(outdir)\jpegimage.obj \
	$(outdir)\mem.obj \
	$(outdir)\mfileio.obj \
	$(outdir)\mpost.obj \
	$(outdir)\numbers.obj \
	$(outdir)\pdfdev.obj \
	$(outdir)\pdfdoc.obj \
	$(outdir)\pdfencrypt.obj \
	$(outdir)\pdfobj.obj \
	$(outdir)\pdfparse.obj \
	$(outdir)\pdfresource.obj \
	$(outdir)\pdfspecial.obj \
	$(outdir)\pkfont.obj \
	$(outdir)\pngimage.obj \
	$(outdir)\psimage.obj \
	$(outdir)\psspecial.obj \
	$(outdir)\pst.obj \
	$(outdir)\pst_obj.obj \
	$(outdir)\sfnt.obj \
	$(outdir)\subfont.obj \
	$(outdir)\t1crypt.obj \
	$(outdir)\tfm.obj \
	$(outdir)\thumbnail.obj \
	$(outdir)\tpic.obj \
	$(outdir)\tt_aux.obj \
	$(outdir)\tt_build.obj \
	$(outdir)\tt_cmap.obj \
	$(outdir)\tt_gsub.obj \
	$(outdir)\tt_table.obj \
	$(outdir)\ttf.obj \
	$(outdir)\type0.obj \
	$(outdir)\type1.obj \
	$(outdir)\type1c.obj \
	$(outdir)\unicode.obj \
	$(outdir)\vf.obj \

sources =	\
	agl.c \
	cff.c \
	cff_dict.c \
	cid.c \
	cidtype0.c \
	cidtype2.c \
	cmap.c \
	cmap_parse.c \
	colorsp.c \
	cs_type2.c \
	dpxcrypt.c \
	dpxutil.c \
	dvi.c \
	dvipdfmx.c \
	encodings.c \
	epdf.c \
	error.c \
	fontmap.c \
	htex.c \
	jpegimage.c \
	mem.c \
	mfileio.c \
	mpost.c \
	numbers.c \
	pdfdev.c \
	pdfdoc.c \
	pdfencrypt.c \
	pdfobj.c \
	pdfparse.c \
	pdfresource.c \
	pdfspecial.c \
	pkfont.c \
	pngimage.c \
	psimage.c \
	psspecial.c \
	pst.c \
	pst_obj.c \
	sfnt.c \
	subfont.c \
	t1crypt.c \
	tfm.c \
	thumbnail.c \
	tpic.c \
	tt_aux.c \
	tt_build.c \
	tt_cmap.c \
	tt_gsub.c \
	tt_table.c \
	ttf.c \
	type0.c \
	type1.c \
	type1c.c \
	unicode.c \
	vf.c \

cc_options = \
	$(cflags) $(cdebug) $(cvarsdll) $(cflagspackfunc) $(miktexvars) \
		-DHAVE_BASENAME \
		-DHAVE_CONFIG_H \
		-DCMAP_ADOBE_IDENTITY_UCS2=\"Adobe-Identity-UCS2\" \
		-DUSE_MIKTEX_EXIT \
		-I$(gnulibdir) \
		-I$(kpslibdir) \
		-I$(libdir) \
		-I$(pnglibdir) \
		-I$(zlibdir) \

.c{$(outdir)\}.obj:
	$(cc) $(cc_options) -Fo$(outdir)\ $<

binaries = $(outdir)\cdi2pdf.exe

all: common-all $(binaries)

install: common-install install-binaries

qrt: common-qrt

# -----------------------------------------------------------------------------
# cdi2pdf
# -----------------------------------------------------------------------------


$(outdir)\cdi2pdf.exe: \
		$(outdir) $(objects) $(outdir)\cdi2pdf.res \
		$(miktex_lib) $(gnu_lib) $(kps_lib) $(zlib_lib) $(png_lib) \
		$(texmf_lib) #dvipdfmx.prf
	$(link) $(conlflags) $(ldebug) \
!if $(miktex_prf) > 0
#		-order:@dvipdfmx.prf \
!endif
		-out:$@ \
		$(objects) $(outdir)\cdi2pdf.res \
		$(gnu_lib) $(miktex_lib) $(kps_lib) $(zlib_lib) $(png_lib) \
		$(texmf_lib) $(conlibsdll)
		
		copy $(outdir)\cdi2pdf.exe c:\texmf\miktex\bin

$(outdir)\cdi2pdf.res: $(outdir) cdi2pdf.rc
	$(rc) $(rcflags) $(rcvars) -I$(libdir) -Fo$@ cdi2pdf.rc

# -----------------------------------------------------------------------------
# cdi2pdf
# -----------------------------------------------------------------------------

config.h:  __$$@ $*_h.ch
	tie -m $@ __$@ $*_h.ch

dvipdfmx.c: __$$@ $*_c.ch
	tie -m $@ __$@ $*_c.ch

dvicodes.h:  __$$@ $*_h.ch
	tie -m $@ __$@ $*_h.ch

dvi.c: __$$@ $*_c.ch
	tie -m $@ __$@ $*_c.ch

fontmap.h:  __$$@ $*_h.ch
	tie -m $@ __$@ $*_h.ch

fontmap.c: __$$@ $*_c.ch
	tie -m $@ __$@ $*_c.ch

pdfparse.h:  __$$@ $*_h.ch
	tie -m $@ __$@ $*_h.ch

pdfparse.c: __$$@ $*_c.ch
	tie -m $@ __$@ $*_c.ch

tt_aux.c: __$$@ $*_c.ch
	tie -m $@ __$@ $*_c.ch

# -----------------------------------------------------------------------------
# clean-up
# -----------------------------------------------------------------------------

mostlyclean: common-mostlyclean

clean: common-clean mostlyclean

distclean: common-distclean clean

realclean: common-realclean distclean

# -----------------------------------------------------------------------------
# dependencies
# -----------------------------------------------------------------------------

!include ..\common-dependencies.inc

depend: $(sources)
	$(MAKEDEPEND1) -I..\zlib -I..\libkps -DHAVE_BASENAME -DHAVE_ZLIB $**

# DO NOT DELETE

$(outdir)\agl.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\agl.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\agl.obj: error.h mfileio.h numbers.h agl.h dpxutil.h
$(outdir)\cff.obj: mem.h error.h system.h ..\lib\miktex.h
$(outdir)\cff.obj: ..\lib\miktex-features.h ..\libgnu\gnu-miktex.h
$(outdir)\cff.obj: ..\libkps\web2c-miktex.h mfileio.h numbers.h cff_limits.h
$(outdir)\cff.obj: cff_types.h cff_stdstr.h cff_dict.h cff.h
$(outdir)\cff_dict.obj: error.h system.h ..\lib\miktex.h
$(outdir)\cff_dict.obj: ..\lib\miktex-features.h ..\libgnu\gnu-miktex.h
$(outdir)\cff_dict.obj: ..\libkps\web2c-miktex.h mem.h mfileio.h numbers.h
$(outdir)\cff_dict.obj: cff_types.h cff_limits.h cff_dict.h cff.h
$(outdir)\cid.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\cid.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\cid.obj: error.h dpxutil.h pdfobj.h cidtype0.h cid.h type0.h
$(outdir)\cid.obj: cid_p.h cidtype2.h pdfparse.h numbers.h cid_basefont.h
$(outdir)\cidtype0.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\cidtype0.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\cidtype0.obj: mfileio.h numbers.h mem.h error.h pdfobj.h dpxutil.h
$(outdir)\cidtype0.obj: sfnt.h tt_aux.h cff_types.h cff_limits.h cff.h
$(outdir)\cidtype0.obj: cff_dict.h cs_type2.h cmap.h cid.h type0.h cid_p.h
$(outdir)\cidtype0.obj: cidtype0.h
$(outdir)\cidtype2.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\cidtype2.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\cidtype2.obj: numbers.h mem.h mfileio.h error.h dpxutil.h pdfobj.h
$(outdir)\cidtype2.obj: sfnt.h tt_aux.h tt_build.h tt_cmap.h tt_gsub.h
$(outdir)\cidtype2.obj: tt_table.h cmap.h cid.h type0.h cid_p.h cidtype2.h
$(outdir)\cmap.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\cmap.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\cmap.obj: error.h dpxutil.h cmap.h cid.h pdfobj.h type0.h
$(outdir)\cmap.obj: cmap_parse.h mfileio.h numbers.h
$(outdir)\cmap_parse.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\cmap_parse.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\cmap_parse.obj: mem.h error.h dpxutil.h pst.h cmap.h cid.h pdfobj.h
$(outdir)\cmap_parse.obj: type0.h cmap_parse.h mfileio.h numbers.h
$(outdir)\colorsp.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\colorsp.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\colorsp.obj: pdfdev.h numbers.h pdfobj.h pdfparse.h
$(outdir)\cs_type2.obj: error.h system.h ..\lib\miktex.h
$(outdir)\cs_type2.obj: ..\lib\miktex-features.h ..\libgnu\gnu-miktex.h
$(outdir)\cs_type2.obj: ..\libkps\web2c-miktex.h cff_types.h cff_limits.h
$(outdir)\dpxcrypt.obj: dpxcrypt.h
$(outdir)\dpxutil.obj: dpxutil.h
$(outdir)\dvi.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\dvi.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\dvi.obj: error.h mfileio.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\dvi.obj: dvicodes.h pdflimits.h pdfdoc.h pdfparse.h pdfspecial.h
$(outdir)\dvi.obj: tfm.h vf.h subfont.h fontmap.h
$(outdir)\dvipdfmx.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\dvipdfmx.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\dvipdfmx.obj: mfileio.h numbers.h dvi.h error.h pdfdev.h pdfobj.h
$(outdir)\dvipdfmx.obj: pdfdoc.h encodings.h agl.h type1.h type1c.h type0.h
$(outdir)\dvipdfmx.obj: cid.h cmap.h ttf.h colorsp.h pdfparse.h pdfspecial.h
$(outdir)\dvipdfmx.obj: vf.h pkfont.h thumbnail.h config.h psimage.h tfm.h
$(outdir)\dvipdfmx.obj: fontmap.h pdfencrypt.h
$(outdir)\encodings.obj: mem.h error.h system.h ..\lib\miktex.h
$(outdir)\encodings.obj: ..\lib\miktex-features.h ..\libgnu\gnu-miktex.h
$(outdir)\encodings.obj: ..\libkps\web2c-miktex.h mfileio.h numbers.h
$(outdir)\encodings.obj: pdfparse.h pdfobj.h dpxutil.h encodings.h
$(outdir)\encodings.obj: asl_charset.h encodings_p.h
$(outdir)\epdf.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\epdf.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\epdf.obj: mfileio.h numbers.h error.h ..\zlib\zlib.h
$(outdir)\epdf.obj: ..\zlib\zconf.h pdfobj.h pdfdoc.h pdfspecial.h epdf.h
$(outdir)\error.obj: error.h system.h ..\lib\miktex.h
$(outdir)\error.obj: ..\lib\miktex-features.h ..\libgnu\gnu-miktex.h
$(outdir)\error.obj: ..\libkps\web2c-miktex.h
$(outdir)\fontmap.obj: mem.h error.h system.h ..\lib\miktex.h
$(outdir)\fontmap.obj: ..\lib\miktex-features.h ..\libgnu\gnu-miktex.h
$(outdir)\fontmap.obj: ..\libkps\web2c-miktex.h mfileio.h numbers.h dpxutil.h
$(outdir)\fontmap.obj: pdfparse.h pdfobj.h pdflimits.h subfont.h
$(outdir)\htex.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\htex.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\htex.obj: mfileio.h numbers.h dvi.h error.h pdfdev.h pdfobj.h
$(outdir)\htex.obj: htex.h pdfparse.h pdfdoc.h
$(outdir)\jpegimage.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\jpegimage.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\jpegimage.obj: error.h mem.h mfileio.h numbers.h pdfobj.h
$(outdir)\jpegimage.obj: jpegimage.h
$(outdir)\mem.obj: mem.h
$(outdir)\mfileio.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\mfileio.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\mfileio.obj: mfileio.h numbers.h error.h
$(outdir)\mpost.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\mpost.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\mpost.obj: error.h mfileio.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\mpost.obj: pdfspecial.h pdfparse.h mpost.h pdflimits.h pdfdoc.h
$(outdir)\mpost.obj: fontmap.h subfont.h
$(outdir)\numbers.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\numbers.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\numbers.obj: error.h mfileio.h numbers.h
$(outdir)\pdfdev.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pdfdev.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\pdfdev.obj: error.h mfileio.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\pdfdev.obj: tfm.h pdfdoc.h pdfresource.h type1.h type1c.h type0.h
$(outdir)\pdfdev.obj: cmap.h cid.h ttf.h pkfont.h pdfspecial.h pdfparse.h
$(outdir)\pdfdev.obj: tpic.h htex.h mpost.h psspecial.h colorsp.h pdflimits.h
$(outdir)\pdfdev.obj: twiddle.h encodings.h agl.h fontmap.h colors.h
$(outdir)\pdfdoc.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pdfdoc.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\pdfdoc.obj: error.h mfileio.h numbers.h dvi.h pdfdev.h pdfobj.h
$(outdir)\pdfdoc.obj: pdfencrypt.h pdflimits.h pdfdoc.h pdfspecial.h
$(outdir)\pdfencrypt.obj: mem.h error.h system.h ..\lib\miktex.h
$(outdir)\pdfencrypt.obj: ..\lib\miktex-features.h ..\libgnu\gnu-miktex.h
$(outdir)\pdfencrypt.obj: ..\libkps\web2c-miktex.h pdfobj.h dpxcrypt.h
$(outdir)\pdfobj.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pdfobj.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\pdfobj.obj: error.h mfileio.h numbers.h pdflimits.h pdfencrypt.h
$(outdir)\pdfobj.obj: pdfobj.h pdfspecial.h pdfparse.h twiddle.h
$(outdir)\pdfobj.obj: ..\zlib\zlib.h ..\zlib\zconf.h
$(outdir)\pdfparse.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pdfparse.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\pdfparse.obj: mfileio.h numbers.h dvi.h error.h pdfdev.h pdfobj.h
$(outdir)\pdfparse.obj: pdfparse.h pdfspecial.h pdfdoc.h cmap.h cid.h type0.h
$(outdir)\pdfresource.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pdfresource.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\pdfresource.obj: mem.h error.h dpxutil.h pdfobj.h pdfresource.h
$(outdir)\pdfspecial.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pdfspecial.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\pdfspecial.obj: mem.h mfileio.h numbers.h dvi.h error.h pdfdev.h
$(outdir)\pdfspecial.obj: pdfobj.h pdflimits.h pdfspecial.h pdfdoc.h
$(outdir)\pdfspecial.obj: pdfparse.h epdf.h mpost.h jpegimage.h psimage.h
$(outdir)\pkfont.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pkfont.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\pkfont.obj: error.h mfileio.h numbers.h pkfont.h pdfobj.h
$(outdir)\pkfont.obj: pdflimits.h tfm.h
$(outdir)\pngimage.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pngimage.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\pngimage.obj: error.h mem.h pdfobj.h
$(outdir)\psimage.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\psimage.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\psimage.obj: mfileio.h numbers.h pdfobj.h psimage.h pdfspecial.h
$(outdir)\psimage.obj: epdf.h
$(outdir)\psspecial.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\psspecial.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\psspecial.obj: mem.h mfileio.h numbers.h psspecial.h pdfparse.h
$(outdir)\psspecial.obj: pdfobj.h pdfspecial.h psimage.h mpost.h pdfdoc.h
$(outdir)\pst.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pst.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\pst.obj: error.h dpxutil.h pst_obj.h pst.h
$(outdir)\pst_obj.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\pst_obj.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\pst_obj.obj: error.h dpxutil.h pst.h pst_obj.h
$(outdir)\sfnt.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\sfnt.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\sfnt.obj: error.h mfileio.h numbers.h sfnt.h
$(outdir)\subfont.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\subfont.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\subfont.obj: error.h mfileio.h numbers.h pdfparse.h pdfobj.h
$(outdir)\subfont.obj: pdflimits.h
$(outdir)\t1crypt.obj: t1crypt.h
$(outdir)\tfm.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\tfm.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\tfm.obj: error.h mfileio.h numbers.h pdflimits.h tfm.h
$(outdir)\thumbnail.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\thumbnail.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\thumbnail.obj: mfileio.h numbers.h mem.h pdfobj.h thumbnail.h
$(outdir)\thumbnail.obj: config.h pngimage.h
$(outdir)\tpic.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\tpic.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\tpic.obj: mfileio.h numbers.h tpic.h pdfparse.h pdfobj.h pdfdoc.h
$(outdir)\tpic.obj: pdfdev.h dvi.h error.h
$(outdir)\tt_aux.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\tt_aux.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\tt_aux.obj: error.h pdfobj.h sfnt.h mfileio.h numbers.h tt_table.h
$(outdir)\tt_aux.obj: tt_aux.h
$(outdir)\tt_build.obj: config.h system.h ..\lib\miktex.h
$(outdir)\tt_build.obj: ..\lib\miktex-features.h ..\libgnu\gnu-miktex.h
$(outdir)\tt_build.obj: ..\libkps\web2c-miktex.h mem.h error.h sfnt.h
$(outdir)\tt_build.obj: mfileio.h numbers.h tt_table.h tt_build.h
$(outdir)\tt_cmap.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\tt_cmap.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\tt_cmap.obj: error.h sfnt.h mfileio.h numbers.h tt_cmap.h
$(outdir)\tt_gsub.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\tt_gsub.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\tt_gsub.obj: error.h mem.h mfileio.h numbers.h sfnt.h tt_gsub.h
$(outdir)\tt_table.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\tt_table.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\tt_table.obj: error.h mem.h mfileio.h numbers.h sfnt.h tt_table.h
$(outdir)\tt_table.obj: macglyphs.h
$(outdir)\ttf.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\ttf.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\ttf.obj: error.h mfileio.h numbers.h tfm.h pdfobj.h pdfresource.h
$(outdir)\ttf.obj: dpxutil.h encodings.h unicode.h agl.h sfnt.h tt_cmap.h
$(outdir)\ttf.obj: tt_table.h tt_build.h tt_aux.h ttf.h
$(outdir)\type0.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\type0.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\type0.obj: error.h pdfobj.h cmap.h cid.h type0.h mfileio.h
$(outdir)\type0.obj: numbers.h pdfresource.h
$(outdir)\type1.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\type1.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\type1.obj: error.h mfileio.h numbers.h pdfobj.h type1.h tfm.h
$(outdir)\type1.obj: pdfparse.h pdflimits.h t1crypt.h twiddle.h encodings.h
$(outdir)\type1.obj: standardenc.h
$(outdir)\type1c.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\type1c.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h
$(outdir)\type1c.obj: mfileio.h numbers.h mem.h error.h pdfobj.h tfm.h
$(outdir)\type1c.obj: encodings.h unicode.h agl.h dpxutil.h sfnt.h tt_aux.h
$(outdir)\type1c.obj: cff_types.h cff_limits.h cff.h cff_dict.h cs_type2.h
$(outdir)\type1c.obj: type1c.h
$(outdir)\unicode.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\unicode.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mem.h
$(outdir)\unicode.obj: error.h pdfobj.h pdfresource.h encodings.h agl.h
$(outdir)\unicode.obj: unicode.h
$(outdir)\vf.obj: system.h ..\lib\miktex.h ..\lib\miktex-features.h
$(outdir)\vf.obj: ..\libgnu\gnu-miktex.h ..\libkps\web2c-miktex.h mfileio.h
$(outdir)\vf.obj: numbers.h pdflimits.h mem.h error.h tfm.h pdfdev.h pdfobj.h
$(outdir)\vf.obj: dvi.h vf.h dvicodes.h
