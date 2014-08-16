%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%%  WEB Change File for PUTeX (CJK version)
%%%  Modified and patched version for TeX Live
%%%
%%%  Copyright (C) 1997-2004 Chey-Woei Tsay <cwtsay@pu.edu.tw>
%%%  Copyright (C) 2013-2014 Clerk Ma      <clerkma@gmail.com>
%%%
%%%  This is the change file of PUTeX.
%%%
%%%  PUTeX is a free software; you can redistribute it and/or
%%%  modify it under the terms of the GNU General Public License as
%%%  published by the Free Software Foundation; either version 3, or (at
%%%  your option) any later version.
%%%
%%%  PUTeX is distributed in the hope that it will be useful, but
%%%  WITHOUT ANY WARRANTY; without even the implied warranty of
%%%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%%%  General Public License for more details.
%%%
%%%  You should have received a copy of the GNU General Public License
%%%  along with TeX Live; if not, write to the Free Software
%%%  Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
%%%  USA.
%%%
%%%
%%%  Version 4.0
%%%     add \PUXcatcode command to set catcodes of dbcs characters.
%%%     remove print_dbchar (58, 59, 70, 318, print_chinese_int)
%%% 
%%%  Version 4.0-web2c
%%%     removed the MikTeX part.
%%%  
%%%
%%%  TO DO:
%%%  new_character(582), make_accent(1123), char_box, rebox
%%%  mathmode: be awear of print_ASCII
%%%  check '(cat_code(buffer[loc])<>escape)' in section 1337
%%%
%%%  \PUXsetcfacehook -- set cface hook macro that is called when switched to the cface
%%%  \PUXsetcfonthook -- set cfont hook macro that is called when switched to the cfont
%%%
%%%  see section 224 for cspace skip and cespace skip
%%%
%%%  New Indices:
%%%     @^Input Encoding Dependencies@>
%%%     @^Modified for handling DBCS characters@>
%%%     @^CJK Fonts Extension@>

@x
\def\gglob{20, 26} % this should be the next two sections of "<Global...>"
@y
\def\gglob{20, 26} % this should be the next two sections of "<Global...>"
\def\PUTeX{PU\TeX}
\def\putexadd{\hskip -0.5in putex -- add -- }
\def\putexmod{\hskip -0.5in putex -- mod -- }
\def\putexend{\hskip -0.5in putex -- end -- }
@z

@x
@d banner==TeX_banner
@d banner_k==TeX_banner_k
@y
@d PUTeX_version_string=='-5.0' {current \PUTeX\ version}
@#
@d PUTeX_banner=='This is PUTeX, Version 3.1415926',PUTeX_version_string
@d PUTeX_banner_k==PUTeX_banner
  {printed when \PUTeX\ starts}
@#
@d banner==PUTeX_banner
@d banner_k==PUTeX_banner_k
@z

@x
@t\4@>@<Error handling procedures@>@/
@y
@t\4@>@<Error handling procedures@>@/
@t\4@>@<PUTeX routines that will be used by TeX routines@>@/
@#
{ end -- putex}
@z

