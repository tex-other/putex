## miktex.mak: PUTeX (MiKTeX 2.4)
## Copyright (C) 2001-2004 Chey-Woei Tsay <cwtsay@pu.edu.tw>
## Time-stamp: "2003-04-15 22:40:52 tcw"
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

miktex_cc_64bit_portability = 1

!include ..\miktex.inc

putex_dll_name = MiKTeX-putex-2

firstfunction = 1
lastfunction = 359


cloptions = \
	$(ccopt_default_char_type_is_unsigned) \
	$(cflagseh) \
	$(cflagsfastcall) \
	$(cflagspackfunc) \
	$(cstandard) \
	$(optvars) \
	-DMIKTEX_TEX -DPUTEX\

formatdir = $(prefix)\miktex\fmt

binaries = \
	$(outdir)\$(putex_dll_name).dll \
	$(outdir)\puinitex.exe \
	$(outdir)\putex.exe \
	$(outdir)\puvirtex.exe \

.cc{$(outdir)\}.obj:
	$(cc) $(cloptions) \
		$(ccopt_compile_all_files_as_cpp) \
		$(ccopt_output_directory)$(outdir)\ $<

all: common-all

$(outdir)\puinitex.exe: $(outdir) $(outdir)\putex.exe
	copy $(outdir)\putex.exe $@

$(outdir)\puvirtex.exe: $(outdir) $(outdir)\putex.exe
	copy $(outdir)\putex.exe $@

tex-n.web: tex.web
	"$(bindir)\web-n" < ..\tex\tex.web >tex-n.web

optimize.log: optimize1.cmd
	-del /f optimize.log
	@for /l %i in ($(firstfunction),1,$(lastfunction)) do optimize1 %i

#install: common-install install2 triptest
install: common-install install3

#install2: tex.pool "$(formatdir)" install-binaries
#	copy tex.pool "$(formatdir)"
#	$(initexmf) --add-file "$(formatdir)\tex.pool"

install3: \
		putex.pool \
		$(outdir)\puinitex.exe \
		$(outdir)\puvirtex.exe \
		$(outdir)\putex.exe \
		$(outdir)\$(putex_dll_name).dll \
		big5tex.tex \
		big5latex.tex \
		
	copy putex.pool "c:\texmf\miktex\fmt"
	copy $(outdir)\puinitex.exe "c:\texmf\miktex\bin"
	copy $(outdir)\puvirtex.exe "c:\texmf\miktex\bin"
	copy $(outdir)\putex.exe "c:\texmf\miktex\bin"
#	copy $(outdir)\putex.exe "c:\texmf\miktex\bin\pulatex.exe"
	copy $(outdir)\putex.exe "c:\texmf\miktex\bin\big5tex.exe"
	copy $(outdir)\putex.exe "c:\texmf\miktex\bin\big5latex.exe"
	copy $(outdir)\putex.exe "c:\texmf\miktex\bin\gbtex.exe"
	copy $(outdir)\putex.exe "c:\texmf\miktex\bin\gblatex.exe"
	copy $(outdir)\$(putex_dll_name).dll "c:\texmf\miktex\bin"
#	initexmf  --add-file "c:\texmf\miktex\fmt\putex.pool"
#	puinitex --alias putex puplain \dump
#	copy puplain.fmt "c:\localtexmf\miktex\fmt\putex.fmt"
#	initexmf  --add-file "c:\localtexmf\miktex\fmt\putex.fmt"
	puinitex big5tex \dump
	copy big5tex.fmt "c:\localtexmf\miktex\fmt"
#	initexmf  --add-file "c:\localtexmf\miktex\fmt\big5tex.fmt"
	puinitex big5latex \dump
	copy big5latex.fmt "c:\localtexmf\miktex\fmt"
#	initexmf  --add-file "c:\localtexmf\miktex\fmt\big5latex.fmt"
	puinitex gbtex \dump
	copy gbtex.fmt "c:\localtexmf\miktex\fmt"
#	initexmf  --add-file "c:\localtexmf\miktex\fmt\gbtex.fmt"
	puinitex gblatex \dump
	copy gblatex.fmt "c:\localtexmf\miktex\fmt"
#	initexmf  --add-file "c:\localtexmf\miktex\fmt\gblatex.fmt"
	del *.fmt


"$(formatdir)":
	$(mkpath) "$(formatdir)"

#qrt: common-qrt
#	cd test & $(MAKE) -f $(miktex_mak) & cd ..

pubig5def = c:\localtexmf\tex\putex\pubig5.def
pubig5faces = c:\localtexmf\tex\putex\big5faces.def
pugbkdef = c:\localtexmf\tex\putex\pugbk.def
pugbkfaces = c:\localtexmf\tex\putex\gbkfaces.def

big5tex.tex: puplain.tex $(pubig5def) $(pubig5faces)
big5latex.tex: pulatex.ltx $(pubig5def) $(pubig5faces)
gbtex.tex: puplain.tex $(pugbkdef) $(pugbkfaces)
gblatex.tex: pulatex.ltx $(pugbkdef) $(pugbkfaces)

# _____________________________________________________________________________
#
# tex
# _____________________________________________________________________________

!ifndef putexexport
putexexport = MiKTeX_PUTeX
!endif

putexclass = CPUTeXApp
putexmain = RunPUTeX
putexapp = g_PUTeXApp
putexdata = g_PUTeXData

tex_web = ..\tex\tex.web
miktex_web = miktex.web
tex_miktex_ch = ..\tex\tex-miktex.ch
tex_miktex_quiet_ch = ..\tex\tex-miktex-quiet.ch
tex_miktex_stat_ch = ..\tex\tex-miktex-stat.ch
tex_miktex_write18_ch = ..\tex\tex-miktex-write18.ch

