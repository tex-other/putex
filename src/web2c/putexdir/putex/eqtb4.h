#define par_shape_loc local_base /* specifies paragraph shape */
#define output_routine_loc local_base+1 /* points to token list for \.{\\output} */
#define every_par_loc local_base+2 /* points to token list for \.{\\everypar} */
#define every_math_loc local_base+3 /* points to token list for \.{\\everymath} */
#define every_display_loc local_base+4 /* points to token list for \.{\\everydisplay} */
#define every_hbox_loc local_base+5 /* points to token list for \.{\\everyhbox} */
#define every_vbox_loc local_base+6 /* points to token list for \.{\\everyvbox} */
#define every_job_loc local_base+7 /* points to token list for \.{\\everyjob} */
#define every_cr_loc local_base+8 /* points to token list for \.{\\everycr} */
#define err_help_loc local_base+9 /* points to token list for \.{\\errhelp} */
#define toks_base local_base+10 /* table of 256 token list registers */
#define box_base toks_base+256 /* table of 256 box registers */
#define cur_font_loc box_base+256 /* internal font number outside math mode */
#define math_font_base cur_font_loc+1 /* table of 48 math font numbers */
#define cat_code_base math_font_base+48 /* table of 256 command codes (the ``catcodes'') */
#define lc_code_base cat_code_base+256 /* table of 256 lowercase mappings */
#define uc_code_base lc_code_base+256 /* table of 256 uppercase mappings */
#define sf_code_base uc_code_base+256 /* table of 256 spacefactor mappings */
#define math_code_base sf_code_base+256 /* table of 256 math mode mappings */
#define int_base math_code_base+256 /* beginning of region 5 */