@x
xchr[@'40]:=' ';
@y
@#@t\putexadd@>@#
for k := 0 to 255 do xchr[k] := k;
@#@t\putexend@>@#
xchr[@'40]:=' ';
@z

@x
The |append_char| macro, defined here, does not check to see if the
value of |pool_ptr| has gotten too high; this test is supposed to be
made before |append_char| is used. There is also a |flush_char|
@y
The |append_char| and |append_wchar| macros, defined here, do not check to see if the
value of |pool_ptr| has gotten too high; this test is supposed to be
made before |append_char| (or |append_wchar|) is used. There is also a |flush_char|
@z

@x
@d append_char(#) == {put |ASCII_code| \# at the end of |str_pool|}
begin str_pool[pool_ptr]:=si(#); incr(pool_ptr);
end
@y
@d append_char(#) == {put |ASCII_code| \# at the end of |str_pool|}
begin str_pool[pool_ptr]:=si(#); incr(pool_ptr);
end
@#@t\putexadd@>
@d append_wchar(#) == {TCW: put a double-byte char \# at the end of |str_pool|}
begin str_pool[pool_ptr]:=# div 256; str_pool[pool_ptr+1]:=# mod 256;
pool_ptr:=pool_ptr+2;
end
@#@t\putexend@>
@z

@x
using the |xchr| array to map it into an external character compatible with
|input_ln|. All printing comes through |print_ln| or |print_char|.
@y
using the |xchr| array to map it into an external character compatible with
|input_ln|. All printing comes through |print_ln|, |print_char|, or |print_wchar|.

TCW: The |print_wchar| macro is used to print one DBCS character. 

@d print_wchar(#)==begin print_char((#) div 256); print_char((#) mod 256) end {TCW}
@z

@x
@d character == subtype {the character code in a |char_node|}
@y
@d character == subtype {the character code in a |char_node|}
@d is_wchar_node(#) == (character(#)>255)
@d is_wchar(#) == ((#)>255)
@z

%% parallel kanji font, when typesetting kanjis, we need a match table.
@x
@!font_in_short_display:integer; {an internal font number}
@y
@!font_in_short_display:integer; {an internal font number}
@!cfont_in_short_display:integer; {TCW: an internal CJK font number}
@z

@x
sort of ``complicated'' are indicated only by printing `\.{[]}'.
@y
sort of ``complicated'' are indicated only by printing `\.{[]}'.@^CJK Fonts Extension@>
@z

@x
      begin if font(p)<>font_in_short_display then
        begin if (font(p)>font_max) then
          print_char("*")
@.*\relax@>
        else @<Print the font identifier for |font(p)|@>;
        print_char(" "); font_in_short_display:=font(p);
        end;
      print_ASCII(qo(character(p)));
      end;
@y
      begin if font(p)<>font_in_short_display and font(p)<>cfont_in_short_display then
        begin if (font(p)>cfont_max) then
          print_char("*")
@.*\relax@>
        else @<Print the font identifier for |font(p)|@>;
        print_char(" ");
        if font(p) <= font_max then
          font_in_short_display:=font(p)
        else
          cfont_in_short_display:=font(p);
        end;
      if is_wchar_node(p) then
        print_wchar(character(p))
      else
        print_ASCII(qo(character(p)));
      end;
@z

@x
its reference count, and one to print a rule dimension.
@y
its reference count, and one to print a rule dimension.@^CJK Fonts Extension@>
@z

@x
else  begin if (font(p)>font_max) then print_char("*")
@y
else  begin if (font(p)>cfont_max) then print_char("*")
@z

@x
  print_char(" "); print_ASCII(qo(character(p)));
@y
  print_char(" ");
  if is_wchar_node(p) then
    print_wchar(character(p))
  else
    print_ASCII(qo(character(p)));
@z

%% for kinsoku
@x
@d max_char_code=15 {largest catcode for individual characters}
@y
@d max_char_code=15 {largest catcode for individual characters}
@d boundary_normal=0 {CJK characters can be in any positions of lines}
@d tail_forbidden=1 {CJK characters can't be put in the head of lines}
@d head_forbidden=2 {CJK characters can't be put in the tail of lines}
@d max_type_code=2 {largest boundary code for CJK characters}
@d set_type_code_end(#) == # end
@d set_type_code(#) == begin
    type_code(#) := set_type_code_end
@z

@x
@ The next codes are special; they all relate to mode-independent
assignment of values to \TeX's internal registers or tables.
Codes that are |max_internal| or less represent internal quantities
that might be expanded by `\.{\\the}'.
@y
@ The next codes are special; they all relate to mode-independent
assignment of values to \TeX's internal registers or tables.
Codes that are |max_internal| or less represent internal quantities
that might be expanded by `\.{\\the}'.

TCW: Add 3 internal commands: |set_cfont|, |puxg_assign_flag|, and |puxg_assign_int|.
Add 12 user commands: |pux_cface_def|, |pux_face_match|, |pux_font_match|,
|pux_set_cface|, |pux_set_cface_attrib|,|pux_set_cfont_attrib|,
|pux_char_num|, |pux_char_given|, |pux_space|, |pux_range_catcode|,
|pux_range_type_code|, and |pux_dump_font_info|.@^CJK Fonts Extension@>
@z

@x
@d def_font=88 {define a font file ( \.{\\font} )}
@d register=89 {internal register ( \.{\\count}, \.{\\dimen}, etc.~)}
@d max_internal=89 {the largest code that can follow \.{\\the}}
@d advance=90 {advance a register or parameter ( \.{\\advance} )}
@d multiply=91 {multiply a register or parameter ( \.{\\multiply} )}
@d divide=92 {divide a register or parameter ( \.{\\divide} )}
@d prefix=93 {qualify a definition ( \.{\\global}, \.{\\long}, \.{\\outer} )}
@d let=94 {assign a command code ( \.{\\let}, \.{\\futurelet} )}
@d shorthand_def=95 {code definition ( \.{\\chardef}, \.{\\countdef}, etc.~)}
  {or \.{\\charsubdef}}
@d read_to_cs=96 {read into a control sequence ( \.{\\read} )}
@d def=97 {macro definition ( \.{\\def}, \.{\\gdef}, \.{\\xdef}, \.{\\edef} )}
@d set_box=98 {set a box ( \.{\\setbox} )}
@d hyph_data=99 {hyphenation data ( \.{\\hyphenation}, \.{\\patterns} )}
@d set_interaction=100 {define level of interaction ( \.{\\batchmode}, etc.~)}
@d max_command=100 {the largest command code seen at |big_switch|}
@y
@d set_cfont=88 {TCW: set current chinese font ( font identifiers )}
@d def_font=89 {define a font file ( \.{\\font} )}
@d register=90 {internal register ( \.{\\count}, \.{\\dimen}, etc.~)}
@d puxg_assign_flag=91 {TCW: set a PU\TeX\ global flag (\.{\\puxgCdiOut}, \.{\\puxgRotateCtext})}
@d puxg_assign_int=92 {TCW: set a PU\TeX\ global integer (\.{\\puxgCspace}, \.{\\puxgCEspace})}
@d pux_get_int=93 {TCW: get internal integer values ( \.{\\PUXnumdigits}, \.{\\PUXsign}, \.{\\PUXdigit} )}
@d max_internal=93 {the largest code that can follow \.{\\the}}
@d advance=94 {advance a register or parameter ( \.{\\advance} )}
@d multiply=95 {multiply a register or parameter ( \.{\\multiply} )}
@d divide=96 {divide a register or parameter ( \.{\\divide} )}
@d prefix=97 {qualify a definition ( \.{\\global}, \.{\\long}, \.{\\outer} )}
@d let=98 {assign a command code ( \.{\\let}, \.{\\futurelet} )}
@d shorthand_def=99 {code definition ( \.{\\chardef}, \.{\\countdef}, etc.~)}
  {or \.{\\charsubdef}}
@d read_to_cs=100 {read into a control sequence ( \.{\\read} )}
@d def=101 {macro definition ( \.{\\def}, \.{\\gdef}, \.{\\xdef}, \.{\\edef} )}
@d set_box=102 {set a box ( \.{\\setbox} )}
@d hyph_data=103 {hyphenation data ( \.{\\hyphenation}, \.{\\patterns} )}
@d set_interaction=104 {define level of interaction ( \.{\\batchmode}, etc.~)}
@d pux_cface_def=105 {TCW: define a chinese font face ( \.{\\PUXcfacedef} )}
@d pux_face_match=106 {TCW: English and Chinese face matching pair ( \.{\\PUXfacematch} )}
@d pux_font_match=107 {TCW: English and CJK font matching pair ( \.{\\PUXfontmatch} )}
@d pux_set_cface=108 {TCW: Set Chinese face}
@d pux_set_cface_attrib=109 {TCW: Set attributes of a Chinese face ( \.{\\PUXsetcfacecspace, etc.} )}
@d pux_set_cfont_attrib=110 {TCW: Set attributes of a CJK font ( \.{\\PUXsetcfontcspace, etc.} )}
@d pux_char_num=111 {TCW: Chinese character number ( \.{\\PUXchar} )}
@d pux_char_given=112 {TCW: define a Chinese character ( \.{\\PUXchardef} )}
@d pux_space=113 {Append space glue between Chinese and Tex characters ( \.{\\PUXcespace} )}
@d pux_range_catcode=114 {TCW: set catcodes for a range of characters( \.{\\PUXrangecatcode} )}
@d pux_range_type_code=115 {TCW: set catcodes for a range of characters( \.{\\PUXrangecatcode} )}
@d pux_split_number=116 {TCW: split a number to digits ( \.{\\PUXsplitnumber} )}
@d puxg_assign_space=117 {TCW: set a PU\TeX\ global integer (\.{\\puxgCspace}, \.{\\puxgCEspace})}
@d pux_set_default_cface=118 {TCW: set default CJK font face ( \.{\\PUXsetdefaultcface} )}
@d pux_dump_font_info=119 {TCW: dump font information ( \.{\\PUXdumpfontinfo} )}
@d max_command=119 {the largest command code seen at |big_switch|}
@z

@x
In the first region we have 256 equivalents for ``active characters'' that
act as control sequences, followed by 256 equivalents for single-character
control sequences.
@y
In the first region we have 65536 equivalents for ``active characters'' that
act as control sequences, followed by 65536 equivalents for single-character
control sequences.
@z

@x
@d single_base=active_base+256 {equivalents of one-character control sequences}
@d null_cs=single_base+256 {equivalent of \.{\\csname\\endcsname}}
@y
@d single_base=active_base+65536 {equivalents of one-character control sequences}
@d null_cs=single_base+65536 {equivalent of \.{\\csname\\endcsname}}
@z

%% parallel font
@x
@d font_id_base=frozen_null_font-font_base
  {begins table of 257 permanent font identifiers}
@d undefined_control_sequence=frozen_null_font+max_font_max+1 {dummy location}
@y
@d font_id_base=frozen_null_font-font_base
  {begins table of 257 permanent English font identifiers}
@d font_max_limit=5000
@d cfont_id_base=font_id_base+font_max_limit+1
  {TCW: begins table of 'font\_max\_limit' permanent CJK font identifiers}
@d cfont_max_limit=font_max_limit
@d cface_id_base=cfont_id_base+cfont_max_limit+1
  {TCW: begins table of 257 permanent Chinese face identifiers}
@d undefined_control_sequence=cface_id_base+257 {dummy location}
@z

@x
token parameters, as well as the tables of \.{\\toks} and \.{\\box}
registers.
@y
token parameters, as well as the tables of \.{\\toks} and \.{\\box}
registers.

TCW: Define |cur_cfont_loc| for two-byte char and the macro |cur_cfont|.
@z

@x
@d math_font_base=cur_font_loc+1 {table of 48 math font numbers}
@y
@d cur_cface_loc=cur_font_loc+1 {TCW: internal chinese font number outside math mode}
@d cur_cfont_loc=cur_cface_loc+1 {TCW: internal chinese font number outside math mode}
@d ectbl_cface_num_base=cur_cfont_loc+1 {TCW: table of 257 CJK face numbers matched with TeX face}
@d max_cface=256 {maximal CJK font faces number}
@d font_matching_table_base=ectbl_cface_num_base+max_cface+1 {table of font matches}
@d math_font_base=font_matching_table_base+font_max_limit+1 {table of 48 math font numbers}
@z

@x
  {table of 256 command codes (the ``catcodes'')}
@d lc_code_base=cat_code_base+256 {table of 256 lowercase mappings}
@y
  {TCW: table of 65536 command codes (the ``catcodes'')}
@d pux_cat_code_base=cat_code_base+256
@d pux_type_code_base=cat_code_base+65536 {TCW: table of 65536 type codes}
@d lc_code_base=pux_type_code_base+65536 {table of 256 lowercase mappings}
@z

@x
@d math_code_base=sf_code_base+256 {table of 256 math mode mappings}
@y
@d pux_local_names_base=sf_code_base+256 {TCW: table of 256 CJK name mappings.}
@d math_code_base=pux_local_names_base+256 {table of 256 math mode mappings}
@z

@x
@d cur_font==equiv(cur_font_loc)
@y
@d cur_font==equiv(cur_font_loc)
@d cur_cface==equiv(cur_cface_loc) {TCW}
@d cur_cfont==equiv(cur_cfont_loc) {TCW}
@d ectbl_cface_num(#)==equiv(ectbl_cface_num_base+(#)) {TCW}
@d font_matching_table(#)==equiv(font_matching_table_base+((#)-font_base)) {TCW}
@z

@x
@d cat_code(#)==equiv(cat_code_base+#)
@y
@d cat_code(#)==equiv(cat_code_base+#)
@d type_code(#)==equiv(pux_type_code_base+#)
@d local_names(#)==equiv(pux_local_names_base+#)
@z

@x
packages, not in \TeX\ itself, so that global interchange of formats is
possible.
@y
packages, not in \TeX\ itself, so that global interchange of formats is
possible.

TCW: Add |null_cfont| and initialization for |cur_font|.
@z

@x
@d null_font==font_base
@y
@d null_font==font_base
@d null_cfont==cfont_base
@d default_cfont==null_cfont+1
@z

@x
begin if n=cur_font_loc then print("current font")
else if n<math_font_base+16 then
@y
begin if n=cur_font_loc then print("current font")
else if n=cur_cface_loc then print("current cface")
else if n=cur_cfont_loc then print("current cfont")
else if n<math_font_base+16 then
@z

@x
  begin if n<lc_code_base then
    begin print_esc("catcode"); print_int(n-cat_code_base);
    end
@y
  begin if n<pux_type_code_base then
    begin
      if n<pux_cat_code_base then print_esc("catcode")
      else print_esc("PUXcatcode");
      print_int(n-cat_code_base);
    end
  else if n<lc_code_base then
      begin print_esc("PUXtypecode"); print_int(n-pux_type_code_base);
      end
@z

@x
  else  begin print_esc("sfcode"); print_int(n-sf_code_base);
    end;
@y
  else  if n<pux_local_names_base then
    begin print_esc("sfcode"); print_int(n-sf_code_base);
    end
  else  begin print_esc("PUXlocalnames"); print_int(n-pux_local_names_base);
    end;
@z

@x
  print_char("="); print_int(equiv(n));
@y
  print_char("=");
  if n>=pux_local_names_base then
    if n < 256 then print_char(equiv(n))
    else print_wchar(equiv(n))
  else print_int(equiv(n));
@z

@x
@d error_context_lines_code=54 {maximum intermediate line pairs shown}
@d tex_int_pars=55 {total number of \TeX's integer parameters}
@y
@d error_context_lines_code=54 {maximum intermediate line pairs shown}
@d puxg_rotate_ctext_code=55
@d puxg_cface_depth_code=56
@d pux_xspace_code=57
@d pux_wcharother_code=58
@d pux_CJKinput_code=59
@d pux_charset_code=60
@d pux_default_cface_code=61
@d pux_digit_num_code=62 {number of digits of the splitted number} 
@d pux_sign_code=63 {sign of the splitted number} 
@d pux_digit_base=64 {10 digits of splitted number} 
@d tex_int_pars=74 {total number of \TeX's integer parameters}
@z

@x
@d error_context_lines==int_par(error_context_lines_code)
@y
@d error_context_lines==int_par(error_context_lines_code)
@d puxg_rotate_ctext==int_par(puxg_rotate_ctext_code)
@d puxg_cface_depth==int_par(puxg_cface_depth_code)
@d pux_xspace==int_par(pux_xspace_code)
@d pux_wcharother==int_par(pux_wcharother_code)
@d pux_CJKinput==int_par(pux_CJKinput_code)
@d pux_charset==int_par(pux_charset_code)
@d pux_default_cface==int_par(pux_default_cface_code)
@d pux_digit_num==int_par(pux_digit_num_code)
@d pux_num_sign==int_par(pux_sign_code)
@d pux_nth_digit(#)==int_par(pux_digit_base+#)
@d default_csp=50
@d default_cesp=150
@d default_depth=200
@z

@x
othercases print("[unknown integer parameter!]")
@y
pux_xspace_code:print_esc("puxXspace");
pux_wcharother_code:print_esc("puxCJKcharOther");
pux_CJKinput_code:print_esc("puxCJKinput");
pux_charset_code:print_esc("puxCharSet");
puxg_rotate_ctext_code:print_esc("puxgRotateCtext");
puxg_cface_depth_code:print_esc("puxgCfaceDepth");
othercases print("[unknown integer parameter!]")
@z

@x
del_code("."):=0; {this null delimiter is used in error recovery}
@y
del_code("."):=0; {this null delimiter is used in error recovery}
puxg_cface_depth:=default_depth;
pux_CJKinput:=1;
@z

@x
      if cat_code(p-single_base)=letter then print_char(" ");
@y
      if get_cat_code(p-single_base)=letter then print_char(" ");
@z

@x
def_font: print_esc("font");
@y
def_font: print_esc("font");
pux_font_match: print_esc("PUXfontmatch"); {TCW}
pux_set_cface: print_esc("cface"); {TCW}
pux_range_catcode: print_esc("PUXrangecatcode"); {TCW}
pux_range_type_code: print_esc("PUXrangetypecode"); {TCW}
pux_split_number: print_esc("PUXsplitnumber"); {TCW}
@z

@x
A \TeX\ token is either a character or a control sequence, and it is
@^token@>
represented internally in one of two ways: (1)~A character whose ASCII
code number is |c| and whose command code is |m| is represented as the
number $2^8m+c$; the command code is in the range |1<=m<=14|. (2)~A control
sequence whose |eqtb| address is |p| is represented as the number
|cs_token_flag+p|. Here |cs_token_flag=@t$2^{12}-1$@>| is larger than
$2^8m+c$, yet it is small enough that |cs_token_flag+p< max_halfword|;
thus, a token fits comfortably in a halfword.

A token |t| represents a |left_brace| command if and only if
|t<left_brace_limit|; it represents a |right_brace| command if and only if
we have |left_brace_limit<=t<right_brace_limit|; and it represents a |match| or
|end_match| command if and only if |match_token<=t<=end_match_token|.
The following definitions take care of these token-oriented constants
and a few others.

@d cs_token_flag==@'7777 {amount added to the |eqtb| location in a
  token that stands for a control sequence; is a multiple of~256, less~1}
@d left_brace_token=@'0400 {$2^8\cdot|left_brace|$}
@d left_brace_limit=@'1000 {$2^8\cdot(|left_brace|+1)$}
@d right_brace_token=@'1000 {$2^8\cdot|right_brace|$}
@d right_brace_limit=@'1400 {$2^8\cdot(|right_brace|+1)$}
@d math_shift_token=@'1400 {$2^8\cdot|math_shift|$}
@d tab_token=@'2000 {$2^8\cdot|tab_mark|$}
@d out_param_token=@'2400 {$2^8\cdot|out_param|$}
@d space_token=@'5040 {$2^8\cdot|spacer|+|" "|$}
@d letter_token=@'5400 {$2^8\cdot|letter|$}
@d other_token=@'6000 {$2^8\cdot|other_char|$}
@d match_token=@'6400 {$2^8\cdot|match|$}
@d end_match_token=@'7000 {$2^8\cdot|end_match|$}
@y
A \TeX\ token is either a character or a control sequence, and it is
@^token@>
represented internally in one of two ways: (1)~A character whose ASCII
code number is |c| and whose command code is |m| is represented as the
number $2^{16}m+c$; the command code is in the range |1<=m<=14|. (2)~A control
sequence whose |eqtb| address is |p| is represented as the number
|cs_token_flag+p|. Here |cs_token_flag=@t$2^{20}-1$@>| is larger than
$2^{16}m+c$, yet it is small enough that |cs_token_flag+p< max_halfword|;
thus, a token fits comfortably in a halfword.

A token |t| represents a |left_brace| command if and only if
|t<left_brace_limit|; it represents a |right_brace| command if and only if
we have |left_brace_limit<=t<right_brace_limit|; and it represents a |match| or
|end_match| command if and only if |match_token<=t<=end_match_token|.
The following definitions take care of these token-oriented constants
and a few others.

@d cs_token_flag==@"FFFFF {amount added to the |eqtb| location in a
  token that stands for a control sequence; is a multiple of~65536, less~1}
@d left_brace_token==@"10000 {$2^{16}\cdot|left_brace|$}
@d left_brace_limit==@"20000 {$2^{16}\cdot(|left_brace|+1)$}
@d right_brace_token==@"20000 {$2^{16}\cdot|right_brace|$}
@d right_brace_limit==@"30000 {$2^{16}\cdot(|right_brace|+1)$}
@d math_shift_token==@"30000 {$2^{16}\cdot|math_shift|$}
@d tab_token==@"40000 {$2^{16}\cdot|tab_mark|$}
@d out_param_token==@"50000 {$2^{16}\cdot|out_param|$}
@d space_token==@"A0020 {$2^{16}\cdot|spacer|+|" "|$}
@d letter_token==@"B0000 {$2^{16}\cdot|letter|$}
@d other_token==@"C0000 {$2^{16}\cdot|other_char|$}
@d match_token==@"D0000 {$2^{16}\cdot|match|$}
@d end_match_token==@"E0000 {$2^{16}\cdot|end_match|$}
@z

@x
else  begin m:=info(p) div @'400; c:=info(p) mod @'400;
@y
else  begin m:=info(p) div @"10000; c:=info(p) mod @"10000;
@z

@x
left_brace,right_brace,math_shift,tab_mark,sup_mark,sub_mark,spacer,
  letter,other_char: print(c);
@y
letter,other_char: if is_wchar(c) then print_wchar(c) else print(c);
left_brace,right_brace,math_shift,tab_mark,sup_mark,sub_mark,spacer: print(c);
@z

@x
@d chr_cmd(#)==begin print(#); print_ASCII(chr_code);
  end
@y
@d chr_cmd(#)==begin print(#); print_ASCII(chr_code);
  end
@d wchr_cmd(#)==begin print(#);
  if is_wchar(chr_code) then
    print_wchar(chr_code)
  else print_ASCII(chr_code);
  end
@z

@x
letter: chr_cmd("the letter ");
other_char: chr_cmd("the character ");
@y
letter: wchr_cmd("the letter ");
other_char: wchr_cmd("the character ");
@z

@x
@!d:2..3; {number of excess characters in an expanded code}
@y
@!d:2..3; {number of excess characters in an expanded code}
@!first_control_char:integer; {the first character code of control sequence}
@z

@x
@ @<Input from external file, |goto restart| if no input found@>=
@^inner loop@>
begin switch: if loc<=limit then {current line not yet finished}
  begin cur_chr:=buffer[loc]; incr(loc);
@y
@ The |get_wchar| macro tries to read a double-byte character from |buffer|
at the position specified by the parameter. The code value is stored in the
global variable |cur_chr|.
@^inner loop@>
@^Modified for handling DBCS characters@>

@d get_wchar(#)==begin cur_chr:=buffer[#]; incr(#);
     if cur_chr > 127 and pux_CJKinput = 1 then begin
        cur_chr := cur_chr * 256 + buffer[#]; incr(#)
     end
   end

@<Input from external file, |goto restart| if no input found@>=
begin switch: if loc<=limit then {current line not yet finished}
  begin get_wchar(loc);
@z

@x
  reswitch: cur_cmd:=cat_code(cur_chr);
@y
  reswitch: cur_cmd:=get_cat_code(cur_chr);
@z

@x
buffer and the process is repeated, slowly but surely.

@<Scan a control...@>=
begin if loc>limit then cur_cs:=null_cs {|state| is irrelevant in this case}
else  begin start_cs: k:=loc; cur_chr:=buffer[k]; cat:=cat_code(cur_chr);
  incr(k);
@y
buffer and the process is repeated, slowly but surely.

\medskip

TCW: When the flag |expand_char| is true, we stop using |get_wchar| but
merely get a one-byte character so that reading DBCS characters
will not be confused. Besides, we neet to handle alphabetic numbers of the form
\.{'\\c}, where \.{c} is a DBCS characters.@^Modified for handling DBCS characters@>

@<Scan a control...@>=
begin if loc>limit then cur_cs:=null_cs {|state| is irrelevant in this case}
else  begin first_control_char := -1;@/
start_cs: k:=loc;
      if expand_char then begin
         cur_chr:=buffer[k];
         incr(k);
         expand_char:=false;
      end
      else get_wchar(k);
      cat:=get_cat_code(cur_chr);
      if first_control_char = -1 then first_control_char := cur_chr;
@z

@x
  cur_cs:=single_base+buffer[loc]; incr(loc);
  end;
@y
  @#{the control sequence is a control symbol, i.e., its name consisits of  only one letter. }
  if is_wchar(first_control_char) then begin
    cur_cs:=single_base+first_control_char; loc:=loc+2;
    end
  else begin
    cur_cs:=single_base+buffer[loc]; incr(loc);
    end;
  end;
@z

@x
the buffer left two or three places.
@y
the buffer left two or three places.

TCW: If it is indeed an expanded code, set the flag |expand_char|.
@^Modified for handling DBCS characters@>
@z

@x
  begin c:=buffer[k+1]; @+if c<@'200 then {yes, one is indeed present}
    begin d:=2;
@y
  begin c:=buffer[k+1]; @+if c<@'200 then {yes, one is indeed present}
    begin d:=2; expand_char:=true;
@z

@x
@ @<Scan ahead in the buffer...@>=
begin repeat cur_chr:=buffer[k]; cat:=cat_code(cur_chr); incr(k);
@y
@ @<Scan ahead in the buffer...@>=
@^Modified for handling DBCS characters@>
begin repeat get_wchar(k); cat:=get_cat_code(cur_chr);
@z

@x
if cat<>letter then decr(k);
  {now |k| points to first nonletter}
if k>loc+1 then {multiletter control sequence has been scanned}
@y
if cat<>letter then if cur_chr > 256 then k:=k-2 { go back 2 steps for a non-letter DBCS code }
else decr(k);
  {now |k| points to first nonletter}
if k>loc+1 and not (k = loc+2 and first_control_char > 255) then {multiletter control sequence has been scanned}
@z

@x
  else  begin cur_cmd:=t div @'400; cur_chr:=t mod @'400;
@y
  else  begin cur_cmd:=t div @"10000; cur_chr:=t mod @"10000;
@z

@x
if cur_cs=0 then cur_tok:=(cur_cmd*@'400)+cur_chr
@y
if cur_cs=0 then cur_tok:=(cur_cmd*@"10000)+cur_chr
@z

@x
  buffer[j]:=info(p) mod @'400; incr(j); p:=link(p);
@y
@^Modified for handling DBCS characters@>
  db_char:=info(p) mod @"10000;
  if is_wchar(db_char) then {a double-byte char}
     begin buffer[j]:=db_char div 256;
     buffer[j+1]:=db_char mod 256;
     j:=j + 2;
     end
  else begin
     buffer[j]:=db_char;
     incr(j);
     end;
  p:=link(p); {fix this for 2-byte code}
@z

@x
done: if cur_cs=0 then cur_tok:=(cur_cmd*@'400)+cur_chr
@y
done: if cur_cs=0 then cur_tok:=(cur_cmd*@"10000)+cur_chr
@z

@x
if cur_cs=0 then cur_tok:=(cur_cmd*@'400)+cur_chr
@y
if cur_cs=0 then cur_tok:=(cur_cmd*@"10000)+cur_chr
@z

@x
toks_register,assign_toks,def_family,set_font,def_font: @<Fetch a token list or
  font identifier, provided that |level=tok_val|@>;
@y
toks_register,assign_toks,def_family,set_font,def_font,set_cfont:
   @<Fetch a token list or font identifier, provided that |level=tok_val|@>;
@z

@x
assign_int: scanned_result(eqtb[m].int)(int_val);
@y
assign_int,puxg_assign_flag,puxg_assign_int: scanned_result(eqtb[m].int)(int_val);
pux_get_int:@<scan \PUTeX\ internal values@>;
@z

@x
char_given,math_given: scanned_result(cur_chr)(int_val);
@y
char_given,math_given,pux_char_given: scanned_result(cur_chr)(int_val);
@z

@x
@ @<Fetch a character code from some table@>=
begin scan_char_num;
@y
@ @<Fetch a character code from some table@>=
begin
  if (m = pux_cat_code_base) or (m = pux_type_code_base) then
    scan_wchar_num
  else if m = pux_local_names_base then begin
    char_val_flag:=true;
    scan_eight_bit_int;
    end
  else
    scan_char_num;
@z

@x
begin scan_font_ident;
if m=0 then scanned_result(hyphen_char[cur_val])(int_val)
else scanned_result(skew_char[cur_val])(int_val);
end
@y
begin scan_font_ident;
if cur_val <= font_max then
  if m=0 then scanned_result(hyphen_char[cur_val])(int_val)
  else scanned_result(skew_char[cur_val])(int_val);
end
@z

@x
@d octal_token=other_token+"'" {apostrophe, indicates an octal constant}
@d hex_token=other_token+"""" {double quote, indicates a hex constant}
@d alpha_token=other_token+"`" {reverse apostrophe, precedes alpha constants}
@d point_token=other_token+"." {decimal point}
@d continental_point_token=other_token+"," {decimal point, Eurostyle}
@y
@d octal_token==(other_token+"'") {apostrophe, indicates an octal constant}
@d hex_token==(other_token+"""") {double quote, indicates a hex constant}
@d alpha_token==(other_token+"`") {reverse apostrophe, precedes alpha constants}
@d point_token==(other_token+".") {decimal point}
@d continental_point_token==(other_token+",") {decimal point, Eurostyle}
@z

@x
if cur_val>255 then
@y
if cur_val>65535 then
@z

@x
@d zero_token=other_token+"0" {zero, the smallest digit}
@d A_token=letter_token+"A" {the smallest special hex digit}
@d other_A_token=other_token+"A" {special hex digit of type |other_char|}
@y
@d zero_token==(other_token+"0") {zero, the smallest digit}
@d A_token==(letter_token+"A") {the smallest special hex digit}
@d other_A_token==(other_token+"A") {special hex digit of type |other_char|}
@z

@x
`\.{height}' or `\.{width}' or `\.{depth}' specifications are
found (in any order).
@y
`\.{height}' or `\.{width}' or `\.{depth}' specifications are
found (in any order).

TCW: not intend to modify the function here;
     just append declarations of scanning routines for PUTeX.
@z

@x
if scan_keyword("depth") then
@.depth@>
  begin scan_normal_dimen; depth(q):=cur_val; goto reswitch;
  end;
scan_rule_spec:=q;
end;
@y
if scan_keyword("depth") then
@.depth@>
  begin scan_normal_dimen; depth(q):=cur_val; goto reswitch;
  end;
scan_rule_spec:=q;
end;

@<PUTeX basic scanning routines@>@;
@z

@x
var p:pointer; {tail of the token list}
@!q:pointer; {new node being added to the token list via |store_new_token|}
@!t:halfword; {token being appended}
@!k:pool_pointer; {index into |str_pool|}
begin str_room(1);
p:=temp_head; link(p):=null; k:=b;
while k<pool_ptr do
  begin t:=so(str_pool[k]);
  if t=" " then t:=space_token
  else t:=other_token+t;
  fast_store_new_token(t);
  incr(k);
  end;
pool_ptr:=b; str_toks:=p;
end;
@y
var p:pointer; {tail of the token list}
@!q:pointer; {new node being added to the token list via |store_new_token|}
@!t:halfword; {token being appended}
@!k:pool_pointer; {index into |str_pool|}
begin str_room(1);
p:=temp_head; link(p):=null; k:=b;
while k<pool_ptr do
  begin t:=so(str_pool[k]);
  if t > 128 then begin
    t:=t*256+so(str_pool[k+1]);
    incr(k);
    end;
  if t=" " then t:=space_token
  else t:=other_token+t;
  fast_store_new_token(t);
  incr(k);
  end;
pool_ptr:=b; str_toks:=p;
end;
@z

@x
containing something like `\.{-3.0pt minus 0.5fill}'.
@y
containing something like `\.{-3.0pt minus 0.5fill}'.

TCW: make the function able to print CJK characters stored in local names table.

@z

@x
begin get_x_token; scan_something_internal(tok_val,false);
@y
begin get_x_token; char_val_flag:=false; scan_something_internal(tok_val,false);
@z

@x
  int_val:print_int(cur_val);
@y
  int_val:if char_val_flag then
            if cur_val > 255 then print_wchar(cur_val)
            else {an empty slot}
            begin print_char("?"); print_char("?"); end
          else print_int(cur_val);
@z

@x
@d font_name_code=4 {command code for \.{\\fontname}}
@d job_name_code=5 {command code for \.{\\jobname}}
@y
@d font_name_code=4 {command code for \.{\\fontname}}
@d cnumber_code=5 {command code for \.{\\PUXcnumber}}
@d scnumber_code=6 {command code for \.{\\PUXscnumber}}
@d ucnumber_code=7 {command code for \.{\\PUXucnumber}}
@d fcnumber_code=8 {command code for \.{\\PUXfcnumber}}
@d acnumber_code=9 {command code for \.{\\PUXacnumber}}
@d cjknumber_code=10 {command code for \.{\\PUXcjknumber}}
@d nameseq_code=11 {command code for \.{\\PUXnameseq}}
@d job_name_code=12 {command code for \.{\\jobname}}
@d lower_cdigit_base=10    {lowercase style Chinese number}
@d upper_cdigit_base=25    {uppercase style Chinese number}
@z

@x
primitive("jobname",convert,job_name_code);@/
@!@:job_name_}{\.{\\jobname} primitive@>
@y
primitive("jobname",convert,job_name_code);@/
@!@:job_name_}{\.{\\jobname} primitive@>
primitive("PUXcnumber",convert,cnumber_code);@/
@!@:cnumber_}{\.{\\PUXcnumber} primitive@>
primitive("PUXscnumber",convert,scnumber_code);@/
@!@:scnumber_}{\.{\\PUXscnumber} primitive@>
primitive("PUXucnumber",convert,ucnumber_code);@/
@!@:ucnumber_}{\.{\\PUXucnumber} primitive@>
primitive("PUXfcnumber",convert,fcnumber_code);@/
@!@:fcnumber_}{\.{\\PUXfcnumber} primitive@>
primitive("PUXacnumber",convert,acnumber_code);@/
@!@:acnumber_}{\.{\\PUXacnumber} primitive@>
primitive("PUXcjknumber",convert,cjknumber_code);@/
@!@:cjknumber_}{\.{\\PUXcjknumber} primitive@>
primitive("PUXnameseq",convert,nameseq_code);@/
@!@:cjknameseq_}{\.{\\PUXnameseq} primitive@>
@z

@x
  meaning_code: print_esc("meaning");
  font_name_code: print_esc("fontname");
@y
  meaning_code: print_esc("meaning");
  font_name_code: print_esc("fontname");
  cnumber_code: print_esc("PUXcnumber");
  scnumber_code: print_esc("PUXscnumber");
  ucnumber_code: print_esc("PUXucnumber");
  fcnumber_code: print_esc("PUXfcnumber");
  acnumber_code: print_esc("PUXfanumber");
  cjknumber_code: print_esc("PUXcjknumber");
  nameseq_code: print_esc("PUXnameseq");
@z

@x
@!save_scanner_status:small_number; {|scanner_status| upon entry}
@!b:pool_pointer; {base of temporary string}
begin c:=cur_chr; @<Scan the argument for command |c|@>;
@y
@!save_scanner_status:small_number; {|scanner_status| upon entry}
@!b:pool_pointer; {base of temporary string}
@!dsize:integer;
@!saved_val,digit_base,sign:integer;
@!min_val,max_val,offset:integer;
begin c:=cur_chr; @<Scan the argument for command |c|@>;
@z

@x
case c of
number_code,roman_numeral_code: scan_int;
@y
case c of
number_code,roman_numeral_code,cnumber_code,scnumber_code,ucnumber_code,
fcnumber_code : scan_int;
acnumber_code: @<scan and split the number@>;
cjknumber_code:@<scan a CJK number with a possible selector and then  split it@>;
nameseq_code:@<scan a CJK name sequence number@>;
@z

@x
roman_numeral_code: print_roman_int(cur_val);
string_code:if cur_cs<>0 then sprint_cs(cur_cs)
  else print_char(cur_chr);
@y
roman_numeral_code: print_roman_int(cur_val);
cnumber_code: print_chinese_int(cur_val,lower_cdigit_base,false,false);
scnumber_code: print_chinese_int(cur_val,lower_cdigit_base,true,false);
ucnumber_code: print_chinese_int(cur_val,upper_cdigit_base,false,false);
fcnumber_code: print_chinese_int(cur_val,upper_cdigit_base,false,true);
acnumber_code: @<using full-width arabic characters to print a CJK number@>;
cjknumber_code: @<print a CJK number with specified format@>;
nameseq_code: @<print a CJK name sequence member@>;
string_code:if cur_cs<>0 then sprint_cs(cur_cs)
  else
    if is_wchar(cur_chr) then print_wchar(cur_chr)
    else print_char(cur_chr);
@z

@x
font_name_code: begin print(font_name[cur_val]);
  if font_size[cur_val]<>font_dsize[cur_val] then
    begin print(" at "); print_scaled(font_size[cur_val]);
    print("pt");
    end;
  end;
@y
font_name_code: begin
  if cur_val <=font_max then begin
    print(font_name[cur_val]);
    if font_size[cur_val]<>font_dsize[cur_val] then
      begin print(" at "); print_scaled(font_size[cur_val]);
      print("pt");
      end;
    end
  else begin
    print("CFONT");
    print(cface[cfont_face[cur_val]]);
    dsize:=cfont_dsize[cur_val] div @"10000;
    print_int(dsize);
    if cfont_size[cur_val]<>cfont_dsize[cur_val] then
      begin print(" at "); print_scaled(cfont_size[cur_val]);
      print("pt");
      end;
    end;
  end;
@z

@x
if (cur_cmd>active_char)or(cur_chr>255) then {not a character}
  begin m:=relax; n:=256;
@y
if (cur_cmd>active_char)or(cur_chr>65535) then {not a character}
  begin m:=relax; n:=256; {values other than 256 will break latex.fmt}
@z

@x
if (cur_cmd>active_char)or(cur_chr>255) then
  begin cur_cmd:=relax; cur_chr:=256;
  end;
@y
if (cur_cmd>active_char)or(cur_chr>65535) then
  begin cur_cmd:=relax; cur_chr:=256; {values other than 256 will break latex.fmt}
  end;
@z

@x
  pack_job_name(".dvi");
  while not b_open_out(dvi_file) do
    prompt_file_name("file name for output",".dvi");
@y
  pack_job_name(".cdi");
  while not b_open_out(dvi_file) do
    prompt_file_name("file name for output",".cdi");
@z

@x
@ Before we forget about the format of these tables, let's deal with two
of \TeX's basic scanning routines related to font information.

@<Declare procedures that scan font-related stuff@>=
procedure scan_font_ident;
var f:internal_font_number;
@!m:halfword;
begin @<Get the next non-blank non-call...@>;
if cur_cmd=def_font then f:=cur_font
else if cur_cmd=set_font then f:=cur_chr
@y
@ Before we forget about the format of these tables, let's deal with two
of \TeX's basic scanning routines related to font information.

TCW: handle the commands |def_cfont| and |set_cfont|.

@<Declare procedures that scan font-related stuff@>=
procedure scan_font_ident;
var f:integer;
@!m:halfword;
begin @<Get the next non-blank non-call...@>;
if cur_cmd=def_font then f:=cur_font
else if cur_cmd=set_font or cur_cmd=set_cfont then f:=cur_chr
@z

@x
bytes long, so it is in the range |0<=c<65536|. \TeX82 never uses this
command, but it should come in handy for extensions of \TeX\ that deal
with oriental languages.
@y
bytes long, so it is in the range |0<=c<65536|. \PUTeX\ uses this to typeset
a CJK two-byte character.
@z

@x
\yskip\hang|pre| 247 |i[1]| |num[4]| |den[4]| |mag[4]| |k[1]| |x[k]|.
Beginning of the preamble; this must come at the very beginning of the
file. Parameters |i|, |num|, |den|, |mag|, |k|, and |x| are explained below.
@y
\yskip\hang|pre| 247 |i[1]| |c[1]| |num[4]| |den[4]| |mag[4]| |k[1]| |x[k]|.
Beginning of the preamble; this must come at the very beginning of the
file. Parameters |i|, |c|, |num|, |den|, |mag|, |k|, and |x| are explained below.
@z

@x
\yskip\hang|post_post| 249. Ending of the postamble, see below.

\yskip\noindent Commands 250--255 are undefined at the present time.
@y
\yskip\hang|post_post| 249. Ending of the postamble, see below.

\yskip\hang|cfnt| 250 |k[2]|. Set |cf:=k|. \PUTeX\ uses this command for CJK font
numbers in the range |0<=k<65535|.

\yskip\hang|cfnt_def| 251 |k[2]| |l[1]| |n[l]| |c[1]| |s[4]| |ds[4]| |wt[2]| |y[1]|
  |w[4]| |h[4]| |d[4]| |fw[4]| |fh[4]| |fd[4]|. Define CJK font |k|, where |0<=k<65536|, see below.

\yskip\noindent Commands 252--255 are undefined at the present time.
@z

@x
@d set1=128 {typeset a character and move right}
@y
@d set1=128 {typeset a character and move right}
@d set2=129 {typeset a two-byte CJK character and move right}
@d set4=131 {typeset a four-byte CJK character and move right}
@z

@x
@d post_post=249 {postamble ending}
@y
@d post_post=249 {postamble ending}
@d cfnt=250 {set current chinese font}
@d cfnt_def=251 {define the meaning of a chinese font}
@z

@x
$$\hbox{|@!i[1]| |@!num[4]| |@!den[4]| |@!mag[4]| |@!k[1]| |@!x[k]|.}$$
The |i| byte identifies \.{DVI} format; currently this byte is always set
to~2. (The value |i=3| is currently used for an extended format that
allows a mixture of right-to-left and left-to-right typesetting.
Some day we will set |i=4|, when \.{DVI} format makes another
incompatible change---perhaps in the year 2048.)
@y
$$\hbox{|@!i[1]| |@!c[1]| |@!num[4]| |@!den[4]| |@!mag[4]| |@!k[1]| |@!x[k]|.}$$
The |i| byte identifies \.{CDI} format; currently this byte is always set
to~100. (Some day we will set |i=101|, when \.{CDI} format makes another
incompatible change---perhaps in the year 2048.)

The |c| byte identifies the default character code set of document. Currently, the following
code value is defined:

  0: USC2 (Unicode, not supported yet)

  1: Big5 (Traditional Chinese used in Taiwan and Hong Kong)
  
  2: GBK (Simplified Chinese used in PRC and Singapore)
@z

@x
@d id_byte=2 {identifies the kind of \.{DVI} files described here}
@y
@d id_byte=100 {identifies the kind of \.{DVI} files described here}
@z

@x
@ Here's a procedure that outputs a font definition. Since \TeX82 uses at
most 256 different fonts per job, |fnt_def1| is always used as the command code.
@y
@ Here's a procedure that outputs a font definition. Since \TeX82 uses at
most 256 different fonts per job, |fnt_def1| is always used as the command code.

TCW: the procedure |dvi_cfont_def| outputs a chinese font definition.
@z

@x
@<Output the font name whose internal number is |f|@>;
end;
@y
@<Output the font name whose internal number is |f|@>;
end;

procedure dvi_cfont_def (f:internal_cfont_number);
var k:pool_pointer;
    j:integer;
begin
j:=cfont_face[f];
dvi_out(cfnt_def);
dvi_out((f-cfont_base-1) div 256); dvi_out((f-cfont_base-1) mod 256);
{Output the CJK font face name}
dvi_out(length(cface_name[j]));
for k:= str_start[cface_name[j]] to str_start[cface_name[j]+1] - 1 do dvi_out(str_pool[k]);
dvi_out(cface_charset[j]);
dvi_four(cfont_size[f]);
dvi_four(cfont_dsize[f]);
dvi_out(cface_weight[j] div 256); dvi_out(cface_weight[j] mod 256);
dvi_out(cface_style[j]);
dvi_four(cfont_width[f]);
dvi_four(cfont_height[f]);
dvi_four(cfont_depth[f]);
dvi_four(cface_fw_width[j]);
dvi_four(cface_fw_height[j]);
dvi_four(cface_fw_depth[j]);
end;
@z

@x
dvi_h:=0; dvi_v:=0; cur_h:=h_offset; dvi_f:=null_font;
@y
dvi_h:=0; dvi_v:=0; cur_h:=h_offset; dvi_f:=null_font; dvi_cf:=null_cfont;
@z

@x
  begin dvi_out(pre); dvi_out(id_byte); {output the preamble}
@y
  begin dvi_out(pre); dvi_out(id_byte);
  doc_charset:=pux_charset; dvi_out(doc_charset); {output the preamble}
@z

@x
  print(" TeX output "); print_int(year); print_char(".");
@y
  print(" PUTeX output "); print_int(year); print_char(".");
@z

%% MMM
@x
@<Output node |p| for |hlist_out|...@>=
reswitch: if is_char_node(p) then
  begin synch_h; synch_v;
  repeat f:=font(p); c:=character(p);
  if f<>dvi_f then @<Change font |dvi_f| to |f|@>;
  if font_ec[f]>=qo(c) then if font_bc[f]<=qo(c) then
    if char_exists(orig_char_info(f)(c)) then  {N.B.: not |char_info|}
      begin if c>=qi(128) then dvi_out(set1);
      dvi_out(qo(c));@/
      cur_h:=cur_h+char_width(f)(orig_char_info(f)(c));
      goto continue;
      end;
  if mltex_enabled_p then
    @<Output a substitution, |goto continue| if not possible@>;
continue:
  p:=link(p);
  until not is_char_node(p);
  dvi_h:=cur_h;
  end
else @<Output the non-|char_node| |p| for |hlist_out|
    and move to the next node@>
@y
@<Output node |p| for |hlist_out|...@>=
reswitch: if is_char_node(p) then
  begin synch_h; synch_v;
  repeat f:=font(p); c:=character(p);
  if (is_wchar(c)) then begin
    if f<>dvi_cf then @<Change font |dvi_cf| to |f|@>;
    dvi_out(set2); dvi_out(c div 256); dvi_out(c mod 256);
    cur_h:=cur_h+cfont_width[f];
    end
  else begin
    if f<>dvi_f then @<Change font |dvi_f| to |f|@>;
    if font_ec[f]>=qo(c) then if font_bc[f]<=qo(c) then
      if char_exists(orig_char_info(f)(c)) then  {N.B.: not |char_info|}
      if c>=qi(128) then dvi_out(set1);
      dvi_out(qo(c));@/
      cur_h:=cur_h+char_width(f)(orig_char_info(f)(c));
      goto continue;
      end;
 if mltex_enabled_p then
    @<Output a substitution, |goto continue| if not possible@>;
continue:
  p:=link(p);
  until not is_char_node(p);
  dvi_h:=cur_h;
  end
else @<Output the non-|char_node| |p| for |hlist_out|
    and move to the next node@>
@z

@x
  dvi_out(post_post); dvi_four(last_bop); dvi_out(id_byte);@/
@y
  @<Output the CJK font definitions for all fonts that were used@>;
  dvi_out(post_post); dvi_four(last_bop); dvi_out(doc_charset); dvi_out(id_byte);@/
@z

@x
@<Incorporate character dimensions into the dimensions of the hbox...@>=
begin f:=font(p); i:=char_info(f)(character(p)); hd:=height_depth(i);
x:=x+char_width(f)(i);@/
s:=char_height(f)(hd);@+if s>h then h:=s;
s:=char_depth(f)(hd);@+if s>d then d:=s;
p:=link(p);
end
@y
@<Incorporate character dimensions into the dimensions of the hbox...@>=
begin f:=font(p); c:=character(p);
if (is_wchar(c)) then begin
  x:=x+cfont_width[f];@/
  s:=cfont_height[f];@+if s>h then h:=s;
  s:=cfont_depth[f];@+if s>d then d:=s;
  end
else begin
  i:=char_info(f)(c); hd:=height_depth(i);
  x:=x+char_width(f)(i);@/
  s:=char_height(f)(hd);@+if s>h then h:=s;
  s:=char_depth(f)(hd);@+if s>d then d:=s;
  end;
p:=link(p);
end
@z

@x
font_in_short_display:=null_font; short_display(list_ptr(r)); print_ln;@/
@y
font_in_short_display:=null_font; cfont_in_short_display:=null_cfont;@/
short_display(list_ptr(r)); print_ln;@/
@z

@x
  if (is_char_node(p))and(link(p)=null) then
    begin f:=font(p); v:=char_width(f)(char_info(f)(character(p)));
    if v<>width(b) then link(p):=new_kern(width(b)-v);
    end;
@y
  if (is_char_node(p))and(link(p)=null) then
    begin
    f:=font(p);
    if is_wchar_node(p) then
      v:=cfont_width[f]
    else
      v:=char_width(f)(char_info(f)(character(p)));
    if v<>width(b) then link(p):=new_kern(width(b)-v);
    end;
@z

@x
if is_char_node(v) then
  begin f:=font(v);
  break_width[1]:=break_width[1]-char_width(f)(char_info(f)(character(v)));
  end
@y
if is_char_node(v) then
  begin f:=font(v);
  if is_wchar_node(v) then
    break_width[1]:=break_width[1]-cfont_width[f]
  else
    break_width[1]:=break_width[1]-char_width(f)(char_info(f)(character(v)));
  end
@z

@x
  ligature_node: begin f:=font(lig_char(v));@/
    break_width[1]:=@|break_width[1]-
      char_width(f)(char_info(f)(character(lig_char(v))));
    end;
@y
  ligature_node: begin f:=font(lig_char(v));@/
    if is_wchar(character(lig_char(v))) then
    break_width[1]:=@|break_width[1]-cfont_width[f]
    else
    break_width[1]:=@|break_width[1]-
      char_width(f)(char_info(f)(character(lig_char(v))));
    end;
@z

@x
if is_char_node(s) then
  begin f:=font(s);
  break_width[1]:=@|break_width[1]+char_width(f)(char_info(f)(character(s)));
  end
@y
if is_char_node(s) then
  begin f:=font(s);
  if is_wchar_node(s) then
    break_width[1]:=break_width[1]+cfont_width[f]
  else
    break_width[1]:=@|break_width[1]+char_width(f)(char_info(f)(character(s)));
  end
@z

@x
  ligature_node: begin f:=font(lig_char(s));
    break_width[1]:=break_width[1]+
      char_width(f)(char_info(f)(character(lig_char(s))));
    end;
@y
  ligature_node: begin f:=font(lig_char(s));@/
    if is_wchar(character(lig_char(s))) then
    break_width[1]:=break_width[1]+cfont_width[f]
    else
    break_width[1]:=break_width[1]+
      char_width(f)(char_info(f)(character(lig_char(s))));
    end;
@z

@x
font_in_short_display:=null_font
@y
cfont_in_short_display:=null_cfont; font_in_short_display:=null_font
@z

@x
ligature_node: begin f:=font(lig_char(cur_p));
  act_width:=act_width+char_width(f)(char_info(f)(character(lig_char(cur_p))));
  end;
@y
ligature_node: begin f:=font(lig_char(cur_p));
  if is_wchar(character(lig_char(cur_p))) then
    act_width:=act_width+cfont_width[f]
  else
    act_width:=act_width+char_width(f)(char_info(f)(character(lig_char(cur_p))));
  end;
@z

@x
repeat f:=font(cur_p);
act_width:=act_width+char_width(f)(char_info(f)(character(cur_p)));
cur_p:=link(cur_p);
until not is_char_node(cur_p);
end
@y
repeat f:=font(cur_p);
if is_wchar_node(cur_p) then
  act_width:=act_width+cfont_width[f]
else
  act_width:=act_width+char_width(f)(char_info(f)(character(cur_p)));
cur_p:=link(cur_p);
until not is_char_node(cur_p);
end
@z

@x
if is_char_node(s) then
  begin f:=font(s);
  disc_width:=disc_width+char_width(f)(char_info(f)(character(s)));
  end
@y
if is_char_node(s) then
  begin f:=font(s);
  if is_wchar_node(s) then
    disc_width:=disc_width+cfont_width[f]
  else
    disc_width:=disc_width+char_width(f)(char_info(f)(character(s)));
  end
@z

@x
  ligature_node: begin f:=font(lig_char(s));
    disc_width:=disc_width+
      char_width(f)(char_info(f)(character(lig_char(s))));
    end;
@y
  ligature_node: begin f:=font(lig_char(s));
    if is_wchar(character(lig_char(s))) then
      disc_width:=disc_width+cfont_width[f]
    else
      disc_width:=disc_width+
        char_width(f)(char_info(f)(character(lig_char(s))));
    end;
@z

@x
if is_char_node(s) then
  begin f:=font(s);
  act_width:=act_width+char_width(f)(char_info(f)(character(s)));
  end
@y
if is_char_node(s) then
  begin f:=font(s);
  if is_wchar_node(s) then
    act_width:=act_width+cfont_width[f]
  else
    act_width:=act_width+char_width(f)(char_info(f)(character(s)));
  end
@z

@x
  ligature_node: begin f:=font(lig_char(s));
    act_width:=act_width+
      char_width(f)(char_info(f)(character(lig_char(s))));
    end;
@y
  ligature_node: begin f:=font(lig_char(s));
    if is_wchar(character(lig_char(s))) then
      act_width:=act_width+cfont_width[f]
    else
      act_width:=act_width+
        char_width(f)(char_info(f)(character(lig_char(s))));
    end;
@z

@x
  char_num: begin scan_char_num; cur_chr:=cur_val; cur_cmd:=char_given;
    goto reswitch;
    end;
  spacer,right_brace: begin if n>1 then @<Enter a hyphenation exception@>;
@y
  char_num: begin scan_char_num; cur_chr:=cur_val; cur_cmd:=char_given;
    goto reswitch;
    end;
  pux_char_given:@<Give improper hyphenation error for Chinese characters inside@>;
  pux_char_num: begin scan_wchar_num; cur_chr:=cur_val; cur_cmd:=pux_char_given;
    goto reswitch;
    end;
  spacer,right_brace: begin if n>1 then @<Enter a hyphenation exception@>;
@z

@x
if cur_chr="-" then @<Append the value |n| to list |p|@>
else  begin if lc_code(cur_chr)=0 then
    begin print_err("Not a letter");
@.Not a letter@>
    help2("Letters in \hyphenation words must have \lccode>0.")@/
      ("Proceed; I'll ignore the character I just read.");
    error;
    end
  else if n<63 then
    begin incr(n); hc[n]:=lc_code(cur_chr);
    end;
  end
@y
if cur_chr="-" then @<Append the value |n| to list |p|@>
else  begin if is_wchar(cur_chr) then
    begin print_err("Chinese character can't appear here");
@.Not a letter@>
    help2("Letters in \hyphenation words can't be Chinese characters.")@/
      ("Proceed; I'll ignore the character I just read.");
    error;
    end
  else if lc_code(cur_chr)=0 then
    begin print_err("Not a letter");
@.Not a letter@>
    help2("Letters in \hyphenation words must have \lccode>0.")@/
      ("Proceed; I'll ignore the character I just read.");
    error;
    end
  else if n<63 then
    begin incr(n); hc[n]:=lc_code(cur_chr);
    end;
  end
@z

@x
@d main_loop=70 {go here to typeset a string of consecutive characters}
@y
@d main_loop=70 {go here to typeset a string of consecutive characters}
@d main_loop_wchar=130 {go here to typeset a string of consecutive double-byte characters}
@d save_cur_wchar=132 {go here to typeset a double-byte characters}
@d next_is_a_char=133 {go here if next token is a single-byte character}
@d fetch_next_tok=134 {go here to fetch next token}
@z

@x
@t\4@>@<Declare the procedure called |handle_right_brace|@>@;
procedure main_control; {governs \TeX's activities}
@y
@t\4@>@<Declare the procedure called |handle_right_brace|@>@;
procedure main_control; {governs \TeX's activities}
@z

@x
label big_switch,reswitch,main_loop,main_loop_wrapup,
@y
label big_switch,reswitch,main_loop_wchar,main_loop_wchar+1,save_cur_wchar,
      next_is_a_char,fetch_next_tok,main_loop,main_loop+1,
      main_loop_wrapup,main_loop_lookahead+2,
@z

@x
var@!t:integer; {general-purpose temporary variable}
begin if every_job<>null then begin_token_list(every_job,every_job_text);
big_switch: get_x_token;@/
@y
var@!t:integer; {general-purpose temporary variable}
begin if every_job<>null then begin_token_list(every_job,every_job_text);
@<Initialization of global variables done in the |main_control| procedure@>@;
big_switch: get_x_token;@/
@z

@x
hmode+letter,hmode+other_char,hmode+char_given: goto main_loop;
@y
hmode+letter,hmode+other_char,hmode+char_given:
  if is_wchar(cur_chr) then goto main_loop_wchar
  else goto main_loop;
hmode+pux_char_given: goto main_loop_wchar;
@z

@x
hmode+char_num: begin scan_char_num; cur_chr:=cur_val; goto main_loop;@+end;
hmode+no_boundary: begin get_x_token;
  if (cur_cmd=letter)or(cur_cmd=other_char)or(cur_cmd=char_given)or
   (cur_cmd=char_num) then cancel_boundary:=true;
  goto reswitch;
  end;
@y
hmode+char_num: begin scan_char_num; cur_chr:=cur_val; goto main_loop;@+end;
hmode+pux_char_num: begin scan_wchar_num; cur_chr:=cur_val; goto main_loop_wchar;@+end;
hmode+no_boundary: begin get_x_token;
  if (cur_cmd=letter)or(cur_cmd=other_char)or(cur_cmd=char_given)or
   (cur_cmd=char_num)or(cur_cmd=pux_char_num)or(cur_cmd=pux_char_given) then cancel_boundary:=true;
  goto reswitch;
  end;
@z

@x
hmode+spacer: if space_factor=1000 then goto append_normal_space
  else app_space;
hmode+ex_space,mmode+ex_space: goto append_normal_space;
@y
@t\4@>@<Cases of |main_control| that handle spacer@>@;
@z

@x
main_loop:@<Append character |cur_chr| and the following characters (if~any)
@y
main_loop_wchar:@<Append double-byte character |cur_chr| and the following double-byte characters
  (if~any) to the current hlist in the current font; |goto main_loop| when a single-byte character
  has been fetched; |goto reswitch| when a non-character has been fetched@>;
main_loop:@<Append character |cur_chr| and the following characters (if~any)
@z

@x
@d adjust_space_factor==@t@>@;@/
  main_s:=sf_code(cur_chr);
@y
@d adjust_space_factor==@t@>@;@/
  if (cur_chr < 256) then main_s:=sf_code(cur_chr)
  else main_s:=1000;
@z

@x
adjust_space_factor;@/
main_f:=cur_font;
@y
main_cf:=cur_cfont; {in case the first letter is not a Chinese character}
@<If the preceding node is wchar node, then append a cespace@>;
main_loop+1:adjust_space_factor; main_f:=cur_font;
@z

@x
get_next; {set only |cur_cmd| and |cur_chr|, for speed}
if cur_cmd=letter then goto main_loop_lookahead+1;
if cur_cmd=other_char then goto main_loop_lookahead+1;
if cur_cmd=char_given then goto main_loop_lookahead+1;
x_token; {now expand and set |cur_cmd|, |cur_chr|, |cur_tok|}
if cur_cmd=letter then goto main_loop_lookahead+1;
if cur_cmd=other_char then goto main_loop_lookahead+1;
if cur_cmd=char_given then goto main_loop_lookahead+1;
if cur_cmd=char_num then
  begin scan_char_num; cur_chr:=cur_val; goto main_loop_lookahead+1;
  end;
if cur_cmd=no_boundary then bchar:=non_char;
cur_r:=bchar; lig_stack:=null; goto main_lig_loop;
main_loop_lookahead+1: adjust_space_factor;
fast_get_avail(lig_stack); font(lig_stack):=main_f;
cur_r:=qi(cur_chr); character(lig_stack):=cur_r;
if cur_r=false_bchar then cur_r:=non_char {this prevents spurious ligatures}
@y
@<Look ahead for next character. If it is a wide character then append
  a cespace, or leave |lig_stack| empty if there's no character there@>
@z

@x
else temp_ptr:=new_param_glue(space_skip_code);
link(tail):=temp_ptr; tail:=temp_ptr;
goto big_switch
@y
else temp_ptr:=new_param_glue(space_skip_code);
link(tail):=temp_ptr; tail:=temp_ptr;
if pux_xspace=0 then goto reswitch else goto big_switch
@z

@x
hbox_group: package(0);
adjusted_hbox_group: begin adjust_tail:=adjust_head; package(0);
  end;
@y
hbox_group: @<Setup |hbox_tail| and package@>;
adjusted_hbox_group: begin adjust_tail:=adjust_head;
  @<Setup |hbox_tail| and package@>;
  end;
@z

@x
vmode+start_par: new_graf(cur_chr>0);
vmode+letter,vmode+other_char,vmode+char_num,vmode+char_given,
   vmode+math_shift,vmode+un_hbox,vmode+vrule,
@y
vmode+start_par: new_graf(cur_chr>0);
vmode+letter,vmode+other_char,vmode+char_num,vmode+char_given,
   vmode+pux_char_num,vmode+pux_char_given,
   vmode+math_shift,vmode+un_hbox,vmode+vrule,
@z

@x
begin if tail<>head then
  begin if is_char_node(tail) then p:=tail
@y
begin if tail<>head then
  begin if is_char_node(tail) and not is_wchar_node(tail) then p:=tail
@z

@x
reswitch: if is_char_node(p) then
  begin f:=font(p); d:=char_width(f)(char_info(f)(character(p)));
  goto found;
  end;
@y
reswitch: if is_char_node(p) then
  begin f:=font(p);
  if is_wchar_node(p) then
    d:=cfont_width[f]
  else
    d:=char_width(f)(char_info(f)(character(p)));
  goto found;
  end;
@z

@x
letter,other_char,char_given: begin c:=ho(math_code(cur_chr));
    if c=@'100000 then
      begin @<Treat |cur_chr| as an active character@>;
      goto restart;
      end;
    end;
@y
letter,other_char,char_given: begin
  if is_wchar(cur_chr) then begin
    print_err("Chinese character is ignored in math mode");
    help1("Did you forget putting it into an \hbox?");
    error;
    goto restart;
    end
  else begin
    c:=ho(math_code(cur_chr));
    if c=@'100000 then
      begin @<Treat |cur_chr| as an active character@>;
      goto restart;
      end;
    end;
  end;
@z

@x
char_num: begin scan_char_num; cur_chr:=cur_val; cur_cmd:=char_given;
  goto reswitch;
  end;
@y
char_num: begin scan_char_num; cur_chr:=cur_val; cur_cmd:=char_given;
  goto reswitch;
  end;
pux_char_num: begin scan_wchar_num; cur_chr:=cur_val; cur_cmd:=pux_char_given;
  goto reswitch;
  end;
pux_char_given:begin print_err("Chinese character is ignored in math mode");
  help1("Did you forget putting it into an \hbox?");
  error;
  goto restart;
  end;
@z

@x
mmode+letter,mmode+other_char,mmode+char_given:
  set_math_char(ho(math_code(cur_chr)));
@y
mmode+letter,mmode+other_char,mmode+char_given:
  if is_wchar(cur_chr) then begin
    print_err("Chinese character is ignored in math mode");
    help1("Did you forget putting it into an \hbox?");
    error;
    end
  else
    set_math_char(ho(math_code(cur_chr)));
@z

@x
  letter,other_char: cur_val:=del_code(cur_chr);
@y
  letter,other_char:
    if is_wchar(cur_chr) then
      cur_val:=-1
    else
      cur_val:=del_code(cur_chr);
@z

@x
mmode+math_shift: if cur_group=math_shift_group then after_math
  else off_save;
@y
mmode+math_shift: if cur_group=math_shift_group then begin
    after_math;
    if math_mode_save<0 then begin
      get_x_token;
      @<If the token is a wide character, then append a cspace@>;
      goto reswitch;
      end;
    end
  else off_save;
@z

@x
tail_append(new_math(math_surround,after));
space_factor:=1000; unsave;
end
@y
math_mode_save:=m;
tail_append(new_math(math_surround,after));
space_factor:=1000; unsave;
end
@z

@x
any_mode(set_font),
any_mode(def_font),
@y
any_mode(set_font),
any_mode(def_font),
any_mode(set_cfont),
any_mode(pux_cface_def),
any_mode(pux_face_match),
any_mode(pux_font_match),
any_mode(pux_set_cface),
any_mode(puxg_assign_flag),
any_mode(puxg_assign_int),
any_mode(pux_get_int),
any_mode(pux_set_cface_attrib),
any_mode(pux_set_cfont_attrib),
any_mode(pux_range_catcode),
any_mode(pux_range_type_code),
any_mode(pux_split_number),
any_mode(puxg_assign_space),
any_mode(pux_set_default_cface),
any_mode(pux_dump_font_info),
@z

@x
@t\4@>@<Declare subprocedures for |prefixed_command|@>@t@>@;@/
procedure prefixed_command;
@y
@t\4@>@<Declare PUTeX subprocedures for |prefixed_command|@>@t@>@;@/
@t\4@>@<Declare subprocedures for |prefixed_command|@>@t@>@;@/
procedure prefixed_command;
@z

@x
@!n:integer; {ditto}
@!e:boolean; {should a definition be expanded? or was \.{\\let} not done?}
@y
@!n:integer; {ditto}
@!e:boolean; {should a definition be expanded? or was \.{\\let} not done?}
@<Other variables used by the procedure |prefixed_command|@>@;
@z

@x
set_font: define(cur_font_loc,data,cur_chr);
@y
set_font: begin define(cur_font_loc,data,cur_chr);@/
  @<Set the matching CJK font@>;
  end;
@z

@x
@d toks_def_code=6 {|shorthand_def| for \.{\\toksdef}}
@y
@d toks_def_code=6 {|shorthand_def| for \.{\\toksdef}}
@d pux_char_def_code=7 {|shorthand_def| for \.{\\PUXchardef}}
@z

@x
primitive("toksdef",shorthand_def,toks_def_code);@/
@!@:toks_def_}{\.{\\toksdef} primitive@>
@y
primitive("toksdef",shorthand_def,toks_def_code);@/
@!@:toks_def_}{\.{\\toksdef} primitive@>
primitive("PUXchardef",shorthand_def,pux_char_def_code);@/
@!@:pux_char_def_}{\.{\\toksdef} primitive@>
@z

@x
  mu_skip_def_code: print_esc("muskipdef");
  char_sub_def_code: print_esc("charsubdef");
  othercases print_esc("toksdef")
@y
  mu_skip_def_code: print_esc("muskipdef");
  char_sub_def_code: print_esc("charsubdef");
  toks_def_code: print_esc("toksdef");
  othercases print_esc("PUXchardef")
@z

@x
  case n of
  char_def_code: begin scan_char_num; define(p,char_given,cur_val);
    end;
  math_char_def_code: begin scan_fifteen_bit_int; define(p,math_given,cur_val);
    end;
  othercases begin scan_eight_bit_int;
@y
  case n of
  char_def_code: begin scan_char_num; define(p,char_given,cur_val);
    end;
  math_char_def_code: begin scan_fifteen_bit_int; define(p,math_given,cur_val);
    end;
  pux_char_def_code: begin scan_wchar_num; define(p,pux_char_given,cur_val);
    end;
  othercases begin scan_eight_bit_int;
@z

@x
primitive("catcode",def_code,cat_code_base);
@!@:cat_code_}{\.{\\catcode} primitive@>
@y
primitive("catcode",def_code,cat_code_base);
@!@:cat_code_}{\.{\\catcode} primitive@>
primitive("PUXcatcode",def_code,pux_cat_code_base);
@!@:pux_cat_code_}{\.{\\PUXcatcode} primitive@>
primitive("PUXtypecode",def_code,pux_type_code_base);
@!@:pux_type_code_base_}{\.{\\PUXtypecode} primitive@>
primitive("PUXlocalnames",def_code,pux_local_names_base);
@!@:pux_local_names_base_}{\.{\\PUXlocalnames} primitive@>
@z

@x
  else if chr_code=math_code_base then print_esc("mathcode")
@y
  else if chr_code=pux_cat_code_base then print_esc("PUXcatcode")
  else if chr_code=pux_type_code_base then print_esc("PUXtypecode")
  else if chr_code=pux_local_names_base then print_esc("PUXlocalnames")
  else if chr_code=math_code_base then print_esc("mathcode")
@z

@x
  p:=cur_chr; scan_char_num; p:=p+cur_val; scan_optional_equals;
  scan_int;
@y
  p:=cur_chr;
  if p = pux_cat_code_base then
  	begin scan_wchar_num; p := cat_code_base;
  	end
  else if p = pux_type_code_base then scan_wchar_num
  else if p = pux_local_names_base then scan_eight_bit_int
  else
  	scan_char_num;
  p:=p+cur_val; scan_optional_equals;
  if p=pux_local_names_base then scan_wchar_num
  else scan_int;
@z

@x
else if cur_chr=math_code_base then n:=@'100000
@y
else if cur_chr=pux_cat_code_base then n:=max_char_code
else if cur_chr=pux_type_code_base then n:=max_type_code
else if cur_chr=pux_local_names_base then n:=65535
else if cur_chr=math_code_base then n:=@'100000
@z

@x
set_box: begin scan_eight_bit_int;
  if global then n:=256+cur_val@+else n:=cur_val;
  scan_optional_equals;
  if set_box_allowed then scan_box(box_flag+n)
  else begin print_err("Improper "); print_esc("setbox");
@y
set_box: begin scan_eight_bit_int;
  if global then n:=256+cur_val@+else n:=cur_val;
  scan_optional_equals;
  if set_box_allowed then begin
    in_set_box:=true;
    scan_box(box_flag+n);
    in_set_box:=false;
    end
  else begin print_err("Improper "); print_esc("setbox");
@z

@x
procedure new_font(@!a:small_number);
label common_ending;
var u:pointer; {user's font identifier}
@!s:scaled; {stated ``at'' size, or negative of scaled magnification}
@!f:internal_font_number; {runs through existing fonts}
@!t:str_number; {name for the frozen font identifier}
@!old_setting:0..max_selector; {holds |selector| setting}
begin if job_name=0 then open_log_file;
  {avoid confusing \.{texput} with the font name}
@.texput@>
get_r_token; u:=cur_cs;
if u>=hash_base then t:=text(u)
else if u>=single_base then
  if u=null_cs then t:="FONT"@+else t:=u-single_base
else  begin old_setting:=selector; selector:=new_string;
  print("FONT"); print(u-active_base); selector:=old_setting;
@.FONTx@>
  str_room(1); t:=make_string;
  end;
define(u,set_font,null_font); scan_optional_equals; scan_file_name;
@<Scan the font size specification@>;
@<If this font has already been loaded, set |f| to the internal
  font number and |goto common_ending|@>;
f:=read_font_info(u,cur_name,cur_area,s);
common_ending: equiv(u):=f; eqtb[font_id_base+f]:=eqtb[u]; font_id_text(f):=t;
end;
@y
@<Declare the function called |fw_times_sd|@>@;
@<Declare the function called |find_cface_num|@>@;
@<Declare the procedure called |check_cfont|@>@;
@<Declare the procedure called |make_cfont|@>@;
procedure new_font(@!a:small_number);
label common_ending;
var u:pointer; {user's font identifier}
@!j,k:pool_pointer;
@!s:scaled; {stated ``at'' size, or negative of scaled magnification}
@!f:internal_font_number; {runs through existing fonts}
@!t:str_number; {name for the frozen font identifier}
@!old_setting:0..max_selector; {holds |selector| setting}
@!flushable_string:str_number; {string not yet referenced}
@<Other local variables used by procedure |new_font|@>@;
begin if job_name=0 then open_log_file;
  {avoid confusing \.{texput} with the font name}
@.texput@>
get_r_token; u:=cur_cs;
if u>=hash_base then t:=text(u)
else if u>=single_base then
  if u=null_cs then t:="FONT"@+else t:=u-single_base
else  begin old_setting:=selector; selector:=new_string;
  print("FONT"); print(u-active_base); selector:=old_setting;
@.FONTx@>
  str_room(1); t:=make_string;
  end;
scan_optional_equals; scan_file_name;@/
@<Scan the font size specification@>;
if (length(cur_name) > 5) then
  begin
  j:=str_start[cur_name];
  if (str_pool[j]='C' and str_pool[j+1]='F' and str_pool[j+2]='O'
      and str_pool[j+3]='N' and str_pool[j+4]='T') then
    @<Define a CJK font and then goto |common_ending|@>;
  end;
define(u,set_font,null_font);
@<If this font has already been loaded, set |f| to the internal
  font number and |goto common_ending|@>;
f:=read_font_info(u,cur_name,cur_area,s);
common_ending: equiv(u):=f; eqtb[font_id_base+f]:=eqtb[u]; font_id_text(f):=t;
end;
@z

@x
@!t:halfword; {token}
@!c:eight_bits; {character code}
begin b:=cur_chr; p:=scan_toks(false,false); p:=link(def_ref);
@y
@!t:halfword; {token}
@!c:quarterword; {character code}
begin b:=cur_chr; p:=scan_toks(false,false); p:=link(def_ref);
@z

@x
@<Change the case of the token in |p|, if a change is appropriate@>=
t:=info(p);
if t<cs_token_flag+single_base then
  begin c:=t mod 256;
  if equiv(b+c)<>0 then info(p):=t-c+equiv(b+c);
  end
@y
@<Change the case of the token in |p|, if a change is appropriate@>=
t:=info(p);
if t<cs_token_flag+single_base then
  begin c:=t mod 65536;
  if c < 256 then {only convert the single-byte char}
    if equiv(b+c)<>0 then info(p):=t-c+equiv(b+c);
  end
@z

@x
@<Dump the font information@>;
@y
@<Dump the font information@>;
@<Dump the CJK font face information@>;
@<Dump the face matching table@>;
@<Dump the CJK font information@>;
@z

@x
@<Undump the font information@>;
@y
@<Undump the font information@>;
@<Undump the CJK font face information@>;
@<Unump the face matching table@>;
@<Undump the CJK font information@>;
@z

@x
15: begin font_in_short_display:=null_font; short_display(n);
@y
15: begin font_in_short_display:=null_font; cfont_in_short_display:=null_cfont; short_display(n);
@z

@x
@* \[55] Index.
@y
@* \[55] Introduction to \PUTeX.
\PUTeX is an extension of \TeX to handle CJK character sets.

@ @<Glob...@>=
@!hi_byte, @!lo_byte : ASCII_code;
{temp var for storing high byte and low byte of a double-byte character}
@!db_char : quarterword; {temp var for storing a double-byte character}
@!expand_char : boolean;
@!doc_charset : eight_bits;
@!char_val_flag : boolean;

@ @<Set initial...@>=
expand_char:=false;

@ The default catcode for CJK characters is `letter'.

@<Initialize table entries...@>=
for k:= 256 to 65535 do
  begin cat_code(k) := letter;
  end;

@ Initially, \PUTeX\ just set type codes for OT1 encoding.

@d set_tail_forbidden(#) == set_type_code(#)(tail_forbidden)
@d set_head_forbidden(#) == set_type_code(#)(head_forbidden)

@<Initialize table entries...@>=
set_tail_forbidden("(");
set_tail_forbidden("[");
set_tail_forbidden("{");@/
set_head_forbidden("!");
set_head_forbidden(")");
set_head_forbidden(",");@/
set_head_forbidden(".");
set_head_forbidden(":");
set_head_forbidden(";");@/
set_head_forbidden("?");
set_head_forbidden("]");
set_head_forbidden("}");

@ @<PUTeX routines...@>=
function get_cat_code (ch:halfword) : halfword;
var cat: halfword; {catcode}
begin
  if pux_wcharother <> 0 then
    if ch > 255 then cat := other_char
    else cat := cat_code(ch)
  else cat := cat_code(ch);
  get_cat_code := cat;
end;

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("PUXrangecatcode",pux_range_catcode,0);
primitive("PUXrangetypecode",pux_range_type_code,0);

@ @<Other variables used by the procedure |prefixed_command|@>=
@!bc, ec: halfword; {the begin char and end char of code range}

@ @<Assignments@>=
pux_range_catcode, pux_range_type_code: begin
    p:=cur_chr;
    if cur_cmd = pux_range_catcode then begin
      n:=max_char_code;
      p:=cat_code_base;
    end
    else begin
      n:=max_type_code;
      p:=pux_type_code_base;
    end;
    scan_wchar_num; bc := cur_val;@/
    scan_keyword("to");@/
    scan_wchar_num; ec := cur_val;@/
    scan_optional_equals;@/
    scan_int;@/
    
    if (bc = 0) or (ec = 0) or (ec < bc) then begin
      if ec < bc then begin
        print_err("Invalid range setting, ec < bc");
      end;
      help1("I'm going to ignore this command.");@/
      error;
      goto exit; @.Invalid range@>
    end;
    
    if (cur_val < 0) or (cur_val > n) then begin
      print_err("Invalid catcode ("); print_int(cur_val);
      print("), should be in the range 0..15");@/
      help1("I'm going to ignore this command.");@/
      error;
      goto exit; @.Invalid code@>
    end;

    for k := bc to ec do define(p+k,data,cur_val);
  end;

@ @<Initialize table entries...@>=
for k:=0 to 255 do local_names(k) := "?";

@ @<PUTeX basic scanning routines@>=
function scan_name: str_number;
  begin
  @<Get the next non-blank non-call token@>;
  while cur_cmd=letter do
    begin
    if (is_wchar(cur_chr)) then append_wchar(cur_chr) else append_char(cur_chr);
    get_x_token;
    end;
  if pool_ptr <> str_start[str_ptr] then
    scan_name:=make_string
  else
    scan_name:=0;
  end;

@ @<Declare procedures that scan restricted classes of integers@>=
procedure scan_wchar_num;
begin scan_int;
if (cur_val<257)or(cur_val>65535) then
  begin print_err("Bad wide character code");
@.Bad wide character code@>
  help2("A wide character number must be between 256 and 65536.")@/
    ("I changed this one to zero."); int_error(cur_val); cur_val:=0;
  end;
end;

@* \[56] CJK Numbers.

@<Global variables@>=
@!cnum_one_flag:boolean;

@
@d ten_wchar_offset=10
@d hundred_wchar_offset=11
@d thousand_wchar_offset=12
@d ten_thousand_wchar_offset=13
@d hundred_million_wchar_offset=14
@d arabic_wchar_offset=40
@d negative_wchar_offset=50
@d negative_wsym_offset=51
@d twenty_wchar_offset=52
@d thirty_wchar_offset=53
@d CJK_digit_offset=0
@d C_simple_digit_offset=10
@d C_formal_digit_offset=25
@d C_arabic_digit_offset=40

@ 
@<Basic print...@>=
procedure print_chinese_int (@!n,@!digit_base:integer;@!simple,@!formal:boolean);
var @!m:integer;
begin
  cnum_one_flag:=false;
  if n < 0 then begin
    {|print_dbchar| is replaced by the following 2 |print_char| calls. }
    print_wchar(local_names(negative_wchar_offset));
    negate(n);
    end;
  if n<100 then print_small_chinese_int(n,digit_base,simple,formal)
  else begin
    if n>99999999 then begin
      print_small_chinese_int(n div 100000000,digit_base,simple,formal);
      print_wchar(local_names(digit_base+hundred_million_wchar_offset));
      cnum_one_flag:=true;
      n:=n mod 100000000;
      if n>0 and n<10000000 then
        print_wchar(local_names(digit_base)); {zero character in Chinese}
      end;
    if n>9999 then begin
      print_medium_chinese_int(n div 10000,digit_base,simple,formal);
      print_wchar(local_names(digit_base+ten_thousand_wchar_offset));
      cnum_one_flag:=true;
      n:=n mod 10000;
      if n>0 and n<1000 then
        print_wchar(local_names(digit_base)); {zero character in Chinese}
      end;
    print_medium_chinese_int(n,digit_base,simple,formal);
  end;
end;

@ The following procedure prints a number n, $0\le n \le 99$.
@<Basic print...@>=
procedure print_small_chinese_int (n,@!digit_base:integer;@!simple,@!formal:boolean);
label done1;
begin@/
  if n<10 then print_wchar(local_names(n+digit_base))
  else begin
    if n<20 then begin
      if formal or cnum_one_flag then
        print_wchar(local_names(digit_base+1));
      print_wchar(local_names(digit_base+10));@/
      goto done1;
      end;
    if n<30 and simple then begin
      print_wchar(local_names(twenty_wchar_offset));@/
      goto done1;
      end;
    if n<40 and simple then begin
      print_wchar(local_names(thirty_wchar_offset));@/
      goto done1;
      end;
    print_wchar(local_names(digit_base + n div 10));
    print_wchar(local_names(digit_base+10));
done1: n:=n mod 10;
    if n>0 then print_wchar(local_names(n+digit_base));
    end
end;

@ Print a chinese number of medium size.
@<Basic print...@>=
procedure print_medium_chinese_int (n,@!digit_base:integer;@!simple,@!formal:boolean);
begin
  if n>999 then begin
    print_wchar(local_names(digit_base+n div 1000));
    print_wchar(local_names(digit_base+thousand_wchar_offset));
    n:=n mod 1000;
    if n>0 and n<99 then
        print_wchar(local_names(digit_base)); {zero character in Chinese}
    end;
  if n>99 then begin
    print_wchar(local_names(digit_base+n div 100));
    print_wchar(local_names(digit_base+hundred_wchar_offset));
    n:=n mod 100;
    if n>0 and n<9 then
        print_wchar(local_names(digit_base)); {zero character in Chinese}
    end;
  cnum_one_flag:=true;
  if n>0 then print_small_chinese_int(n,digit_base,simple,formal);
end;

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("puxnumdigits",pux_get_int,int_base+pux_digit_num_code);
primitive("puxsign",pux_get_int,int_base+pux_sign_code);
primitive("puxdigit",pux_get_int,int_base+pux_digit_base);

@ @<Cases of |print_cmd_chr|...@>=
pux_get_int:
  if chr_code=pux_digit_num_code+int_base then
    print_esc("puxnumdigits")
  else if chr_code=pux_sign_code+int_base then
    print_esc("puxsign")
  else if chr_code=pux_digit_base+int_base then
    print_esc("puxdigit");

@ @<Assignments@>=
pux_get_int: begin
  print_err("You can't assign values to internal read-only parameters.");
  error;
end;

@ @<scan \PUTeX\ internal values@>=
begin
  if m=pux_digit_base+int_base then begin
    scan_int;
    if cur_val < 0 or cur_val > 9 then begin
      print_err("Improper digit place specified (");
      print_int(cur_val); print("), replaced by 0");
      cur_val:=0;
    end;
    m:=m+cur_val;
  end;
  scanned_result(eqtb[m].int)(int_val);
end

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("PUXsplitnumber",pux_split_number,0);

@ @<Assignments@>=
pux_split_number: begin
  scan_int;
  split_number(cur_val);
end;

@ The following procedure splits the integer parameter |n| to digit list and stores the number of digits into
|pux_digit_num|, the sign (1: positive or -1: negative) into |pux_num_sign|, and the digits into
the array |pux_nth_digit|. Since the largest |n| is $2^{31}$, n contains at most 10 digits.

@<PUTeX routines...@>=
procedure split_number (n:integer);
var k: 0..10;
begin
if n<0 then begin
  pux_num_sign := -1;
  negate(n)
  end
else
  pux_num_sign := 1;
k:=0;
repeat pux_nth_digit(k):=n mod 10; n:=n div 10; incr(k);
until n=0;
pux_digit_num:=k;
while k < 10 do begin
  pux_nth_digit(k) := 0;
  incr(k);
  end;
end;

@
@<scan and split the number@>=
begin
  scan_int;
  split_number(cur_val);
end

@ @<scan a CJK number with a possible selector and then  split it@>=
begin
  scan_int; saved_val:=cur_val;
  split_number(cur_val);
  if scan_keyword("offset") then begin
    scan_eight_bit_int;
    digit_base:=cur_val;
    if scan_keyword("sign") then begin
      scan_eight_bit_int;
      sign:=cur_val;
      end
    else
      sign:=negative_wchar_offset;
  end
  else digit_base:=0;
end

@ Using full-width arabic characters to show chinese numbers.
@<Basic print...@>=
procedure print_cjk_int(@!n:integer;digit_base,sign:integer);
var k:0..9; {index to current digit}
begin
  if pux_num_sign = -1 then print_wchar(local_names(sign));
  for k:=pux_digit_num-1 downto 0 do
    print_wchar(local_names(digit_base+pux_nth_digit(k)));
end;

@ @<using full-width arabic characters to print a CJK number@>=
print_cjk_int(cur_val,C_arabic_digit_offset,negative_wsym_offset)

@ @<print a CJK number with specified format@>=
print_cjk_int(saved_val,digit_base,sign)

@ @<scan a CJK name sequence number@>=
begin
  scan_eight_bit_int; saved_val:=cur_val;
  if scan_keyword("min") then begin
    scan_optional_equals; scan_eight_bit_int;
    min_val:=cur_val;
  end
  else begin
    print_err("Missing 'min' part ("); print("min 0 inserted)");
    error;
  end;
  if scan_keyword("max") then begin
    scan_optional_equals; scan_eight_bit_int;
    max_val:=cur_val;
  end
  else begin
    print_err("Missing 'max' part ("); print("max 255 inserted)");
    error;
  end;
  if scan_keyword("offset") then begin
    scan_optional_equals; scan_eight_bit_int;
    offset:=cur_val;
  end
  else begin
    print_err("Missing 'offset' part ("); print("offset 0 inserted)");
    error;
  end;
  if min_val <= saved_val and saved_val <= max_val then
    cur_val:=offset+saved_val-min_val
  else begin
    print_err("Number is out of the range ("); print("replaced with the min value)");
    cur_val:=offset;
    error;
  end;
end

@ @<print a CJK name sequence member@>=
print_wchar(local_names(cur_val))

@ A fix\_word is a {\sl scaled integers\/} that are multiples
of $2^{-20}$. In other words, a binary point is assumed to be twenty bit
positions from the right end of a binary computer word.
@d fw_unity == @"100000 {$2^{20}$, represents 1.00000}
@d fw_two == @"200000 {$2^{21}$, represents 2.00000}
@d fw_one_fifth==@"33333 {0.2}
@d convfix(#)== (#)*fw_unity div 1000

@<Types...@>=
@!fixword = integer; {this type is used for fixword (12.20) integers}


@ @<Declare the function called |print_fixword|@>=
procedure print_fixword(@!s:fixword); {prints fixword real, rounded to five
  digits}
var delta:fixword; {amount of allowable inaccuracy}
begin if s<0 then
  begin print_char("-"); negate(s); {print the sign, if negative}
  end;
print_int(s div fw_unity); {print the integer part}
print_char(".");
s:=10*(s mod fw_unity)+5; delta:=10;
repeat if delta>fw_unity then s:=s+@'200000000-50000; {round the last digit}
print_char("0"+(s div fw_unity)); s:=10*(s mod fw_unity); delta:=delta*10;
until s<=delta;
end;


@ The function |fw_times_sd| do the multiplication of a fixword and a scaled number.
The value of fixword is assumed between 16 and $-16$.
The function returns the result as a scaled number. (See also Sec. 571, 572 and 600.)

@<Declare the function called |fw_times_sd|@>=
function fw_times_sd (@!x:fixword; @!z:scaled) : scaled;
  {compute |f| times |s|}
var @!sw:scaled;
@!a,@!b,@!c,@!d:eight_bits; {byte variables}
@!alpha:integer;@!beta:1..16;
begin @<Replace |z|...@>;
if x>=0 then a:=x div @'100000000
else  begin x:=x+@'10000000000;
  x:=x+@'10000000000;
  a:=(x div @'100000000) + 128;
  end;
x:=x mod @'100000000; b:=x div @'200000;
x:=x mod @'200000; c:=x div @'400;
d:=x mod @'400;
sw:=(((((d*z)div@'400)+(c*z))div@'400)+(b*z))div beta;
if a=0 then fw_times_sd:=sw
else if a=255 then fw_times_sd:=sw-alpha
else fw_times_sd:=unity;
end;

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("PUXchar",pux_char_num,0);

@ @<Cases of |print_cmd_chr|...@>=
pux_char_num: print_esc("PUXchar");

@ @<Give improper hyphenation error for Chinese characters inside@>=
begin print_err("Improper "); print_esc("hyphenation");
@.Improper \\hyphenation...@>
  print(" will be flushed");
help2("Hyphenation exceptions can't contain Chinese characters")@/
  ("But continue; I'll forgive and forget.");
error;
end


@ @<Cases of |main_control| that build...@>=
mmode+pux_char_num: begin scan_wchar_num; cur_chr:=cur_val;
  print_err("Chinese character is ignored in math mode");
  help1("Did you forget putting it into an \hbox?");
  error;
  end;
mmode+pux_char_given: begin
  print_err("Chinese character is ignored in math mode");
  help1("Did you forget putting it into an \hbox?");
  error;
  end;

@ @<Cases of |print_cmd_chr|...@>=
pux_char_given: begin print_esc("PUXchar"); print_hex(chr_code);
  end;

@* \[58] All about spaces.
 
@d is_tail_forbidden(#) == type_code(#) = tail_forbidden
@d is_head_forbidden(#) == type_code(#) = head_forbidden
@d is_head_forbidden_wchar(#) == ((# > 255) and (type_code(#) = head_forbidden))
@d is_punc_wchar(#) == ((# > 255) and (type_code(#) <> 0))

@
@<Global variables@>=
@!main_cf:internal_cfont_number; {the current chinese font}
@!math_mode_save:-mmode..mmode;
@!prev_main_cf:internal_cfont_number; {the current chinese font}
@!pre_undet_glue_ptr:pointer; {point to the node just before a undetermined glue}
@!undet_glue_ptr:pointer; {point to the undetermined glue}
@!cglue_ptr:pointer;
@!cglue_spec:pointer;
@!pre_glue_char_ptr:pointer;
@!outer_tail:pointer;
@!hbox_tail:pointer;
@!in_set_box:boolean;

@ @<Initialization of global variables done in the |main_control| procedure@>=
pre_undet_glue_ptr:=null;
pre_glue_char_ptr:=null;

@
@d tail_append_glue(#)==
begin cglue_ptr:=get_node(small_node_size); cglue_spec:=#;
type(cglue_ptr):=glue_node; subtype(cglue_ptr):=normal;
leader_ptr(cglue_ptr):=null; glue_ptr(cglue_ptr):=cglue_spec;
incr(glue_ref_count(cglue_spec));
tail_append(cglue_ptr);
end

@ Here is the check done before switching to regular character string.
@<If the preceding node is wchar node, then append a cespace@>=
if tail=head then begin
  if mode=-hmode then begin {beginning of a restricted hlist}
    outer_tail:=nest[nest_ptr-1].tail_field;
    if pre_undet_glue_ptr<>null then begin
      if outer_tail=link(pre_undet_glue_ptr) and pre_glue_char_ptr<>null
             and is_wchar_node(pre_glue_char_ptr) then begin
        decr(glue_ref_count(glue_ptr(outer_tail)));
        glue_ptr(outer_tail):=cfont_ceglue_spec[prev_main_cf];
        incr(glue_ref_count(cfont_ceglue_spec[prev_main_cf]));
        end;
      pre_undet_glue_ptr:=null;
      end;
    end;
  end
else if is_char_node(tail) and is_wchar_node(tail) then begin
  if is_head_forbidden(cur_chr) then tail_append(new_penalty(inf_penalty));
  tail_append_glue(cfont_ceglue_spec[main_cf]);
  end
else if pre_undet_glue_ptr<>null and link(pre_undet_glue_ptr)=tail and
     pre_glue_char_ptr<>null and is_wchar_node(pre_glue_char_ptr) then begin
  decr(glue_ref_count(glue_ptr(tail)));
  glue_ptr(tail):=cfont_ceglue_spec[prev_main_cf];
  incr(glue_ref_count(cfont_ceglue_spec[prev_main_cf]));
  end;
pre_undet_glue_ptr:=null;
pre_glue_char_ptr:=null;


@ If the next token come after the math shift \$ is a wide character, then
a cespace is appended first.
@<If the token is a wide character, then append a cspace@>=
if cur_cmd=pux_char_num then
  begin scan_wchar_num; cur_chr:=cur_val; cur_cmd:=pux_char_given;
  end;
if cur_cmd=letter or cur_cmd=other_char or cur_cmd=pux_char_given then
  if is_wchar(cur_chr) then
    if is_punc_wchar(cur_chr) then begin
      if is_head_forbidden(cur_chr) then tail_append(new_penalty(inf_penalty));
      tail_append_glue(zero_glue);
      end
    else
      tail_append_glue(cfont_ceglue_spec[main_cf])


@
@<Append double-byte character |cur_chr|...@>=
main_cf:=cur_cfont;
@<If the current wchar is at the beginning of a restricted hlist that
  is after a undetermined spacer, then we have to determine that space.
  When it is done |goto save_cur_wchar|@>;
@<If the previous node is an undetermined glue, then make it certain and
  |goto save_cur_wchar|@>;
if not is_char_node(tail) then goto save_cur_wchar;
main_loop_wchar+1:@<the previous node is a character node, so we have to append
a glue first@>;
save_cur_wchar:
fast_get_avail(lig_stack);
font(lig_stack):=main_cf;
character(lig_stack):=cur_chr;
tail_append(lig_stack);@/
@<Prepare a nonbreak space if the current wide character is not allowed to
  appear at the end of line@>;
fetch_next_tok:get_next; {set only |cur_cmd| and |cur_chr|, for speed}
@<Check the lookahead character@>;
x_token; {now expand and set |cur_cmd|, |cur_chr|, |cur_tok|}
@<Check the lookahead character@>;
if cur_cmd=char_num then
  begin scan_char_num; cur_chr:=cur_val; cur_cmd:=char_given; goto next_is_a_char;
  end;
if cur_cmd=pux_char_num then
  begin scan_wchar_num; cur_chr:=cur_val;
  if is_punc_wchar(cur_chr) then
    if is_head_forbidden(cur_chr) then tail_append(new_penalty(inf_penalty));
  tail_append_glue(cfont_glue_spec[main_cf]);
  goto save_cur_wchar;
  end;
{next token is not a character token}
if cur_cmd=math_shift then
  if is_punc_wchar(character(lig_stack)) then
    tail_append_glue(zero_glue)
  else
    tail_append_glue(cfont_ceglue_spec[main_cf]);
goto reswitch;
next_is_a_char: begin@/
  if cur_chr<256 then
    if is_head_forbidden(cur_chr) then tail_append(new_penalty(inf_penalty));
  if is_punc_wchar(character(lig_stack)) then
    tail_append_glue(zero_glue)
  else
    tail_append_glue(cfont_ceglue_spec[main_cf]);
  goto main_loop+1;
  end


@ @<If the current wchar is at the beginning...@>=
if tail=head then begin {beginning of a restricted hlist}
  if mode=-hmode then begin
    outer_tail:=nest[nest_ptr-1].tail_field;
    if pre_undet_glue_ptr<>null then begin
      if outer_tail=link(pre_undet_glue_ptr) then begin
        undet_glue_ptr:=outer_tail;
        @<Modify the undetermined glue according the type of pre-glue character@>;
        end;
      pre_undet_glue_ptr:=null;
      end;
    end;
  goto save_cur_wchar;
  end


@ @<Modify the undetermined glue...@>=
decr(glue_ref_count(glue_ptr(undet_glue_ptr)));
if pre_glue_char_ptr<>null and is_wchar_node(pre_glue_char_ptr) then begin
  glue_ptr(undet_glue_ptr):=cfont_glue_spec[prev_main_cf];
  incr(glue_ref_count(cfont_glue_spec[prev_main_cf]));
  pre_glue_char_ptr:=null;
  end
else begin
  glue_ptr(undet_glue_ptr):=cfont_ceglue_spec[prev_main_cf];
  incr(glue_ref_count(cfont_ceglue_spec[prev_main_cf]));
  end

@ @<If the previous node is an undetermined glue...@>=
if pre_undet_glue_ptr<>null then begin
  if link(pre_undet_glue_ptr)=tail then begin
    undet_glue_ptr:=tail;
    @<Modify the undetermined glue according the type of pre-glue character@>;
    pre_undet_glue_ptr:=null;
    goto save_cur_wchar;
    end;
  pre_undet_glue_ptr:=null;
  end


@ @<the previous node is a character node...@>=
if is_wchar_node(tail) then begin
  if is_head_forbidden_wchar(cur_chr) then
    tail_append(new_penalty(inf_penalty));
  tail_append(new_glue(cfont_glue_spec[main_cf]));
  end
else begin {previous node is a single byte character}
  if is_punc_wchar(cur_chr) then begin
    if is_head_forbidden(cur_chr) then tail_append(new_penalty(inf_penalty));
    tail_append_glue(zero_glue);
    end
  else begin
    if is_head_forbidden(character(tail)) then tail_append(new_penalty(inf_penalty));
    tail_append_glue(cfont_ceglue_spec[main_cf]);
    end;
  end



@ For those Chinese puncuations that shoudn't appear in the line end,
we append a penalty node to prevent line boken after it.
@<Prepare a nonbreak space if the current wide...@>=
if is_punc_wchar(cur_chr) then
  if is_tail_forbidden(cur_chr) then tail_append(new_penalty(inf_penalty))

@ @<Check the lookahead character@>=
if cur_cmd=letter or cur_cmd=other_char or cur_cmd=pux_char_given or
   cur_cmd=char_given then
  if is_wchar(cur_chr) then begin
    if is_punc_wchar(cur_chr) then
      if is_head_forbidden(cur_chr) then tail_append(new_penalty(inf_penalty));
    tail_append_glue(cfont_glue_spec[main_cf]);
    goto save_cur_wchar;
    end
  else goto next_is_a_char


@ @<Look ahead for next character. If it is a wide...@>=
get_next; {set only |cur_cmd| and |cur_chr|, for speed}
if cur_cmd=letter or cur_cmd=other_char then
  if is_wchar(cur_chr) then goto main_loop_lookahead+2
  else goto main_loop_lookahead+1;
if cur_cmd=char_given then goto main_loop_lookahead+1;
if cur_cmd=pux_char_given then goto main_loop_lookahead+2;
x_token; {now expand and set |cur_cmd|, |cur_chr|, |cur_tok|}
if cur_cmd=letter or cur_cmd=other_char then
  if is_wchar(cur_chr) then goto main_loop_lookahead+2
  else goto main_loop_lookahead+1;
if cur_cmd=char_given then goto main_loop_lookahead+1;
if cur_cmd=char_num then
  begin scan_char_num; cur_chr:=cur_val; goto main_loop_lookahead+1;
  end;
if cur_cmd=pux_char_num then
  begin scan_wchar_num; cur_chr:=cur_val; goto main_loop_lookahead+2;
  end;
if cur_cmd=no_boundary then bchar:=non_char;
main_loop_lookahead+2: cur_r:=bchar; lig_stack:=null; goto main_lig_loop;
main_loop_lookahead+1: adjust_space_factor;
fast_get_avail(lig_stack); font(lig_stack):=main_f;
cur_r:=qi(cur_chr); character(lig_stack):=cur_r;
if cur_r=false_bchar then cur_r:=non_char {this prevents spurious ligatures}


@ @<Cases of |main_control| that handle spacer@>=
hmode+spacer: @<Lookahead and determine the type of spacer to append@>;
hmode+ex_space: @<Lookahead and determine the type of |ex_spacer| to append@>;
mmode+ex_space: begin if pux_xspace=0 then get_x_token; {lookahead}
                goto append_normal_space;
                end;
hmode+pux_space:@<Handle \PUTeX space command@>;
mmode+pux_space:begin
  print_err("This space command is ignored in math mode");
  help1("Did you forget putting it into an \hbox?");
  error;
  end;

@ @<Setup |hbox_tail| and package@>=
if in_set_box then package(0)
else begin
  if tail<>head and is_char_node(tail) then
    hbox_tail:=tail
  else
    hbox_tail:=null;
  package(0);
  get_x_token;
  if cur_cmd<>spacer then hbox_tail:=null;
  back_input;
  end

@ @<Lookahead and determine the type of spacer to append@>=
begin
if pux_xspace=0 then begin
  if tail<>head and is_char_node(tail) then
    pre_glue_char_ptr:=tail
  else
    pre_glue_char_ptr:=null;
  get_x_token; {lookahead}
  if cur_cmd=char_num then
    begin scan_char_num; cur_chr:=cur_val; cur_cmd:=char_given;
    end
  else if cur_cmd=pux_char_num then
    begin scan_wchar_num; cur_chr:=cur_val; cur_cmd:=pux_char_given;
    end;
  if cur_cmd=letter or cur_cmd=other_char or cur_cmd=char_given or
     cur_cmd=pux_char_given then
    if is_wchar(cur_chr) then begin
      main_cf:=cur_cfont;
      if pre_glue_char_ptr<>null then goto main_loop_wchar+1;
      if hbox_tail<>null and is_wchar_node(hbox_tail) then begin
        tail_append_glue(cfont_glue_spec[main_cf]);
        hbox_tail:=null;
        end
      else begin
        tail_append_glue(cfont_ceglue_spec[main_cf]);
        if is_punc_wchar(cur_chr) then
          if is_head_forbidden(cur_chr) then tail_append(new_penalty(inf_penalty));
        if hbox_tail<>null then hbox_tail:= null;
        end;
      goto save_cur_wchar;
      end
    else if (pre_glue_char_ptr<>null and is_wchar_node(tail))
         or (hbox_tail<>null and is_wchar_node(hbox_tail)) then begin
        tail_append_glue(cfont_ceglue_spec[cur_cfont]);
        hbox_tail:=null;
        goto main_loop;
        end;
  prev_main_cf:=cur_cfont;
  pre_undet_glue_ptr:=tail;
  if pre_glue_char_ptr<>null and is_wchar_node(pre_glue_char_ptr) then begin
    tail_append_glue(cfont_ceglue_spec[cur_cfont]);
    goto reswitch;
    end;
  end;
if space_factor=1000 then goto append_normal_space
else begin app_space;
  if pux_xspace=0 then goto reswitch else goto big_switch;
  end;
end

@ @<Lookahead and determine the type of |ex_spacer| to append@>=
begin
if pux_xspace=0 then begin
  get_x_token; {lookahead}
  if cur_cmd=char_num then
    begin scan_char_num; cur_chr:=cur_val; cur_cmd:=char_given;
    end;
  if cur_cmd=pux_char_num then
    begin scan_wchar_num; cur_chr:=cur_val; cur_cmd:=pux_char_given;
    end;
  if cur_cmd=letter or cur_cmd=other_char or cur_cmd=char_given or cur_cmd=pux_char_given then
    if is_wchar(cur_chr) then begin
      main_cf:=cur_cfont;
      if tail<>head and is_char_node(tail) then
        if is_wchar_node(tail) then
          goto append_normal_space
        else
          goto main_loop_wchar+1;
      tail_append_glue(cfont_glue_spec[main_cf]);
      goto save_cur_wchar;
      end
    else if tail<>head and is_char_node(tail) then
      if is_wchar_node(tail) then begin
        tail_append_glue(cfont_ceglue_spec[cur_cfont]);
        goto main_loop;
        end;
  if tail<>head and is_char_node(tail) then
    if is_wchar_node(tail) then begin
      tail_append_glue(cfont_glue_spec[cur_cfont]);
      goto reswitch;
      end;
  prev_main_cf:=cur_cfont;
  pre_undet_glue_ptr:=tail;
  end;
goto append_normal_space;
end

@
@d pux_space_code=0
@d pux_exspace_code=1
@d pux_cspace_code=2
@d pux_cespace_code=3
@<Put each of \TeX's primitives into the hash table@>=
primitive("PUXspace",pux_space,pux_space_code);
primitive("PUXexspace",pux_space,pux_exspace_code);
primitive("PUXcspace",pux_space,pux_cspace_code);
primitive("PUXcespace",pux_space,pux_cespace_code);

@ @<Cases of |print_cmd_chr|...@>=
pux_space: case chr_code of
  pux_space_code: print_esc("PUXspace");
  pux_exspace_code: print_esc("PUXexspace");
  pux_cspace_code: print_esc("PUXcspace");
  othercases print_esc("PUXcespace")
  endcases;

@ @<Handle \PUTeX space command@>=
case cur_chr of
  pux_space_code: begin get_x_token;
    if space_factor=1000 then goto append_normal_space;
    app_space;
    if pux_xspace=0 then goto reswitch else goto big_switch;
    end;
  pux_exspace_code: begin get_x_token; goto append_normal_space;
    end;
  pux_cspace_code: tail_append(new_glue(cfont_glue_spec[cur_cfont]));
  othercases tail_append(new_glue(cfont_ceglue_spec[cur_cfont]))
  endcases

@* \[59] CJK font face definition table.

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("PUXcfacedef",pux_cface_def,0);

@ @<Cases of |print_cmd_chr|...@>=
pux_cface_def: print_esc("PUXcfacedef"); {TCW}

@ @<Assignments@>=
pux_cface_def: new_cface(a);


@ @<Constants...@>=
@!cface_base=0; {CJK font face base}
@!null_cface=0; {null CJK font faces}

@
@<Types...@>=
@!internal_cface_number=cface_base..max_cface;

@ The CJK font face definition table is implemented by parallel arrays as follows.
@d regular=0
@d italic=@"40 {bit 6: italic flag}
@d underline=@"20 {bit 5: underline flag}
@d strikeout=@"10 {bit 4: strikeout flag}
@d inverse=@"08 {bit 3: inverse flag}
@d rotated=@"01 {bit 0: rotation flag}
@d default_cface_weight==400
@d default_cface_style=regular
@d default_cface_fw_width==fw_unity
@d default_cface_fw_height==fw_unity
@d cface_id_text(#)==text(cface_id_base+#)

@<Global variables@>=
@!cface_ptr:internal_cface_number;
  {index of the first unused entry}
@!cface:array[internal_cface_number] of str_number;
  {CJK font face identifier}
@!cface_name:array[internal_cface_number] of str_number;
  {CJK font face name}
@!cface_charset:array[internal_cface_number] of eight_bits;
  {CJK font charset}
@!cface_weight:array[internal_cface_number] of 1..1000;
  {CJK font weight}
@!cface_style:array[internal_cface_number] of eight_bits;
  {CJK font style}
@!cface_fw_width:array[internal_cface_number] of fixword;
  {CJK font width ratio}
@!cface_fw_height:array[internal_cface_number] of fixword;
  {CJK font heigh ratio}
@!cface_fw_depth:array[internal_cface_number] of fixword; 
  {CJK font depth ratio}
@!cface_csp_width:array[internal_cface_number] of integer;
  {CJK font c-space width}
@!cface_csp_shrink:array[internal_cface_number] of integer;
  {CJK font c-space shrink}
@!cface_csp_stretch:array[internal_cface_number] of integer;
  {CJK font c-space stretch}
@!cface_cesp_width:array[internal_cface_number] of integer;
  {CJK font ce-space width}
@!cface_cesp_shrink:array[internal_cface_number] of integer;
  {CJK font ce-space shrink}
@!cface_cesp_stretch:array[internal_cface_number] of integer;
  {CJK font ce-space stretch}
@!cface_fw_default_depth:fixword;


@ @<Put each of \TeX's primitives into the hash table@>=
primitive("PUXsetdefaultcface",pux_set_default_cface,int_base+pux_default_cface_code);

@ @<Cases of |print_cmd_chr|...@>=
pux_set_default_cface: print_esc("PUXsetdefaultcface"); {TCW}

@ @<Assignments@>=
pux_set_default_cface: begin p:=cur_chr;
  @<Get the next non-blank non-call token@>;
  if cur_cmd = pux_set_cface then
    word_define(p,cur_chr)
  else begin
    print_err("Here should put a CJK font face command. ");
    print("The dafault CJK font face remains unchanged");
    error;
  end;
  end;


@ @<PUTeX routines that will be used by TeX routines@>=
procedure reset_cface_cspace (face_num:integer);
begin
  cface_csp_width[face_num]:=g_cspace_width;@/
  cface_csp_shrink[face_num]:=g_cspace_shrink;@/
  cface_csp_stretch[face_num]:=g_cspace_stretch;@/
end;

@ @<PUTeX routines that will be used by TeX routines@>=
procedure reset_cface_cespace (face_num:integer);
begin
  cface_cesp_width[face_num]:=g_cespace_width;@/
  cface_cesp_shrink[face_num]:=g_cespace_shrink;@/
  cface_cesp_stretch[face_num]:=g_cespace_stretch;@/
end;

@ Setup default and null CJK font faces.
@<Initialize table...@>=
cur_cface:=null_cface; eq_type(cur_cface_loc):=data;
eq_level(cur_cface_loc):=level_one;@/
cface_fw_default_depth:=convfix(puxg_cface_depth);
cface_ptr:=cface_base+1;

cface[null_cface]:="nullcface";@/
cface_name[null_cface]:="nullcjkface";@/
cface_charset[null_cface]:=0;@/
cface_weight[null_cface]:=400; {normal weight}@/
cface_style[null_cface]:=0;@/
cface_fw_width[null_cface]:=0;@/
cface_fw_height[null_cface]:=0;@/
cface_fw_depth[null_cface]:=0;@/
reset_cface_cspace(null_cface);@/
reset_cface_cespace(null_cface);@/

@ The function |find_cface_num| searches the CJK font face definition table
for the entry with the same identifier as |id|. The entry index is return if found;
otherwise, the current value of |cface_ptr| is return.

@d cface_found(#)==((#)<cface_ptr)

@<Declare the function called |find_cface_num|@>=
function find_cface_num(id:str_number):internal_cface_number;
  label done;
  var f:internal_cface_number; {runs through existing faces}
  begin
  f:=cface_base;
  while (f < cface_ptr) do
    begin
    if str_eq_str(id, cface[f]) then goto done;
    incr(f);
    end;
 done:find_cface_num:=f;
  end;

@ @<Declare subprocedures for |prefixed_command|@>=
@<Declare the function called |print_fixword|@>@;
procedure new_cface(@!a:small_number);
label done, done1, common_ending;
var u:pointer; {user's chinese face identifier}
@!t:str_number; {name for the frozen font identifier}
@!id:str_number; {CJK font face identifier}
@!face_name:str_number; {CJK font face name}
@!charset:integer; {CJK font charset}
@!weight:integer; {CJK font weight}
@!style:integer; {CJK font style}
@!w:integer; {CJK font width ratio}
@!h:integer; {CJK font height ratio}
@!d:integer; {CJK font depth ratio}
@!fix_w:fixword; {CJK font width ratio}
@!fix_h:fixword; {CJK font height ratio}
@!fix_d:fixword; {CJK font depth ratio}
@!f:internal_cface_number; {runs through existing faces}
@!k:integer;@/
@<Other variables used by |new_cface|@>@;
begin if job_name=0 then open_log_file;
  {avoid confusing \.{texput} with the font name}
get_r_token; u:=cur_cs;
if u>=hash_base then t:=text(u)
else if u>=single_base then
  if u=null_cs then t:="CFACE"@+else t:=u-single_base
else  begin old_setting:=selector; selector:=new_string;
  print("CFACE"); print(u-active_base); selector:=old_setting;
  str_room(1); t:=make_string;
  end;
define(u,pux_set_cface,null_cface);
scan_optional_equals;
@<Setup variables before scanning CJK font face parameters@>;
@<Scan CJK font face identifier@>;
@<Scan CJK font face name@>;
@<Scan optional CJK font face definition parameters@>;
@<If the face name is missing, then ignore this face deinition@>;
@<If this Chinese face has already been loaded, then |goto common_ending|@>;
@<Setup this new Chinese face@>;
common_ending: equiv(u):=f; eqtb[cface_id_base+f]:=eqtb[u]; cface_id_text(f):=t;
end;

@ @<Setup variables before scanning CJK font face parameters@>=
charset:=pux_charset; {set to the base charset of document}
w:=1000; h:=1000; d:=puxg_cface_depth; @/
weight:=400; {normal weight}@/
style:=0; {regular style}
if puxg_rotate_ctext<>0 then style:=style+rotated;
f:=null_cface


@ @<Scan CJK font face identifier@>=
id:=scan_name;
if id > 0 then
  begin
  f:=find_cface_num(id);
  if (f < cface_ptr) then
    begin
    flush_string; id:=cface[f]; {for saving string pool sapce}
    f:=null_cface;
    print_err("The Chinese face id ("); print(id);
    print(") is already used"); error;
    end;
  end
else
  begin
  print_err("Missing CJK font face identifier"); error;
  end

@ @<Scan CJK font face name@>=
begin
face_name:=scan_name;
if face_name > 0 then
  begin
  k:=cface_base;
  while (k < cface_ptr) do
    begin
    if str_eq_str(face_name, cface_name[k]) then
      begin
      flush_string;
      face_name:=cface_name[k]; f:=k;
      goto done1;
      end;
    incr(k);
    end;
  end
else
  begin
  print_err("Missing CJK font face name"); error;
  face_name:=cface_name[null_cface];
  f:=null_cface;
  end;
done1: end

@ @<Other variables used by |new_cface|@>=
@!i_flag:boolean; {italic flag}
@!u_flag:boolean; {underline flag}
@!s_flag:boolean; {strikeout flag}
@!r_flag:boolean; {rotation flag}
@!v_flag:boolean; {inverse flag}
@!more_param:boolean; {have more parameters to come}

@ @<Scan optional CJK font face definition parameters@>=
i_flag:=false; u_flag:=false; s_flag:=false;@/
r_flag:=false; v_flag:=false;@/
more_param:=true;
while more_param do
  begin
  @<Get the next non-blank non-call token@>;
  if cur_cmd=letter then
    case cur_chr of
      'c','C': @<Scan the CJK font charset@>;
      'w','W': @<Scan the CJK font width@>;
      'h','H': @<Scan the CJK font height@>;
      'd','D': @<Scan the CJK font depth@>;
      't','T': @<Scan the CJK font weight@>;
      's','S': @<Scan the CJK font style@>;
      othercases more_param:=false;
    endcases
  else more_param:=false;
  end;
back_input


@
@<Scan the CJK font charset@>=
begin scan_optional_equals;@/
scan_int; 
if (cur_val<0)or(cur_val>255) then
  begin print_err("Improper `charset' value (");
  print_int(charset); print("), replaced by default charset");
  help2("I can only handle nonnegative charset value up to 255,")@/
  ("so I've changed what you said to default charset.");
  error;
  end
else
  charset:=cur_val;
end

@
@<Scan the CJK font width@>=
begin scan_optional_equals;@/
scan_int; w:=cur_val;
if (w<=0)or(w>1000) then
  begin print_err("Improper `width' value (");
  print_int(w); print("), replaced by 1000");
  help2("I can only handle fonts at positive width ratio that are less")@/
  ("than or equal to 1000, so I've changed what you said to 1000.");
  error; w:=1000;
  end;
end

@ @<Scan the CJK font height@>=
begin scan_optional_equals;@/
scan_int; h:=cur_val;
if (h<=0)or(h>1000) then
  begin print_err("Improper `height value (");
  print_int(h); print("), replaced by 1000");
  help2("I can only handle fonts at positive height ratio that are less")@/
  ("than or equal to 1000, so I've changed what you said to 1000.");
  error; h:=1000;
  end;
end

@ @<Scan the CJK font depth@>=
begin scan_optional_equals;@/
scan_int; d:=cur_val;
if (d<0)or(d>1000) then
  begin print_err("Improper `depth' value (");
  print_int(d); print("), replaced by 0.2");
  help3("I can only handle fonts at nonegative depth ratio that are less")@/
  ("than or equal to 1000, so I've changed what you said to")@/
  ("the current \puxgCfaceDepth value.");
  error; d:=puxg_cface_depth;
  end;
end

@ @<Scan the CJK font weight@>=
begin scan_optional_equals;@/
scan_int; weight:=cur_val;
if (weight < 0) or (weight > 1000) then
  begin print_err("Illegal CJK font weight has been changed to 400");@/
  help1("The font weight must be between 1 and 1000.");
  int_error(cur_val); weight:=400; {normal weight}
  end;
end

@
@<Scan the CJK font style@>=
begin scan_optional_equals;@/
@<Get the next non-blank non-call token@>;
if cur_cmd=letter then
  case cur_chr of
  "i", "I": if not i_flag then begin style:=style+italic; i_flag:=true; end;
  "u", "U": if not u_flag then begin style:=style+underline; u_flag:=true; end;
  "s", "S": if not s_flag then begin style:=style+strikeout; s_flag:=true; end;
  "r", "R": if not r_flag then @<Set CJK font rotation style@>;
  "v", "V": if not v_flag then begin style:=style+inverse; v_flag:=true; end;
  othercases@/
    begin print_err("Illegal CJK font style setting has been ignored");@/
    print(" ("); print(cur_chr); print(")"); back_error;
      {fix the case when cur\_chr is a double-byte char}
    help2("The CJK font style setting should use characters:")@/
    ("i:italic, u:underline, s:strikeout, r:rotated, v:reversed");
    end;
  endcases;@/
end

@ @<Set CJK font rotation style@>=
begin
if puxg_rotate_ctext<>0 then
  style:=style-rotated
else
  style:=style+rotated;
r_flag:=true;
end

@ @<If the face name is missing, then ignore this face deinition@>=
if f=null_cface then
  goto common_ending


@ @<If this Chinese face has...@>=
fix_w:=convfix(w);
fix_h:=convfix(h);
fix_d:=convfix(d);
if f <> null_cface then
  if weight=cface_weight[f] and style=cface_style[f] then
    if fix_w=cface_fw_width[f] and fix_h=cface_fw_height[f] and fix_d=cface_fw_depth[f] then@/
      goto common_ending

@
@<Setup this new Chinese face@>=
if cface_ptr <= max_cface then
  begin
  f:=cface_ptr;
  cface[f]:=id;
  cface_name[f]:=face_name;@/
  cface_charset[f]:=charset;@/
  cface_weight[f]:=weight;@/
  cface_style[f]:=style;@/
  if style mod 2 = 1 then begin
    cface_fw_width[f]:=fix_w; cface_fw_height[f]:=fix_h;
    end
  else begin
    cface_fw_width[f]:=fix_h; cface_fw_height[f]:=fix_w;
    end;
  cface_fw_depth[f]:=fix_d;@/
  reset_cface_cspace(f);@/
  reset_cface_cespace(f);@/
  incr(cface_ptr);
  end
else begin
  f:=null_cface;
  print_err("CJK font Face definition table overflow"); error;
  end
  
@* \[59] CJK font definition table.

@ @<Constants...@>=
@!cfont_base=font_max_limit+1; {CJK font base}
@!cfont_max=font_max_limit+1+cfont_max_limit; {maximum internal chinese font number}

@
@<Types...@>=
@!internal_cfont_number=cfont_base..cfont_max;

@ @<Initialize table entries...@>=
cur_cfont:=default_cfont; eq_type(cur_cfont_loc):=data;
eq_level(cur_cfont_loc):=level_one;@/

@ @<Global variables@>=
@!cfont_ptr:internal_cfont_number;
@!cfont_face:array[internal_cfont_number] of internal_cface_number;
  {CJK font face name}
@!cfont_dsize:array[internal_cfont_number] of scaled;
  {CJK font design size}
@!cfont_size:array[internal_cfont_number] of scaled;
  {CJK font size}
@!cfont_width:array[internal_cfont_number] of scaled;
  {CJK font width}
@!cfont_height:array[internal_cfont_number] of scaled;
  {CJK font heigh}
@!cfont_depth:array[internal_cfont_number] of scaled;
  {CJK font depth}
@!cfont_glue_spec:array[internal_cfont_number] of pointer;
  {CJK font inter-character space}
@!cfont_ceglue_spec:array[internal_cfont_number] of pointer;
  {CJK font inter-character space}
@!cfont_used:array[internal_cfont_number] of boolean;
  {has a character from this chinese font actually appeared in the output?}


@ @<Set init...@>=
for k:=cfont_base to cfont_max do cfont_used[k]:=false;
cfont_face[null_cfont]:=null_cface;
cfont_dsize[null_cfont]:=0;
cfont_size[null_cfont]:=0;
cfont_width[null_cfont]:=0;
cfont_height[null_cfont]:=0;
cfont_depth[null_cfont]:=0;

@ @<Initialize table entries...@>=
cfont_ptr:=default_cfont;

@ @<Declare PUTeX subprocedures for |prefixed_command|@>=
procedure set_cglue_spec(n:integer);
var cface_num:integer;
begin
  cface_num:=cfont_face[n];
  width(cfont_glue_spec[n]):=xn_over_d(cfont_size[n], cface_csp_width[cface_num], 1000);
  shrink(cfont_glue_spec[n]):=xn_over_d(cfont_size[n], cface_csp_shrink[cface_num], 1000);
  stretch(cfont_glue_spec[n]):=xn_over_d(cfont_size[n], cface_csp_stretch[cface_num], 1000);
end;

@ @<Declare PUTeX subprocedures for |prefixed_command|@>=
procedure set_ceglue_spec(n:integer);
var cface_num:integer;
begin
  cface_num:=cfont_face[n];
  width(cfont_ceglue_spec[n]):=xn_over_d(cfont_size[n], cface_cesp_width[cface_num], 1000);
  shrink(cfont_ceglue_spec[n]):=xn_over_d(cfont_size[n], cface_cesp_shrink[cface_num], 1000);
  stretch(cfont_ceglue_spec[n]):=xn_over_d(cfont_size[n], cface_cesp_stretch[cface_num], 1000);
end;


@ @<Initialization of global variables done in the |main_control| procedure@>=
cfont_glue_spec[null_cfont]:=new_spec(zero_glue);
cfont_ceglue_spec[null_cfont]:=new_spec(zero_glue);

@ @<Other local variables used by procedure |new_font|@>=
@!face_id:str_number; {Chinese face name fetched from |\\font| command}
@!jj:internal_cface_number;
@!cface_num:internal_cface_number;
@!ds:integer;
@!dsize:scaled;
@!size:scaled;

@
@<Define a CJK font and then goto |common_ending|@>=
begin
define(u, set_cfont, null_cfont);
cface_num:=pux_default_cface;@/
@<Fetch the Chinese face name@>;
@<Fetch the font design size and compute font 'at' size@>;
@<If this CJK font has already been loaded, set |f| to the internal
  CJK font number and |goto| common\_ending@>;
f:=make_cfont(cface_num,dsize,size);@/
goto common_ending;
end;

@
@d is_letter(#)==((#>='A' and #<='Z') or (#>='a' and #<='z'))
@<Fetch the Chinese face name@>=
jj:=j;
j:=j+5; {skip the prefix 'CFONT'}
while is_letter(str_pool[j]) do {fixme for wchar}
  begin
  append_char(str_pool[j]);
  incr(j);
  end;
if pool_ptr <> str_start[str_ptr] then
  begin
  face_id:=make_string;@/
  cface_num:=find_cface_num(face_id);
  flush_string;
  end
else
  begin
  print_err("Missing Chinese face identifier"); error;
  end;

@
@d is_digit(#)==(# >= '0' and # <= '9')
@<Fetch the font design size and compute font 'at' size@>=
ds:=0;
while is_digit(str_pool[j]) do
  begin
  ds:= ds*10+(str_pool[j]-'0');
  incr(j);
  end;
if ds=0 then
  begin
  print_err("Missing CJK font size specification, replaced by 10pt");
  ds:=10; {set to default size: 10pt}
  error;
  end;
dsize:=mult_integers(ds,unity);
if s=-1000 then
  size:=dsize
else
  if s>=0 then size:=s
  else size:=xn_over_d(dsize, -s, 1000);

@
@d defined_cfont(#)==(#)<cfont_ptr
@d undefined_cfont(#)==(#)=cfont_ptr

@<Declare the procedure called |check_cfont|@>=
function check_cfont(@!cface_num:internal_cface_number;@!size:scaled):internal_cfont_number;
label done;
var f:internal_cfont_number;
begin
f:=cfont_base+1;
while (f<cfont_ptr) do
  begin
  if cface_num=cfont_face[f] and size=cfont_size[f] then goto done;
  incr(f);
  end;
done:check_cfont:=f;
end;

@ @<If this CJK font has already been...@>=
f:=check_cfont(cface_num,size);
if defined_cfont(f) then goto common_ending;


@
@<Declare the procedure called |make_cfont|@>=
function make_cfont(cfn:internal_cface_number; dsize, size:scaled):internal_cfont_number;
begin
if cfont_ptr <= cfont_max then
  begin
  cfont_face[cfont_ptr]:=cfn;@/
  cfont_dsize[cfont_ptr]:=dsize;@/
  cfont_size[cfont_ptr]:=size;@/
  cfont_width[cfont_ptr]:=fw_times_sd(cface_fw_width[cfn], size);@/
  cfont_height[cfont_ptr]:=fw_times_sd(cface_fw_height[cfn], size);@/
  cfont_depth[cfont_ptr]:=fw_times_sd(cface_fw_depth[cfn], size);@/
  cfont_glue_spec[cfont_ptr]:=new_spec(zero_glue);
  set_cglue_spec(cfont_ptr);
  cfont_ceglue_spec[cfont_ptr]:=new_spec(zero_glue);
  set_ceglue_spec(cfont_ptr);
  make_cfont:=cfont_ptr;@/
  incr(cfont_ptr);
  end
else
  begin
  print_err("CJK font table overflow"); error;
  end
end;


@ @<Cases of |print_cmd_chr|...@>=
set_cfont:begin print("select CJK font "); slow_print(cface[cfont_face[chr_code]]);
  print(" at ("); print_scaled(cfont_size[chr_code]); print("pt"); print(")");
  end;

@* \[57] Matching faces.
@d min_ectbl=0
@d max_ectbl=255

@ @<Types...@>=
@!internal_ectbl_number=min_ectbl..max_ectbl;


@ @<Global variables@>=
@!ectbl_eface_name:array[internal_ectbl_number] of str_number;
  {the table of English face names }
@!ectbl_ptr:internal_ectbl_number;
  {index to the first unused entry}

@ |ectbl_cface_num| table entries are already initialized in section 232.

@<Initialize table entries...@>=
ectbl_ptr:=min_ectbl;
equiv(ectbl_cface_num_base):=null_cface;
eq_type(ectbl_cface_num_base):=data;
eq_level(ectbl_cface_num_base):=level_one;
for k:=ectbl_cface_num_base+1 to font_matching_table_base-1 do
  eqtb[k]:=eqtb[ectbl_cface_num_base];


@ @<Put each of \TeX's primitives into the hash table@>=
primitive("PUXfacematch",pux_face_match,0);

@ @<Cases of |print_cmd_chr|...@>=
pux_face_match: print_esc("PUXfacematch");

@ @<Assignments@>=
pux_face_match: match_ec_face(a);


@ The function |find_ec_num| lookup the |ectbl_eface_name| table
for the name |eface_name|. It returns the index to the name if the name
exits; otherwose, it returns the current value of |ectbl_ptr|.

@d ectbl_found(#)==((#)<ectbl_ptr)

@<Declare the function called |find_ec_num|@>=
function find_ec_num(eface_name:str_number):internal_ectbl_number;
  label done;
  var k:integer;@/
  begin
  k:=min_ectbl;
  while k < ectbl_ptr do
    begin
    if str_eq_str(eface_name,ectbl_eface_name[k]) then goto done;
    incr(k);
    end;
done: find_ec_num:=k;
  end;

@
@<Declare subprocedures for |prefixed_command|@>=
procedure make_cfont_id (f:internal_cfont_number; a:small_number);
var
@!i:0..23;
@!m:integer;
@!u: pointer;
@!t:str_number;
@!n:integer;
begin
buffer[buf_size+1]:='C';
buffer[buf_size+2]:='F';
buffer[buf_size+3]:='O';
buffer[buf_size+4]:='N';
buffer[buf_size+5]:='T';
m:=buf_size+6;
n:=f; i:=0;
repeat dig[i]:=n mod 10; n:=n div 10; incr(i);
until n=0;
while i>0 do {append design size}
  begin decr(i);
  buffer[m]:="0"+dig[i];
  incr(m);
  end;
no_new_control_sequence:=false;
u:=id_lookup(buf_size+1,m-buf_size-1);
no_new_control_sequence:=true;
t:=text(u);
define(u,set_cfont,f); eqtb[font_id_base+f]:=eqtb[u]; font_id_text(f):=t;
end;

@ @<Declare PUTeX subprocedures for |prefixed_command|@>=
function fetch_efont_face (@!efont_name:str_number):str_number;
var k:integer;@/
@!p:pool_pointer;
@!s: str_number;
begin
  p:=str_start[efont_name+1]-1; {last char position of efont\_name}@/
  while is_digit(str_pool[p]) do decr(p); {assumed that the TeX font name has letters}
  k:=str_start[efont_name];
  while k <= p do
    begin
    append_char(str_pool[k]);
    incr(k);
    end;
  s:=make_string;
  fetch_efont_face:=s;
end;


@
@<Declare subprocedures for |prefixed_command|@>=
@<Declare the function called |find_ec_num|@>@;
procedure match_ec_face(@!a:small_number);
label done1, done2, exit;
var k, f:integer;
@!eface_name, @!efname, @!efont_name, @!cface_id:str_number;
@!cfont_num:internal_cfont_number;
@!cface_num:internal_cface_number;
@!err:boolean;
  begin
  err:=false; f:=ectbl_ptr;@/
  eface_name:=scan_name;
  if cur_cmd=pux_set_cface then
    eface_name:=fetch_efont_face(font_name[cur_font]) {should be flushed later}
  else if eface_name=0 then begin
      print_err("Missing a TeX face name");
      error; goto exit;
      end;
  f:=find_ec_num(eface_name);
  if ectbl_found(f) then begin {it is already in the |ectbl_eface_name| table}
    flush_string;
    eface_name:=ectbl_eface_name[f]
    end;
  if cur_cmd=pux_set_cface then begin {the second form: match face of current efont}
      cface_num:=cur_chr;
      @<Define the |cur_cfont| according to |cur_font| and |cface_num|@>;
      end
  else
    @<Fetch a Chinese face id@>;
  @<Add this face matching@>;
exit: end;

@ @<Define the |cur_cfont| according to |cur_font| and |cface_num|@>=
cfont_num:=check_cfont(cface_num, font_size[cur_font]);
if undefined_cfont(cfont_num) then begin
  cfont_num:=make_cfont(cface_num,font_dsize[cur_font],font_size[cur_font]);
  make_cfont_id(cfont_num,a);
  end;
define(cur_cfont_loc,data,cfont_num)



@ @<Fetch a Chinese face id@>=
begin
@<Get the next non-blank non-call token@>;
if cur_cmd=pux_set_cface then
  cface_num:=cur_chr
else begin
  print_err("Missing a CJK font face identifier");
  err:=true; error;
  cface_num:=pux_default_cface;
  end
end

@ @<Add this face matching@>=
if f > max_ectbl then begin
  print_err("Font face matching table overflow");
  err:=true;
  error;
  end;
if not err then begin
  define(ectbl_cface_num_base+f,data,cface_num);
  if f = ectbl_ptr then begin {add this new eface name the the |eface_name table|}
    ectbl_eface_name[f]:=eface_name;
    incr(ectbl_ptr);
    end;
  end

@ @<Declare subprocedures for |prefixed_command|@>=
function lookup_cface (@!efont_name: str_number) : internal_cface_number;
var k:integer;@/
@!cface_num:internal_cface_number;@/
@!eface_name:str_number;@/
begin
  eface_name:=fetch_efont_face(efont_name);
  k:=find_ec_num(eface_name);
  flush_string;
  if ectbl_found(k) then
    cface_num:=ectbl_cface_num(k)
  else cface_num:=pux_default_cface;
  lookup_cface:=cface_num;
end;


@* \[60] Font matching.

@ @<Initialize table entries...@>=
equiv(font_matching_table_base):=null_cfont;
eq_type(font_matching_table_base):=data;
eq_level(font_matching_table_base):=level_one;
for k:=font_matching_table_base+1 to math_font_base-1 do
  eqtb[k]:=eqtb[font_matching_table_base];

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("PUXfontmatch",pux_font_match,0);


@ @<Assignments@>=
pux_font_match: match_ec_font(a);

@ @<Declare subprocedures for |prefixed_command|@>=
procedure match_ec_font(@!a:small_number);
label done;
var efont_num:internal_font_number;
@!cfont_num:internal_cfont_number;
@!cface_num:internal_cface_number;
begin
  @<Get the next non-blank non-call token@>;
  if cur_cmd = pux_set_cface then {the first form}
    begin
    efont_num:=cur_font;
    cface_num:=cur_chr;
    @<Define the |cur_cfont| according to |cur_font| and |cface_num|@>;
    goto done;
    end;
  if cur_cmd = set_font then {the second form}
    efont_num:=cur_chr
  else begin
    print_err("Missing Tex font identifier");
    help2("I was looking for a control sequence whose")@/
    ("current meaning has been defined by \font.");
    back_error; efont_num:=null_font;
    end;
  @<Get the next non-blank non-call token@>;
  if cur_cmd = set_cfont then cfont_num:=cur_chr
  else begin
    print_err("Missing CJK font identifier");
    help2("I was looking for a control sequence whose")@/
    ("current meaning has been defined by \cfont.");
    back_error; cfont_num:=null_cfont;
    end;
done:
  if efont_num<>null_font and cfont_num<>null_cfont then
    define(font_matching_table_base+efont_num-font_base,data,cfont_num);
end;

@ @<Other variables used by the procedure |prefixed_command|@>=
@!cface_num:internal_cface_number;
@!cfont_num:internal_cfont_number;

@ @<Set the matching CJK font@>=
cfont_num:=font_matching_table(cur_chr);
if cfont_num=null_cfont then begin {efont not mapped}
  if cur_cface=null_cface then
    cface_num:=lookup_cface(font_name[cur_chr])
  else cface_num:=cur_cface;
  @<Build a CJK font according to |cur_chr| and |cface_num| if it is not exist@>;
  end
else
  if cur_cface<>null_cface and cfont_face[cfont_num]<>cur_cface then begin
    cface_num:=cur_cface;
    @<Build a CJK font according to |cur_chr| and |cface_num| if it is not exist@>;
    end;
define(cur_cfont_loc,data,cfont_num)

@ @<Build a CJK font according to |cur_chr| and |cface_num| if it is not exist@>=
cfont_num:=check_cfont(cface_num, font_size[cur_chr]);
if undefined_cfont(cfont_num) then begin
  cfont_num:=make_cfont(cface_num,font_dsize[cur_chr],font_size[cur_chr]);
  make_cfont_id(cfont_num,a);
  end

@ @<Assignments@>=
set_cfont: define(cur_cfont_loc,data,cur_chr);

@ @<Other variables used by the procedure |prefixed_command|@>=
cface_id:str_number;

@ @<Assignments@>=
pux_set_cface: begin
  cface_num:=cur_chr;
  if cface_num <> cfont_face[cur_cfont] then begin
    @<Define the |cur_cfont| according to |cur_font| and |cface_num|@>;
    end;
  define(cur_cface_loc,data,cface_num);
  end;

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("puxgRotateCtext",puxg_assign_flag,int_base+puxg_rotate_ctext_code);
primitive("puxXspace",puxg_assign_int,int_base+pux_xspace_code);
primitive("puxCJKcharOther",puxg_assign_int,int_base+pux_wcharother_code);
primitive("puxCJKinput",puxg_assign_int,int_base+pux_CJKinput_code);
primitive("puxCharSet",puxg_assign_int,int_base+pux_charset_code);
primitive("puxgCfaceDepth",puxg_assign_int,int_base+puxg_cface_depth_code);


@ @<Cases of |print_cmd_chr|...@>=
puxg_assign_flag:
  if chr_code=puxg_rotate_ctext_code+int_base then
    print_esc("puxgRotateCtext");
puxg_assign_int:
  if chr_code=pux_xspace_code+int_base then
    print_esc("puxXspace")
  else if chr_code=pux_wcharother_code+int_base then
    print_esc("puxCJKcharOther")
  else if chr_code=pux_CJKinput_code+int_base then
    print_esc("puxCJKinput")
  else if chr_code=pux_charset_code+int_base then
    print_esc("puxCharSet")
  else if chr_code=puxg_cface_depth_code+int_base then
    print_esc("puxgCfaceDepth");

@ @<Assignments@>=
puxg_assign_flag: begin p:=cur_chr; scan_optional_equals; scan_int;
  if cur_val=0 and eqtb[p].int<>0 then begin
    print_err("Reset a PUTeX global parameter is not allowed here");
    help2("If a PUTeX global parameter was set to be a nonzero value,")@/
         ("it can't be reset to be zero again");
    error;
    end
  else begin
    if p=puxg_rotate_ctext_code+int_base then
      @<Handle the command |puxgRotateCtext|@>;
    word_define(p,cur_val);
    end;
  end;

@ @<Handle the command |puxgRotateCtext|@>=
if puxg_rotate_ctext=0 and cur_val<>0 then begin
  n:=cface_base;
  while n < cface_ptr do begin
    if cface_style[n] mod 2 = 1 then
      cface_style[n]:=cface_style[n]-rotated
    else
      cface_style[n]:=cface_style[n]+rotated;
    incr(n);
    end;
  end

@ @<Assignments@>=
puxg_assign_int: begin p:=cur_chr; q:=p-int_base;
  scan_optional_equals; scan_int;
  if cur_val < 0 then begin
    print_err("Negative "); print_param(p-int_base);
    print(" value ("); print_int(cur_val); print("), it remains unchanged");
    help1("This PUTeX parameter can't be negative.");
    error;
    end
  else if q=pux_charset_code and cur_val > 255 then begin
    print_err("Too large "); print_param(q);
    print(" value ("); print_int(cur_val); print("), it remains unchanged");
    help1("The value of document charset should be in the range 0..255.");
    error;
    end
  else begin
    case q of
    pux_xspace_code,pux_wcharother_code,pux_CJKinput_code,pux_charset_code:word_define(p, cur_val);
    puxg_cface_depth_code: if cur_val<>eqtb[p].int then
            @<Set PUTeX global parameter |puxgCfaceDepth|@>;
    othercases begin
      print_err("Unknow integer parameter!");
      error;
      end;
    endcases
    end;
  end;

@ @<Set PUTeX global parameter |puxgCfaceDepth|@>=
begin
  if cur_val>1000 then begin
    print_err("Improper `depth' value (");
    print_int(cur_val); print("). It is ignored");
    error;
    end
  else begin
    word_define(p,cur_val);
    cface_fw_default_depth:=convfix(puxg_cface_depth);
    n:=cface_base;
    while n<cface_ptr do begin
      cface_fw_depth[n]:=cface_fw_default_depth;
      incr(n);
      end;
    n:=cfont_base+1;
    while n<cfont_ptr do begin
      cfont_depth[n]:=fw_times_sd(cface_fw_depth[cfont_face[n]], cfont_size[n]);
      incr(n);
      end;
    end;
end

@
@d pux_set_cface_csp=0
@d pux_set_cface_cesp=1
@d pux_set_cface_depth=2
@<Put each of \TeX's primitives into the hash table@>=
primitive("PUXcfacecspace",pux_set_cface_attrib,pux_set_cface_csp);
primitive("PUXcfacecespace",pux_set_cface_attrib,pux_set_cface_cesp);
primitive("PUXcfacedepth",pux_set_cface_attrib,pux_set_cface_depth);

@ @<Cases of |print_cmd_chr|...@>=
pux_set_cface_attrib: begin
  case chr_code of
  pux_set_cface_csp:print_esc("PUXcfacecspace");
  pux_set_cface_cesp:print_esc("PUXcfacecespace");
  pux_set_cface_depth:print_esc("PUXcfacedepth");
  endcases;
  end;

@ @<Assignments@>=
pux_set_cface_attrib: begin p:=cur_chr;
  @<Get the next non-blank non-call token@>;
  if cur_cmd=pux_set_cface then
    cface_num:=cur_chr
  else begin
    cface_num:=null_cface;
    print_err("Missing a CJK font face identifier");
    error;
    end;
  scan_optional_equals;
  if p=pux_set_cface_csp or p=pux_set_cface_cesp then
    @<Scan spacing dimension of CJK font face@>
  else
    scan_int;

  if cface_num<>null_cface then begin
    if p=pux_set_cface_csp then
      @<Modify the cspace factor of the specified chinese face@>
    else if p=pux_set_cface_cesp then
      @<Modify the cespace factor of the specified chinese face@>
    else if p=pux_set_cface_depth then
      @<Modify the depth factor of the specified chinese face@>;
    end;
  end;

@ @<Other variables used by the procedure |prefixed_command|@>=
@!width_value:integer; {width of space}
@!stretch_value:integer; {stretch of space}
@!shrink_value:integer; {shrink of space}

@
@d puxg_set_cspace=0
@d puxg_set_cespace=1

@<Scan spacing dimension of CJK font face@>=
begin
  scan_optional_equals;
  scan_int;
  width_value:=cur_val;
  if scan_keyword("plus") then begin
    scan_int; stretch_value:=cur_val;
    end
  else {make stretch value compatible to \PUTeX 3}
    if width_value < 250 and p = puxg_set_cspace then stretch_value:=125
    else stretch_value:=width_value/2;

  if scan_keyword("minus") then begin
    scan_int; shrink_value:=cur_val;
    end
  else {make shrink value compatible to \PUTeX 3}
     if width_value > 0 then shrink_value:=width_value div 3
     else shrink_value:=-width_value div 3;
end

@ @<Modify the cspace factor of the specified chinese face@>=
begin
  if cface_csp_width[cface_num]<>width_value or 
     cface_csp_stretch[cface_num]<>stretch_value or
     cface_csp_shrink[cface_num]<>shrink_value then begin
    cface_csp_width[cface_num]:=width_value; 
    cface_csp_stretch[cface_num]:=stretch_value;
    cface_csp_shrink[cface_num]:=shrink_value;
    n:=cfont_base+1;
    while n<cfont_ptr do begin
      if cface_num = cfont_face[n] then set_cglue_spec(n);
      incr(n);
      end;
    end;
end

@ @<Modify the cespace factor of the specified chinese face@>=
begin
  if cface_cesp_width[cface_num]<>width_value or 
     cface_cesp_stretch[cface_num]<>stretch_value or
     cface_cesp_shrink[cface_num]<>shrink_value then begin
    cface_cesp_width[cface_num]:=width_value; 
    cface_cesp_stretch[cface_num]:=stretch_value;
    cface_cesp_shrink[cface_num]:=shrink_value;
    n:=cfont_base+1;
    while n<cfont_ptr do begin
      if cface_num=cfont_face[n] then set_ceglue_spec(n);
      incr(n);
      end;
    end;
end

@ @<Modify the depth factor of the specified chinese face@>=
begin
  cur_val:=convfix(cur_val);
  if cface_fw_depth[cface_num]<>cur_val then begin
    cface_fw_depth[cface_num]:=cur_val;
    n:=cfont_base+1;
    while n<cfont_ptr do begin
      if cface_num=cfont_face[n] then
        cfont_depth[n]:=fw_times_sd(cface_fw_depth[cface_num], cfont_size[n]);
      incr(n);
      end;
    end;
end

@
@d pux_set_cfont_csp=0
@d pux_set_cfont_cesp=1
@<Put each of \TeX's primitives into the hash table@>=
primitive("PUXcfontcspace",pux_set_cfont_attrib,pux_set_cfont_csp);
primitive("PUXcfontcespace",pux_set_cfont_attrib,pux_set_cfont_cesp);

@ @<Cases of |print_cmd_chr|...@>=
pux_set_cfont_attrib: begin
  case chr_code of
  pux_set_cfont_csp:print_esc("PUXcfontcspace");
  pux_set_cfont_cesp:print_esc("PUXcfontcespace");
  endcases;
  end;

@ @<Assignments@>=
pux_set_cfont_attrib: begin p:=cur_chr;
  @<Get the next non-blank non-call token@>;
  if cur_cmd = set_cfont then {the first form}
    begin
    cfont_num:=cur_chr;
    end
  else if cur_cmd = set_font and cur_chr=cur_font then
    cfont_num:=cur_cfont
  else begin
    print_err("Missing CJK font identifier");
    help2("I was looking for a control sequence whose")@/
    ("current meaning is a CJK font command.");
    back_error; cfont_num:=null_cfont;
    end;
  scan_optional_equals;
  case p of
  pux_set_cfont_csp: begin
    scan_glue(glue_val);
    width(cfont_glue_spec[cfont_num]):=width(cur_val);
    shrink(cfont_glue_spec[cfont_num]):=shrink(cur_val);
    stretch(cfont_glue_spec[cfont_num]):=stretch(cur_val);
    fast_delete_glue_ref(cur_val);
    end;
  pux_set_cfont_cesp: begin
    scan_glue(glue_val);
    width(cfont_ceglue_spec[cfont_num]):=width(cur_val);
    shrink(cfont_ceglue_spec[cfont_num]):=shrink(cur_val);
    stretch(cfont_ceglue_spec[cfont_num]):=stretch(cur_val);
    fast_delete_glue_ref(cur_val);
    end;
  endcases;
  end;

@ @<Global variables@>=
@!g_cspace_width:integer;
@!g_cspace_shrink:integer;
@!g_cspace_stretch:integer;
@!g_cespace_width:integer;
@!g_cespace_shrink:integer;
@!g_cespace_stretch:integer;

@
@d default_csp_width=50
@d default_cesp_width=150
@<Set init...@>=
g_cspace_width:=default_csp_width;
g_cspace_shrink:=g_cspace_width div 3;
g_cspace_stretch:=125;
g_cespace_width:=default_cesp_width;
g_cespace_shrink:=g_cespace_width div 3;
g_cespace_stretch:=g_cespace_width div 2;

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("puxgCspace",puxg_assign_space,puxg_set_cspace);
primitive("puxgCEspace",puxg_assign_space,puxg_set_cespace);


@ @<Cases of |print_cmd_chr|...@>=
puxg_assign_space: begin
  if chr_code = puxg_set_cspace then
    print_esc("puxgCspace")
  else if chr_code = puxg_set_cespace then
    print_esc("puxgCEspace");
  end;



@ @<Assignments@>=
puxg_assign_space:  begin p:=cur_chr;
  @<Scan spacing dimension of CJK font face@>;
  if p = puxg_set_cspace then begin
    g_cspace_width:=width_value;
    g_cspace_stretch:=stretch_value;
    g_cspace_shrink:=shrink_value;
    n:=cface_base;
    while n < cface_ptr do begin
       cface_csp_width[n]:=width_value;
       cface_csp_shrink[n]:=shrink_value;
       cface_csp_stretch[n]:=stretch_value;
      incr(n);
      end;
    n:=cfont_base+1;
    while n<cfont_ptr do begin
      set_cglue_spec(n);
      incr(n);
      end;

    end
  else if p = puxg_set_cespace then begin
    g_cespace_width:=width_value;
    g_cespace_stretch:=stretch_value;
    g_cespace_shrink:=shrink_value;
    end;
    n:=cface_base;
    while n < cface_ptr do begin
       cface_cesp_width[n]:=width_value;
       cface_cesp_shrink[n]:=shrink_value;
       cface_cesp_stretch[n]:=stretch_value;
      incr(n);
      end;
    n:=cfont_base+1;
    while n<cfont_ptr do begin
      set_ceglue_spec(n);
      incr(n);
      end;
  end;


@* \[61] Dump Font Info.

@<Other variables used by the procedure |prefixed_command|@>=
@!old_setting:0..max_selector; {holds |selector| setting}

@ @<Put each of \TeX's primitives into the hash table@>=
primitive("PUXdumpfontinfo",pux_dump_font_info,0);

@ @<Cases of |print_cmd_chr|...@>=
pux_dump_font_info: print_esc("PUXdumpfontinfo"); {TCW}

@ @<Assignments@>=
pux_dump_font_info: begin old_setting:=selector; selector:=log_only;@/
  @<Print TeX fonts@>;@/
  @<Print CJK font faces@>;@/
  @<Print CJK fonts@>;@/
  @<Print font faces matching table@>;@/
  selector:=old_setting;
  end;

@ @<Print TeX fonts@>=
print_ln; print("Tex fonts"); print_ln;
n:=0;
while n <= font_ptr do
  begin
  print_int(n); print(": "); print(font_name[n]);@/
  print(" dsize= "); print_scaled(font_dsize[n]); print("pt");@/
  print(" at "); print_scaled(font_size[n]); print("pt");@/
  print(" matched CJK font="); print_int(font_matching_table(n));
  print_ln; incr(n);
  end

@ @<Print CJK font faces@>=
print("Chinese faces"); print_ln;
n:=0;
while n < cface_ptr do
  begin
  print_int(n); print(": "); print("id="); print(cface[n]);@/
  print(" name="); print(cface_name[n]);@/
  print(" charset="); print_int(cface_charset[n]);@/
  print(" weight="); print_int(cface_weight[n]);@/
  print(" style="); print_int(cface_style[n]);@/
  print(" w="); print_fixword(cface_fw_width[n]);@/
  print(" h="); print_fixword(cface_fw_height[n]);@/
  print(" d="); print_fixword(cface_fw_depth[n]);@/
  print_ln; incr(n);
  end

@ @<Print CJK fonts@>=
print("CJK fonts"); print_ln;
n:=cfont_base;
while n < cfont_ptr do
  begin
  print_int(n); print(":face= ");
  print(cface[cfont_face[n]]);@/
  print(" dsize= "); print_scaled(cfont_dsize[n]); print("pt");@/
  print(" at "); print_scaled(cfont_size[n]); print("pt");@/
  print_ln; incr(n);
  end

@ @<Print font faces matching table@>=
print("English/CJK font faces matching table"); print_ln;
n:=min_ectbl;
while n < ectbl_ptr do
  begin
  print_int(n); print(": "); print("eface="); print(ectbl_eface_name[n]);@/
  print(" cface_id="); print(cface[ectbl_cface_num(n)]);@/
  print(" cface_num="); print_int(ectbl_cface_num(n));@/
  print_ln; incr(n);
  end

@ @<Global variables@>=
@!dvi_cf:internal_cfont_number;  {the current chinese font}

@ @<Output the CJK font definitions for all fonts that were used@>=
while cfont_ptr>cfont_base do
  begin if cfont_used[cfont_ptr] then dvi_cfont_def(cfont_ptr);
  decr(cfont_ptr);
  end


@ @<Change font |dvi_cf| to |f|@>=
begin if not cfont_used[f] then
  begin dvi_cfont_def(f); cfont_used[f]:=true;
  end;
  dvi_out(cfnt); dvi_out((f-cfont_base-1) div 256); dvi_out((f-cfont_base-1) mod 256);
  dvi_cf:=f;
end

@* \[62] Dump/undump \PUTeX\ internal information.

@ @<Dump the CJK font face information@>=
dump_int(cface_ptr);
dump_int(cface_fw_default_depth);
for k:=cface_base to cface_ptr-1 do begin
  dump_int(cface[k]);
  dump_int(cface_name[k]);
  dump_int(cface_charset[k]);
  dump_int(cface_weight[k]);
  dump_int(cface_style[k]);
  dump_int(cface_fw_width[k]);
  dump_int(cface_fw_height[k]);
  dump_int(cface_fw_depth[k]);
  dump_int(cface_csp_width[k]);
  dump_int(cface_csp_shrink[k]);
  dump_int(cface_csp_stretch[k]);
  dump_int(cface_cesp_width[k]);
  dump_int(cface_cesp_shrink[k]);
  dump_int(cface_cesp_stretch[k]);
  print_ln;
  print_int(k); print(": "); print("id="); print(cface[k]);@/
  print(" name="); print(cface_name[k]);@/
  print(" charset="); print_int(cface_charset[k]);@/
  print(" weight="); print_int(cface_weight[k]);@/
  print(" style="); print_int(cface_style[k]);@/
  print(" w="); print_fixword(cface_fw_width[k]);@/
  print(" h="); print_fixword(cface_fw_height[k]);@/
  print(" d="); print_fixword(cface_fw_depth[k]);@/
  end;
print_ln;
print_int(cface_ptr-cface_base); print(" preloaded CJK font face");
if cface_ptr<>cface_base+1 then print_char("s")

@ @<Undump the CJK font face information@>=
undump_size(cface_base)(max_cface)('cface max')(cface_ptr);
undump_int(cface_fw_default_depth);
for k:=cface_base to cface_ptr-1 do begin
  undump_size(0)(pool_size)('cface id')(cface[k]);
  undump_size(0)(pool_size)('cface name')(cface_name[k]);
  undump_size(0)(255)('charset size')(cface_charset[k]);
  undump_size(1)(1000)('cface weight')(cface_weight[k]);
  undump_size(0)(255)('cface style')(cface_style[k]);
  undump_int(cface_fw_width[k]);
  undump_int(cface_fw_height[k]);
  undump_int(cface_fw_depth[k]);
  undump_int(cface_csp_width[k]);
  undump_int(cface_csp_shrink[k]);
  undump_int(cface_csp_stretch[k]);
  undump_int(cface_cesp_width[k]);
  undump_int(cface_cesp_shrink[k]);
  undump_int(cface_cesp_stretch[k]);
  end

@ @<Dump the face matching table@>=
dump_int(ectbl_ptr);
for k:=min_ectbl to ectbl_ptr-1 do
  dump_int(ectbl_eface_name[k])

@ @<Unump the face matching table@>=
undump_size(min_ectbl)(max_ectbl)('ectbl_ptr')(ectbl_ptr);
for k:=min_ectbl to ectbl_ptr-1 do
  undump_size(0)(pool_size)('ectbl eface name')(ectbl_eface_name[k])

@ @<Dump the CJK font information@>=
begin
dump_int(cfont_ptr);
for k:=default_cfont to cfont_ptr-1 do begin
  dump_int(cfont_face[k]);
  dump_int(cfont_dsize[k]);
  dump_int(cfont_size[k]);
  dump_int(cfont_width[k]);
  dump_int(cfont_height[k]);
  dump_int(cfont_depth[k]);
  dump_int(cfont_glue_spec[k]);
  dump_int(cfont_ceglue_spec[k]);
  print_ln;
  print_int(k); print(":face= ");
  print(cface[cfont_face[k]]);@/
  print(" dsize= "); print_scaled(cfont_dsize[k]); print("pt");@/
  print(" at "); print_scaled(cfont_size[k]); print("pt");@/
  end;
end

@ @<Undump the CJK font information@>=
begin
undump_size(cfont_base)(cfont_max)('cfont max')(cfont_ptr);
for k:=default_cfont to cfont_ptr-1 do begin
  undump_size(cface_base)(max_cface)('cface max')(cfont_face[k]);
  undump_int(cfont_dsize[k]);
  undump_int(cfont_size[k]);
  undump_int(cfont_width[k]);
  undump_int(cfont_height[k]);
  undump_int(cfont_depth[k]);
  undump_int(cfont_glue_spec[k]);
  undump_int(cfont_ceglue_spec[k]);
  end;
end
  
@* \[63] Index.

@z

