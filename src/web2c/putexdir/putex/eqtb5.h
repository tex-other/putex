#define pretolerance_code 0 /* badness tolerance before hyphenation */
#define tolerance_code 1 /* badness tolerance after hyphenation */
#define line_penalty_code 2 /* added to the badness of every line */
#define hyphen_penalty_code 3 /* penalty for break after discretionary hyphen */
#define ex_hyphen_penalty_code 4 /* penalty for break after explicit hyphen */
#define club_penalty_code 5 /* penalty for creating a club line */
#define widow_penalty_code 6 /* penalty for creating a widow line */
#define display_widow_penalty_code 7 /* ditto, just before a display */
#define broken_penalty_code 8 /* penalty for breaking a page at a broken line */
#define bin_op_penalty_code 9 /* penalty for breaking after a binary operation */
#define rel_penalty_code 10 /* penalty for breaking after a relation */
#define pre_display_penalty_code 11 /* penalty for breaking just before a displayed formula */
#define post_display_penalty_code 12 /* penalty for breaking just after a displayed formula */
#define inter_line_penalty_code 13 /* additional penalty between lines */
#define double_hyphen_demerits_code 14 /* demerits for double hyphen break */
#define final_hyphen_demerits_code 15 /* demerits for final hyphen break */
#define adj_demerits_code 16 /* demerits for adjacent incompatible lines */
#define mag_code 17 /* magnification ratio */
#define delimiter_factor_code 18 /* ratio for variable-size delimiters */
#define looseness_code 19 /* change in number of lines for a paragraph */
#define time_code 20 /* current time of day */
#define day_code 21 /* current day of the month */
#define month_code 22 /* current month of the year */
#define year_code 23 /* current year of our Lord */
#define show_box_breadth_code 24 /* nodes per level in |show_box| */
#define show_box_depth_code 25 /* maximum level in |show_box| */
#define hbadness_code 26 /* hboxes exceeding this badness will be shown by |hpack| */
#define vbadness_code 27 /* vboxes exceeding this badness will be shown by |vpack| */
#define pausing_code 28 /* pause after each line is read from a file */
#define tracing_online_code 29 /* show diagnostic output on terminal */
#define tracing_macros_code 30 /* show macros as they are being expanded */
#define tracing_stats_code 31 /* show memory usage if \TeX\ knows it */
#define tracing_paragraphs_code 32 /* show line-break calculations */
#define tracing_pages_code 33 /* show page-break calculations */
#define tracing_output_code 34 /* show boxes when they are shipped out */
#define tracing_lost_chars_code 35 /* show characters that aren't in the font */
#define tracing_commands_code 36 /* show command codes at |big_switch| */
#define tracing_restores_code 37 /* show equivalents when they are restored */
#define uc_hyph_code 38 /* hyphenate words beginning with a capital letter */
#define output_penalty_code 39 /* penalty found at current page break */
#define max_dead_cycles_code 40 /* bound on consecutive dead cycles of output */
#define hang_after_code 41 /* hanging indentation changes after this many lines */
#define floating_penalty_code 42 /* penalty for insertions heldover after a split */
#define global_defs_code 43 /* override \.{\\global} specifications */
#define cur_fam_code 44 /* current family */
#define escape_char_code 45 /* escape character for token output */
#define default_hyphen_char_code 46 /* value of \.\{\hyphenchar} when a font is loaded */
#define default_skew_char_code 47 /* value of \.{\\skewchar} when a font is loaded */
#define end_line_char_code 48 /* character placed at the right end of the buffer */
#define new_line_char_code 49 /* character that prints as |print_ln| */
#define language_code 50 /* current hyphenation table */
#define left_hyphen_min_code 51 /* minimum left hyphenation fragment size */
#define right_hyphen_min_code 52 /* minimum right hyphenation fragment size */
#define holding_inserts_code 53 /* do not remove insertion nodes from \.{\\box255} */
#define error_context_lines_code 54 /* maximum intermediate line pairs shown */
#define int_pars 55 /* total number of integer parameters */

#define count_base int_base+int_pars /* 256 user \.{\\count} registers */
#define del_code_base count_base+256 /* 256 delimiter code mappings */
#define dimen_base del_code_base+256 /* beginning of region 6 */
