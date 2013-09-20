#define input_line_no_code glue_val+1 /* code for \.{\\inputlineno} */
#define badness_code glue_val+2 /* code for \.{\\badness} */

#define skipping 1 /* |scanner_status| when passing conditional text */
#define defining 2 /* |scanner_status| when reading a macro definition */
#define matching 3 /* |scanner_status| when reading macro arguments */
#define aligning 4 /* |scanner_status| when reading an alignment preamble */
#define absorbing 5 /* |scanner_status| when reading a balanced text */

#define vmode 1 /* vertical mode */
#define hmode vmode+max_command+1 /* horizontal mode */
#define mmode hmode+max_command+1 /* math mode */

#define level_zero min_quarterword /* level for undefined quantities */
#define level_one level_zero+1 /* outermost level for defined quantities */

#define token_list 0 /* |state| code when scanning a token list */
#define parameter 0 /* |token_type| code for parameter */
#define u_template 1 /* |token_type| code for \<u_j> template */
#define v_template 2 /* |token_type| code for \<v_j> template */
#define backed_up 3 /* |token_type| code for text to be reread */
#define inserted 4 /* |token_type| code for inserted texts */
#define macro 5 /* |token_type| code for defined control sequences */
#define output_text 6 /* |token_type| code for output routines */
#define every_par_text 7 /* |token_type| code for \.{\\everypar} */
#define every_math_text 8 /* |token_type| code for \.{\\everymath} */
#define every_display_text 9 /* |token_type| code for \.{\\everydisplay} */
#define every_hbox_text 10 /* |token_type| code for \.{\\everyhbox} */
#define every_vbox_text 11 /* |token_type| code for \.{\\everyvbox} */
#define every_job_text 12 /* |token_type| code for \.{\\everyjob} */
#define every_cr_text 13 /* |token_type| code for \.{\\everycr} */
#define mark_text 14 /* |token_type| code for \.{\\topmark}, etc. */
#define write_text 15 /* |token_type| code for \.{\\write} */

#define mid_line 1 /* |state| code when scanning a line of characters */
#define skip_blanks 2+max_char_code /* |state| code when ignoring blanks */
#define new_line 3+max_char_code+max_char_code /* |state| code at start of line */

#define number_code 0 /* command code for \.{\\number} */
#define roman_numeral_code 1 /* command code for \.{\\romannumeral} */
#define string_code 2 /* command code for \.{\\string} */
#define meaning_code 3 /* command code for \.{\\meaning} */
#define font_name_code 4 /* command code for \.{\\fontname} */
#define job_name_code 5 /* command code for \.{\\jobname} */

#define if_char_code 0 /*  `\.{\\if}'  */
#define if_cat_code 1 /*  `\.{\\ifcat}'  */
#define if_int_code 2 /*  `\.{\\ifnum}'  */
#define if_dim_code 3 /*  `\.{\\ifdim}'  */
#define if_odd_code 4 /*  `\.{\\ifodd}'  */
#define if_vmode_code 5 /*  `\.{\\ifvmode}'  */
#define if_hmode_code 6 /*  `\.{\\ifhmode}'  */
#define if_mmode_code 7 /*  `\.{\\ifmmode}'  */
#define if_inner_code 8 /*  `\.{\\ifinner}'  */
#define if_void_code 9 /*  `\.{\\ifvoid}'  */
#define if_hbox_code 10 /*  `\.{\\ifhbox}'  */
#define if_vbox_code 11 /*  `\.{\\ifvbox}'  */
#define ifx_code 12 /*  `\.{\\ifx}'  */
#define if_eof_code 13 /*  `\.{\\ifeof}'  */
#define if_true_code 14 /*  `\.{\\iftrue}'  */
#define if_false_code 15 /*  `\.{\\iffalse}'  */
#define if_case_code 16 /*  `\.{\\ifcase}'  */

#define restore_old_value 0 /* |save_type| when a value should be restored later */
#define restore_zero 1 /* |save_type| when an undefined entry should be restored */
#define insert_token 2 /* |save_type| when a token is being saved for later use */
#define level_boundary 3 /* |save_type| corresponding to beginning of group */

#define bottom_level 0 /* group code for the outside world */
#define simple_group 1 /* group code for local structure only */
#define hbox_group 2 /* code for `\.{\\hbox}\grp' */
#define adjusted_hbox_group 3 /* code for `\.{\\hbox}\grp' in vertical mode */
#define vbox_group 4 /* code for `\.{\\vbox}\grp' */
#define vtop_group 5 /* code for `\.{\\vtop}\grp' */
#define align_group 6 /* code for `\.{\\halign}\grp', `\.{\\valign}\grp' */
#define no_align_group 7 /* code for `\.{\\noalign}\grp' */
#define output_group 8 /* code for output routine */
#define math_group 9 /* code for, e.g., `\.{\char'136}\grp' */
#define disc_group 10 /* code for `\.{\\discretionary}\grp\grp\grp' */
#define insert_group 11 /* code for `\.{\\insert}\grp', `\.{\\vadjust}\grp' */
#define vcenter_group 12 /* code for `\.{\\vcenter}\grp' */
#define math_choice_group 13 /* code for `\.{\\mathchoice}\grp\grp\grp\grp' */
#define semi_simple_group 14 /* code for `\.{\\begingroup...\\endgroup}' */
#define math_shift_group 15 /* code for `\.{\$...\$}' */
#define math_left_group 16 /* code for `\.{\\left...\\right}' */
#define max_group_code 16