tex_dyn_sed = ..\tex\dyn.sed

changefile = tex-putex-all.ch

miktex_change_files = \
	$(tex_miktex_ch) \
	$(tex_miktex_quiet_ch) \
	$(tex_miktex_stat_ch) \
	$(tex_miktex_write18_ch)


putex_change_files = putex400.ch

putex_obj = $(outdir)\putex.obj $(outdir)\putex.res $(miktex_lib) $(popt_lib) $(texmf_lib)

$(outdir)\$(putex_dll_name).dll $(outdir)\putex.lib: \
		$(outdir) \
		$(putex_obj) \
		putex.prf \
		$(dllbases) \
		
	$(link) $(dlllflags) $(ldebug) \
		-base:@$(dllbases),$(putex_dll_name) \
		-export:$(putexexport) \
		-implib:$(outdir)\putex.lib \
!if $(miktex_prf) > 0
		-order:@putex.prf \
!endif
		-out:$(outdir)\$(putex_dll_name).dll \
		$(putex_obj) $(conlibsdll)

$(outdir)\putex.res: \
		$(libdir)\miktex-version.h \
		$(libdir)\miktex.version \
		$(outdir) \
		tex-version.h \
		putex.rc \

	$(rc) $(rcflags) $(rcvars) -I$(libdir) -Fo$@ putex.rc

$(outdir)\putex.exe: \
		$(alias_cpp) \
		$(outdir) \
		$(outdir)\putex.lib \
		
	$(cc) \
		$(ccopt_minimize_size) \
		$(ccopt_output_file)$(outdir)\alias.obj  \
		$(ccopt_select_crt_mt_dll) \
		-DDLLAPI=__stdcall \
		-DDLLMAIN=$(putexexport) \
		$(alias_cpp) \
		$(ccopt_link) $(linkopt_output_file)$@ $(outdir)\putex.lib


$(outdir)\putex.obj: putex.cc putex.h
	$(cc) $(cloptions) \
		$(ccopt_compile_all_files_as_cpp) \
		$(ccopt_output_file)$(outdir)\ \
		-DTEXAPP=$(putexapp) \
		-DTEXCLASS=$(putexclass) \
		-DTEXDATA=$(putexdata) \
		putex.cc


putex.cc putex.h: putex.p $(tex_dyn_sed)
	$(c4p) $(c4prenames) \
		--chars-are-unsigned \
		--def-filename=putexdefs.h \
		--dll \
		--entry-name=$(putexmain) \
		--include-filename=putex-miktex.h \
		--one=putex \
		--using-namespace=MiKTeXMF \
		--var-name-prefix=m_ \
		--var-struct=$(putexdata) \
		-C \
		putex.p
	echo MIKTEX_DEFINE_WEBAPP($(putexexport), $(putexclass), \
		$(putexapp), $(putexmain), $(putexdata))>> putex.cc
	sed -f $(tex_dyn_sed) putex.h > putex.h.new
	del putex.h
	move putex.h.new putex.h


putex.p putex.pool: $(miktex_web) $(changefile)
	$(tangle) $(miktex_web) $(changefile) putex.p putex.pool

#putex.pool: $(changefile) $(miktex_web)
#	$(tangle) $(miktex_web) $(changefile) nul $@

pdf: putex40.tex
	pdftex putex40

dvi: putex40.tex
	tex putex40

putex40.tex: putex40-n.web
	weave putex40.web nul putex40.tex

$(miktex_web): $(miktex_change_file) $(tex_web)
	tie -m $(miktex_web) $(tex_web) $(miktex_change_files)

$(changefile): $(miktex_web) $(putex_change_files)
	$(tie) -c $(changefile) $(miktex_web) $(putex_change_files)

putex40-n.web: $(miktex_web) putex400.ch
	tie -m putex40.web $(miktex_web) putex400.ch
	web-n < putex40.web > putex40-n.web

# _____________________________________________________________________________
#
# cleanup
# _____________________________________________________________________________


mostlyclean: common-mostlyclean triptest-clean
	-del /f $(changefile)
	-del /f putex.cc
	-del /f putex.h
	-del /f putexdefs.h
	-del /f *.p
	-del /f *.log
	-del /f texsys.aux

clean: common-clean mostlyclean
	-del /f putex.pool
	-del /f *.fmt

distclean: common-distclean clean
	-del /f tex-n.web


# _____________________________________________________________________________
#
# dependencies
# _____________________________________________________________________________


!include ..\common-dependencies.inc

depend: triptex.cc tex.cc
	$(MAKEDEPEND1) -DMIKTEX_TEX $**

# DO NOT DELETE

$(outdir)\putex.obj: putex.h ..\libtexmf\c4plib.h ..\lib\miktex.h putex-miktex.h
$(outdir)\putex.obj: putexdefs.h putex.rc ..\lib\miktex.version ..\lib\miktex-version.h
$(outdir)\putex.obj: ..\libtexmf\texapp.h ..\libtexmf\texmfapp.h
$(outdir)\putex.obj: ..\libtexmf\inputline.h ..\libtexmf\webapp.h
$(outdir)\putex.obj: ..\libtexmf\miktex-texmf.h ..\libpopt\popt-miktex.h
$(outdir)\putex.obj: ..\libpopt\popt.h ..\libtexmf\texapp.inl
$(outdir)\putex.obj: ..\libtexmf\texmfapp.inl ..\libtexmf\inputline.inl
$(outdir)\putex.obj: ..\libtexmf\webapp.inl
