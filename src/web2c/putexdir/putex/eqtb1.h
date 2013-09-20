#define escape 0 /* escape delimiter (called \.\\ in /* \sl The \TeX book\/}) */
#define relax 0 /* do nothing ( \.{\\relax} ) */
#define left_brace 1 /* beginning of a group ( \.\{ ) */
#define right_brace 2 /* ending of a group ( \.\} ) */
#define math_shift 3 /* mathematics shift character ( \.\$ ) */
#define tab_mark 4 /* alignment delimiter ( \.\&, \.{\\span} ) */
#define car_ret 5 /* end of line ( |carriage_return|, \.{\\cr}, \.{\\crcr} ) */
#define out_param 5 /* output a macro parameter */
#define mac_param 6 /* macro parameter symbol ( \.\# ) */
#define sup_mark 7 /* superscript ( \.{\char'136} ) */
#define sub_mark 8 /* subscript ( \.{\char'137} ) */
#define ignore 9 /* characters to ignore ( \.{\^\^@@} ) */
#define endv 9 /* end of \<v_j> list in alignment template */
#define spacer 10 /* characters equivalent to blank space ( \.{\ } ) */
#define letter 11 /* characters regarded as letters ( \.{A..Z}, \.{a..z} ) */
#define other_char 12 /* none of the special character types */
#define active_char 13 /* characters that invoke macros ( \.{\char`\~} ) */
#define par_end 13 /* end of paragraph ( \.{\\par} ) */
#define match 13 /* match a macro parameter */
#define comment 14 /* characters that introduce comments ( \.\% ) */
#define end_match 14 /* end of parameters to macro */
#define stop 14 /* end of job ( \.{\\end}, \.{\\dump} ) */
#define invalid_char 15 /* characters that shouldn't appear ( \.{\^\^?} ) */
#define delim_num 15 /* specify delimiter numerically ( \.{\\delimiter} ) */
#define max_char_code 15 /* largest catcode for individual characters */
#define char_num 16 /* character specified numerically ( \.{\\char} ) */
#define math_char_num 17 /* explicit math code ( \.{\\mathchar} ) */
#define mark 18 /* mark definition ( \.{\\mark} ) */
#define xray 19 /* peek inside of \TeX\ ( \.{\\show}, \.{\\showbox}, etc.~) */
#define make_box 20 /* make a box ( \.{\\box}, \.{\\copy}, \.{\\hbox}, etc.~) */
#define hmove 21 /* horizontal motion ( \.{\\moveleft}, \.{\\moveright} ) */
#define vmove 22 /* vertical motion ( \.{\\raise}, \.{\\lower} ) */
#define un_hbox 23 /* unglue a box ( \.{\\unhbox}, \.{\\unhcopy} ) */
#define un_vbox 24 /* unglue a box ( \.{\\unvbox}, \.{\\unvcopy} ) */
#define remove_item 25 /* nullify last item ( \.{\\unpenalty}, \.{\\unkern}, \.{\\unskip} ) */
#define hskip 26 /* horizontal glue ( \.{\\hskip}, \.{\\hfil}, etc.~) */
#define vskip 27 /* vertical glue ( \.{\\vskip}, \.{\\vfil}, etc.~) */
#define mskip 28 /* math glue ( \.{\\mskip} ) */
#define kern 29 /* fixed space ( \.{\\kern}) */
#define mkern 30 /* math kern ( \.{\\mkern} ) */
#define leader_ship 31 /* use a box ( \.{\\shipout}, \.{\\leaders}, etc.~) */
#define halign 32 /* horizontal table alignment ( \.{\\halign} ) */
#define valign 33 /* vertical table alignment ( \.{\\valign} ) */
#define no_align 34 /* temporary escape from alignment ( \.{\\noalign} ) */
#define vrule 35 /* vertical rule ( \.{\\vrule} ) */
#define hrule 36 /* horizontal rule ( \.{\\hrule} ) */
#define insert 37 /* vlist inserted in box ( \.{\\insert} ) */
#define vadjust 38 /* vlist inserted in enclosing paragraph ( \.{\\vadjust} ) */
#define ignore_spaces 39 /* gobble |spacer| tokens ( \.{\\ignorespaces} ) */
#define after_assignment 40 /* save till assignment is done ( \.{\\afterassignment} ) */
#define after_group 41 /* save till group is done ( \.{\\aftergroup} ) */
#define break_penalty 42 /* additional badness ( \.{\\penalty} ) */
#define start_par 43 /* begin paragraph ( \.{\\indent}, \.{\\noindent} ) */
#define ital_corr 44 /* italic correction ( \.{\\/} ) */
#define accent 45 /* attach accent in text ( \.{\\accent} ) */
#define math_accent 46 /* attach accent in math ( \.{\\mathaccent} ) */
#define discretionary 47 /* discretionary texts ( \.{\\-}, \.{\\discretionary} ) */
#define eq_no 48 /* equation number ( \.{\\eqno}, \.{\\leqno} ) */
#define left_right 49 /* variable delimiter ( \.{\\left}, \.{\\right} ) */
#define math_comp 50 /* component of formula ( \.{\\mathbin}, etc.~) */
#define limit_switch 51 /* diddle limit conventions ( \.{\\displaylimits}, etc.~) */
#define above 52 /* generalized fraction ( \.{\\above}, \.{\\atop}, etc.~) */
#define math_style 53 /* style specification ( \.{\\displaystyle}, etc.~) */
#define math_choice 54 /* choice specification ( \.{\\mathchoice} ) */
#define non_script 55 /* conditional math glue ( \.{\\nonscript} ) */
#define vcenter 56 /* vertically center a vbox ( \.{\\vcenter} ) */
#define case_shift 57 /* force specific case ( \.{\\lowercase}, \.{\\uppercase}~) */
#define message 58 /* send to user ( \.{\\message}, \.{\\errmessage} ) */
#define extension 59 /* extensions to \TeX\ ( \.{\\write}, \.{\\special}, etc.~) */
#define in_stream 60 /* files for reading ( \.{\\openin}, \.{\\closein} ) */
#define begin_group 61 /* begin local grouping ( \.{\\begingroup} ) */
#define end_group 62 /* end local grouping ( \.{\\endgroup} ) */
#define omit 63 /* omit alignment template ( \.{\\omit} ) */
#define ex_space 64 /* explicit space ( \.{\\\ } ) */
#define no_boundary 65 /* suppress boundary ligatures ( \.{\\noboundary} ) */
#define radical 66 /* square root and similar signs ( \.{\\radical} ) */
#define end_cs_name 67 /* end control sequence ( \.{\\endcsname} ) */
#define min_internal 68 /* the smallest code that can follow \.{\\the} */
#define char_given 68 /* character code defined by \.{\\chardef} */
#define math_given 69 /* math code defined by \.{\\mathchardef} */
#define last_item 70 /* most recent item ( \.{\\lastpenalty}, \.{\\lastkern}, \.{\\lastskip} ) */
#define max_non_prefixed_command 70 /* largest command code that can't be \.{\\global} */
#define toks_register 71 /* token list register ( \.{\\toks} ) */
#define assign_toks 72 /* special token list ( \.{\\output}, \.{\\everypar}, etc.~) */
#define assign_int 73 /* user-defined integer ( \.{\\tolerance}, \.{\\day}, etc.~) */
#define assign_dimen 74 /* user-defined length ( \.{\\hsize}, etc.~) */
#define assign_glue 75 /* user-defined glue ( \.{\\baselineskip}, etc.~) */
#define assign_mu_glue 76 /* user-defined muglue ( \.{\\thinmuskip}, etc.~) */
#define assign_font_dimen 77 /* user-defined font dimension ( \.{\\fontdimen} ) */
#define assign_font_int 78 /* user-defined font integer ( \.{\\hyphenchar}, \.{\\skewchar} ) */
#define set_aux 79 /* specify state info ( \.{\\spacefactor}, \.{\\prevdepth} ) */
#define set_prev_graf 80 /* specify state info ( \.{\\prevgraf} ) */
#define set_page_dimen 81 /* specify state info ( \.{\\pagegoal}, etc.~) */
#define set_page_int 82 /* specify state info ( \.{\\deadcycles}, \.{\\insertpenalties} ) */
#define set_box_dimen 83 /* change dimension of box ( \.{\\wd}, \.{\\ht}, \.{\\dp} ) */
#define set_shape 84 /* specify fancy paragraph shape ( \.{\\parshape} ) */
#define def_code 85 /* define a character code ( \.{\\catcode}, etc.~) */
#define def_family 86 /* declare math fonts ( \.{\\textfont}, etc.~) */
#define set_font 87 /* set current font ( font identifiers ) */
#define def_font 88 /* define a font file ( \.{\\font} ) */
#define register 89 /* internal register ( \.{\\count}, \.{\\dimen}, etc.~) */
#define max_internal 89 /* the largest code that can follow \.{\\the} */
#define advance 90 /* advance a register or parameter ( \.{\\advance} ) */
#define multiply 91 /* multiply a register or parameter ( \.{\\multiply} ) */
#define divide 92 /* divide a register or parameter ( \.{\\divide} ) */
#define prefix 93 /* qualify a definition ( \.{\\global}, \.{\\long}, \.{\\outer} ) */
#define let 94 /* assign a command code ( \.{\\let}, \.{\\futurelet} ) */
#define shorthand_def 95 /* code definition ( \.{\\chardef}, \.{\\countdef}, etc.~) */
#define read_to_cs 96 /* read into a control sequence ( \.{\\read} ) */
#define def 97 /* macro definition ( \.{\\def}, \.{\\gdef}, \.{\\xdef}, \.{\\edef} ) */
#define set_box 98 /* set a box ( \.{\\setbox} ) */
#define hyph_data 99 /* hyphenation data ( \.{\\hyphenation}, \.{\\patterns} ) */
#define set_interaction 100 /* define level of interaction ( \.{\\batchmode}, etc.~) */
#define max_command 100 /* the largest command code seen at |big_switch| */
#define undefined_cs max_command+1 /*initial state of most |eq_type| fields */
#define expand_after max_command+2 /*special expansion ( \.{\\expandafter} ) */
#define no_expand max_command+3 /*special nonexpansion ( \.{\\noexpand} ) */
#define input max_command+4 /*input a source file ( \.{\\input}, \.{\\endinput} ) */
#define if_test max_command+5 /*conditional text ( \.{\\if}, \.{\\ifcase}, etc.~) */
#define fi_or_else max_command+6 /*delimiters for conditionals ( \.{\\else}, etc.~) */
#define cs_name max_command+7 /*make a control sequence from tokens ( \.{\\csname} ) */
#define convert max_command+8 /*convert to text ( \.{\\number}, \.{\\string}, etc.~) */
#define the max_command+9 /*expand an internal quantity ( \.{\\the} ) */
#define top_bot_mark max_command+10 /*inserted mark ( \.{\\topmark}, etc.~) */
#define call max_command+11 /*non-long, non-outer control sequence */
#define long_call max_command+12 /*long, non-outer control sequence */
#define outer_call max_command+13 /*non-long, outer control sequence */
#define long_outer_call max_command+14 /*long, outer control sequence */
#define end_template max_command+15 /*end of an alignment template */
#define dont_expand max_command+16 /*the following token was marked by \.{\\noexpand} */
#define glue_ref max_command+17 /*the equivalent points to a glue specification */
#define shape_ref max_command+18 /*the equivalent points to a parshape specification */
#define box_ref max_command+19 /*the equivalent points to a box node, or is |null| */
#define data max_command+20 /*the equivalent is simply a halfword number */