#define int_val 0 /* integer values */
#define dimen_val 1 /* dimension values */
#define glue_val 2 /* glue specifications */
#define mu_val 3 /* math glue specifications */
#define ident_val 4 /* font identifier */
#define tok_val 5 /* token lists */

#define top_mark_code 0 /* the mark in effect at the previous page break */
#define first_mark_code 1 /* the first mark between |top_mark| and |bot_mark| */
#define bot_mark_code 2 /* the mark in effect at the current page break */
#define split_first_mark_code 3 /* the first mark found by \.{\\vsplit} */
#define split_bot_mark_code 4 /* the last mark found by \.{\\vsplit} */

#define if_node_size 2 /* number of words in stack entry for conditionals */
#define if_code 1 /* code for \.{\\if...} being evaluated */
#define fi_code 2 /* code for \.{\\fi} */
#define else_code 3 /* code for \.{\\else} */
#define or_code 4 /* code for \.{\\or} */

#define no_tag 0 /* vanilla character */
#define lig_tag 1 /* character has a ligature/kerning program */
#define list_tag 2 /* character has a successor in a charlist */
#define ext_tag 3 /* character is extensible */

#define slant_code 1
#define space_code 2
#define space_stretch_code 3
#define space_shrink_code 4
#define x_height_code 5
#define quad_code 6
#define extra_space_code 7

#define exactly 0 /* a box dimension is pre-specified */
#define additional 1 /* a box dimension is increased from the natural one */

#define math_char 1 /* |math_type| when the attribute is simple */
#define sub_box 2 /* |math_type| when the attribute is a box */
#define sub_mlist 3 /* |math_type| when the attribute is a formula */
#define math_text_char 4 /* |math_type| when italic correction is dubious */

#define ord_noad unset_node+3 /* |type| of a noad classified Ord */
#define op_noad ord_noad+1 /* |type| of a noad classified Op */
#define bin_noad ord_noad+2 /* |type| of a noad classified Bin */
#define rel_noad ord_noad+3 /* |type| of a noad classified Rel */
#define open_noad ord_noad+4 /* |type| of a noad classified Ope */
#define close_noad ord_noad+5 /* |type| of a noad classified Clo */
#define punct_noad ord_noad+6 /* |type| of a noad classified Pun */
#define inner_noad ord_noad+7 /* |type| of a noad classified Inn */
#define limits 1 /* |subtype| of |op_noad| whose scripts are to be above, below */
#define no_limits 2 /* |subtype| of |op_noad| whose scripts are to be normal */

#define char_def_code 0 /* |shorthand_def| for \.{\\chardef} */
#define math_char_def_code 1 /* |shorthand_def| for \.{\\mathchardef} */
#define count_def_code 2 /* |shorthand_def| for \.{\\countdef} */
#define dimen_def_code 3 /* |shorthand_def| for \.{\\dimendef} */
#define skip_def_code 4 /* |shorthand_def| for \.{\\skipdef} */
#define mu_skip_def_code 5 /* |shorthand_def| for \.{\\muskipdef} */
#define toks_def_code 6 /* |shorthand_def| for \.{\\toksdef} */

#define above_code 0 /*  `\.{\\above}'  */
#define over_code 1 /*  `\.{\\over}'  */
#define atop_code 2 /*  `\.{\\atop}'  */
#define delimited_code 3 /*  `\.{\\abovewithdelims}', etc. */

#define box_code 0 /* |chr_code| for `\.{\\box}' */
#define copy_code 1 /* |chr_code| for `\.{\\copy}' */
#define last_box_code 2 /* |chr_code| for `\.{\\lastbox}' */
#define vsplit_code 3 /* |chr_code| for `\.{\\vsplit}' */
#define vtop_code 4 /* |chr_code| for `\.{\\vtop}' */

#define fil_code 0 /* identifies \.{\\hfil} and \.{\\vfil} */
#define fill_code 1 /* identifies \.{\\hfill} and \.{\\vfill} */
#define ss_code 2 /* identifies \.{\\hss} and \.{\\vss} */
#define fil_neg_code 3 /* identifies \.{\\hfilneg} and \.{\\vfilneg} */
#define skip_code 4 /* identifies \.{\\hskip} and \.{\\vskip} */
#define mskip_code 5 /* identifies \.{\\mskip} */

#define show_code 0 /*  \.{\\show}  */
#define show_box_code 1 /*  \.{\\showbox}  */
#define show_the_code 2 /*  \.{\\showthe}  */
#define show_lists 3 /*  \.{\\showlists}  */

#define open_node 0 /* |subtype| in whatsits that represent files to \.{\\openout} */
#define write_node 1 /* |subtype| in whatsits that represent things to \.{\\write} */
#define close_node 2 /* |subtype| in whatsits that represent streams to \.{\\closeout} */
#define special_node 3 /* |subtype| in whatsits that represent \.{\\special} things */
#define language_node 4 /* |subtype| in whatsits that change the current language */

#define tight_fit 3 /* fitness classification for lines shrinking 0.5 to 1.0 of their shrinkability */
#define loose_fit 1 /* fitness classification for lines stretching 0.5 to 1.0 of their stretchability */
#define very_loose_fit 0 /* fitness classification for lines stretching more than their stretchability */
#define decent_fit 2 /* fitness classification for all other lines */
