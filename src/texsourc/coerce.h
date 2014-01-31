/* Copyright 2007 TeX Users Group
   Copyright 2014 Clerk Ma

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA.  */

/* #define USEREGISTERS tells compiler use registers for mem and eqtb bkph */
/* this may make executable slightly smaller and slightly faster ... */
/* HOWEVER, won't work anymore for mem with dynamic memory allocation ... */
/* so can use registers now *only* for eqtb ... NOT for mem ... */
/* It is OK for eqtb, because, even though we may allocate eqtb, */
/* we won't ever reallocate it ... */

/* #define USEREGISTERS */
#undef USEREGISTERS 

/* WARNING: with dynamic allocation cannot use memory_word *mem=zmem */

#ifdef USEREGISTERS
void initialize(void);
#define initialize_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void print_ln(void);
void print_char_(ASCII_code);
#define print_char(s) print_char_((ASCII_code) (s))
#define printchar_regmem register memory_word *eqtb=zeqtb;
void print_(integer);
#define print(s) print_((integer) (s))
#define print_regmem register memory_word *eqtb=zeqtb;
void slow_print_(integer); 
#define slow_print(s) slow_print_((integer) (s))
#define slowprint_regmem
void print_nl_(str_number); 
#define print_nl(s) print_nl_((str_number) (s))
#define printnl_regmem
void print_esc_(str_number); 
#define print_esc(s) print_esc_((str_number) (s))
#define printesc_regmem register memory_word *eqtb=zeqtb;
void print_the_digs_(eight_bits); 
#define print_the_digs(k) print_the_digs_((eight_bits) (k))
#define printthedigs_regmem
void print_int_(integer); 
#define print_int(n) print_int_((integer) (n))
#define printint_regmem
void print_cs_(integer); 
#define print_cs(p) print_cs_((integer) (p))
#define printcs_regmem register memory_word *eqtb=zeqtb;
void sprint_cs_(halfword); 
#define sprint_cs(p) sprint_cs_((halfword) (p))
#define sprintcs_regmem
void print_file_name_(integer, integer, integer); 
#define print_file_name(n, a, e) print_file_name_((integer) (n), (integer) (a), (integer) (e))
#define printfilename_regmem
void print_size_(integer); 
#define print_size(s) print_size_((integer) (s))
#define printsize_regmem
void print_write_whatsit_(str_number, halfword); 
#define print_write_whatsit(s, p) print_write_whatsit_((str_number) (s), (halfword) (p))
#define printwritewhatsit_regmem register memory_word *mem=zmem;
// void jump_out(void);
int jump_out(void); 
#define jumpout_regmem
// void error(void);
int error(void); 
#define error_regmem
// void fatal_error_(str_number);
int fatal_error_(str_number); 
#define fatal_error(s) fatal_error_((str_number) (s))
#define fatalerror_regmem
// void overflow_(str_number, integer);
int overflow_(str_number, integer); 
#define overflow(s, n) overflow_((str_number) (s), (integer) (n))
#define overflow_regmem
// void confusion_(str_number);
int confusion_(str_number); 
#define confusion(s) confusion_((str_number) (s))
#define confusion_regmem
bool init_terminal(void); 
#define initterminal_regmem
str_number make_string(void); 
#define makestring_regmem
bool str_eq_buf_(str_number, integer); 
#define str_eq_buf(s, k) str_eq_buf_((str_number) (s), (integer) (k))
#define streqbuf_regmem
bool str_eq_str_(str_number, str_number); 
#define str_eq_str(s, t) str_eq_str_((str_number) (s), (str_number) (t))
#define streqstr_regmem
bool get_strings_started(void); 
#define getstringsstarted_regmem
void print_two_(integer); 
#define print_two(n) print_two_((integer) (n))
#define printtwo_regmem
void print_hex_(integer); 
#define print_hex(n) print_hex_((integer) (n))
#define printhex_regmem
void print_roman_int_(integer);
#define print_roman_int(n) print_roman_int_((integer) (n))
#define printromanint_regmem
void print_current_string(void); 
#define printcurrentstring_regmem
// void term_input(void); 
void term_input(int, int); 
#define terminput_regmem
void int_error_(integer); 
#define int_error(n) int_error_((integer) (n))

void normalize_selector(void); 

void pause_for_instructions(void); 

integer half_(integer); 
#define half(x) half_((integer) (x))

scaled round_decimals_(small_number); 
#define round_decimals(k) round_decimals_((small_number) (k))

void print_scaled_(scaled); 
#define print_scaled(s) print_scaled_((scaled) (s))

scaled mult_and_add_(integer, scaled, scaled, scaled); 
#define mult_and_add(n, x, y, maxanswer) mult_and_add_((integer) (n), (scaled) (x), (scaled) (y), (scaled) (maxanswer))

scaled x_over_n_(scaled, integer);  /* scaled x_over_n_(); */
#define x_over_n(x, n) x_over_n_((scaled) (x), (integer) (n))

scaled xn_over_d_(scaled, integer, integer); 
#define xn_over_d(x, n, d) xn_over_d_((scaled) (x), (integer) (n), (integer) (d))

halfword badness_(scaled, scaled); 
#define badness(t, s) badness_((scaled) (t), (scaled) (s))

void print_word_(memory_word); 
#define print_word(w) print_word_((memory_word) (w))

void show_token_list_(integer, integer, integer); 
#define show_token_list(p, q, l) show_token_list_((integer) (p), (integer) (q), (integer) (l))
#define showtokenlist_regmem register memory_word *mem=zmem;
void runaway(void); 
#define runaway_regmem register memory_word *mem=zmem;
halfword get_avail(void); 
#define getavail_regmem register memory_word *mem=zmem;
void flush_list_(halfword); 
#define flush_list(p) flush_list_((halfword) (p))
#define flushlist_regmem register memory_word *mem=zmem;
halfword get_node_(integer); 
#define get_node(s) get_node_((integer) (s))
#define getnode_regmem register memory_word *mem=zmem;
void free_node_(halfword, halfword); 
#define free_node(p, s) free_node_((halfword) (p), (halfword) (s))
#define freenode_regmem register memory_word *mem=zmem;
void sort_avail(void); 
#define sortavail_regmem register memory_word *mem=zmem;
halfword new_null_box(void); 
#define newnullbox_regmem register memory_word *mem=zmem;
halfword new_rule(void); 
#define newrule_regmem register memory_word *mem=zmem;
halfword new_ligature_(quarterword, quarterword, halfword); 
#define new_ligature(f, c, q) new_ligature_((quarterword) (f), (quarterword) (c), (halfword) (q))
#define newligature_regmem register memory_word *mem=zmem;
halfword new_lig_item_(quarterword); 
#define new_lig_item(c) new_lig_item_((quarterword) (c))
#define newligitem_regmem register memory_word *mem=zmem;
halfword new_disc(void); 
#define newdisc_regmem register memory_word *mem=zmem;
halfword new_math_(scaled, small_number); 
#define new_math(w, s) new_math_((scaled) (w), (small_number) (s))
#define newmath_regmem register memory_word *mem=zmem;
halfword znewspec(halfword); 
#define new_spec(p) znewspec((halfword) (p))
#define newspec_regmem register memory_word *mem=zmem;
halfword new_param_glue_(small_number); 
#define new_param_glue(n) new_param_glue_((small_number) (n))
#define newparamglue_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
halfword znewglue(halfword); 
#define new_glue(q) znewglue((halfword) (q))
#define newglue_regmem register memory_word *mem=zmem;
halfword znewskipparam(small_number); 
#define new_skip_param(n) znewskipparam((small_number) (n))
#define newskipparam_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
halfword new_kern_(scaled); 
#define new_kern(w) new_kern_((scaled) (w))
#define newkern_regmem register memory_word *mem=zmem;
halfword new_penalty_(integer); 
#define new_penalty(m) new_penalty_((integer) (m))
#define newpenalty_regmem register memory_word *mem=zmem;
void check_mem_(bool); 
#define check_mem(printlocs) check_mem_((bool) (printlocs))
#define checkmem_regmem register memory_word *mem=zmem;
void search_mem_(halfword); 
#define search_mem(p) search_mem_((halfword) (p))
#define searchmem_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void short_display_(integer); 
#define short_display(p) short_display_((integer) (p))
#define shortdisplay_regmem register memory_word *mem=zmem;
void print_font_and_char_(integer); 
#define print_font_and_char(p) print_font_and_char_((integer) (p))
#define printfontandchar_regmem register memory_word *mem=zmem;
void print_mark_(integer); 
#define print_mark(p) print_mark_((integer) (p))
#define printmark_regmem register memory_word *mem=zmem;
void print_rule_dimen_(scaled); 
#define print_rule_dimen(d) print_rule_dimen_((scaled) (d))
#define printruledimen_regmem
void print_glue_(scaled, integer, str_number); 
#define print_glue(d, order, s) print_glue_((scaled) (d), (integer) (order), (str_number) (s))
#define printglue_regmem
void print_spec_(integer, str_number); 
#define print_spec(p, s) print_spec_((integer) (p), (str_number) (s))
#define printspec_regmem register memory_word *mem=zmem;
void print_fam_and_char_(halfword); 
#define print_fam_and_char(p) print_fam_and_char_((halfword) (p))
#define printfamandchar_regmem register memory_word *mem=zmem;
void print_delimiter_(halfword); 
#define print_delimiter(p) print_delimiter_((halfword) (p))
#define printdelimiter_regmem register memory_word *mem=zmem;
void print_subsidiary_data_(halfword, ASCII_code); 
#define print_subsidiary_data(p, c) print_subsidiary_data_((halfword) (p), (ASCII_code) (c))
#define printsubsidiarydata_regmem register memory_word *mem=zmem;
void print_style_(integer); 
#define print_style(c) print_style_((integer) (c))
#define printstyle_regmem
void print_skip_param_(integer); /* void print_skip_param_(); */
#define print_skip_param(n) print_skip_param_((integer) (n))
#define printskipparam_regmem
void show_node_list_(integer); /* void show_node_list_(); */
#define show_node_list(p) show_node_list_((integer) (p))
#define shownodelist_regmem register memory_word *mem=zmem;
void show_box_(halfword); /* void show_box_(); */
#define show_box(p) show_box_((halfword) (p))
#define showbox_regmem register memory_word *eqtb=zeqtb;
void delete_token_ref_(halfword); /* void delete_token_ref_(); */
#define delete_token_ref(p) delete_token_ref_((halfword) (p))
#define deletetokenref_regmem register memory_word *mem=zmem;
void delete_glue_ref_(halfword); /* void delete_glue_ref_(); */
#define delete_glue_ref(p) delete_glue_ref_((halfword) (p))
#define deleteglueref_regmem register memory_word *mem=zmem;
void flush_node_list_(halfword); /* void flush_node_list_(); */
#define flush_node_list(p) flush_node_list_((halfword) (p))
#define flushnodelist_regmem register memory_word *mem=zmem;
halfword copy_node_list_(halfword); /* halfword copy_node_list_(); */
#define copy_node_list(p) copy_node_list_((halfword) (p))
#define copynodelist_regmem register memory_word *mem=zmem;
void print_mode_(integer); /* void print_mode_(); */
#define print_mode(m) print_mode_((integer) (m))
#define printmode_regmem
void push_nest(void); /* void push_nest(); */
#define pushnest_regmem
void pop_nest(void); /* void pop_nest(); */
#define popnest_regmem register memory_word *mem=zmem;
void show_activities(void); /* void show_activities(); */
#define showactivities_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void print_param_(integer); /* void print_param_(); */
#define print_param(n) print_param_((integer) (n))
#define printparam_regmem
void begin_diagnostic(void); /* void begin_diagnostic(); */
#define begindiagnostic_regmem register memory_word *eqtb=zeqtb;
void end_diagnostic_(bool); /* void end_diagnostic_(); */
#define end_diagnostic(blankline) end_diagnostic_((bool) (blankline))
#define enddiagnostic_regmem
void print_length_param_(integer); /* void print_length_param_(); */
#define print_length_param(n) print_length_param_((integer) (n))
#define printlengthparam_regmem
void print_cmd_chr_(quarterword, halfword); /* void print_cmd_chr_(); */
#define print_cmd_chr(cmd, chrcode) print_cmd_chr_((quarterword) (cmd), (halfword) (chrcode))
#define printcmdchr_regmem
void show_eqtb_(halfword); /* void show_eqtb_(); */
#define show_eqtb(n) show_eqtb_((halfword) (n))
#define showeqtb_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
halfword id_lookup_(integer, integer); /* halfword id_lookup_(); */
#define id_lookup(j, l) id_lookup_((integer) (j), (integer) (l))
#define idlookup_regmem
void primitive_(str_number, quarterword, halfword); /* void primitive_(); */
#define primitive(s, c, o) primitive_((str_number) (s), (quarterword) (c), (halfword) (o))
#define primitive_regmem register memory_word *eqtb=zeqtb;
void new_save_level_(group_code); /* void new_save_level_(); */
#define new_save_level(c) new_save_level_((group_code) (c))
#define newsavelevel_regmem
void eq_destroy_(memory_word); /* void eq_destroy_(); */
#define eq_destroy(w) eq_destroy_((memory_word) (w))
#define eqdestroy_regmem register memory_word *mem=zmem;
void eq_save_(halfword, quarterword); /* void eq_save_(); */
#define eq_save(p, l) eq_save_((halfword) (p), (quarterword) (l))
#define eqsave_regmem register memory_word *eqtb=zeqtb;
void eq_define_(halfword, quarterword, halfword); /* void eq_define_(); */
#define eq_define(p, t, e) eq_define_((halfword) (p), (quarterword) (t), (halfword) (e))
#define eqdefine_regmem register memory_word *eqtb=zeqtb;
void eq_word_define_(halfword, integer); /* void eq_word_define_(); */
#define eq_word_define(p, w) eq_word_define_((halfword) (p), (integer) (w))
#define eqworddefine_regmem register memory_word *eqtb=zeqtb;
void geq_define_(halfword, quarterword, halfword); /* void geq_define_(); */
#define geq_define(p, t, e) geq_define_((halfword) (p), (quarterword) (t), (halfword) (e))
#define geqdefine_regmem register memory_word *eqtb=zeqtb;
void geq_word_define_(halfword, integer); /* void geq_word_define_(); */
#define geq_word_define(p, w) geq_word_define_((halfword) (p), (integer) (w))
#define geqworddefine_regmem register memory_word *eqtb=zeqtb;
void save_for_after_(halfword); /* void save_for_after_(); */
#define save_for_after(t) save_for_after_((halfword) (t))
#define saveforafter_regmem
void restore_trace_(halfword, str_number); /* void restore_trace_(); */
#define restore_trace(p, s) restore_trace_((halfword) (p), (str_number) (s))
#define restoretrace_regmem
void unsave(void); /* void unsave(); */
#define unsave_regmem register memory_word *eqtb=zeqtb;
void prepare_mag(void); /* void prepare_mag(); */
#define preparemag_regmem register memory_word *eqtb=zeqtb;
void token_show_(halfword); /* void token_show_(); */
#define token_show(p) token_show_((halfword) (p))
#define tokenshow_regmem register memory_word *mem=zmem;
void print_meaning(void); /* void print_meaning(); */
#define printmeaning_regmem
void show_cur_cmd_chr(void); /* void show_cur_cmd_chr(); */
#define showcurcmdchr_regmem
void show_context(void); /* void show_context(); */
#define showcontext_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void begin_token_list_(halfword, quarterword); /* void begin_token_list_(); */
#define begin_token_list(p, t) begin_token_list_((halfword) (p), (quarterword) (t))
#define begintokenlist_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void end_token_list(void); /* void end_token_list(); */
#define endtokenlist_regmem
void back_input(void); /* void back_input(); */
#define backinput_regmem register memory_word *mem=zmem;
void back_error(void); /* void back_error(); */
#define backerror_regmem
void ins_error(void); /* void ins_error(); */
#define inserror_regmem
void begin_file_reading(void); /* void begin_file_reading(); */
#define beginfilereading_regmem
void end_file_reading(void); /* void end_file_reading(); */
#define endfilereading_regmem
void clear_for_error_prompt(void); /* void clear_for_error_prompt(); */
#define clearforerrorprompt_regmem
void check_outer_validity(void); /* void check_outer_validity(); */
#define checkoutervalidity_regmem register memory_word *mem=zmem;
void get_next(void); /* void get_next(); */
#define getnext_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void firm_up_the_line(void); /* void firm_up_the_line(); */
#define firmuptheline_regmem register memory_word *eqtb=zeqtb;
void get_token(void); /* void get_token(); */
#define gettoken_regmem
void macro_call(void); /* void macro_call(); */
#define macrocall_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void insert_relax(void); /* void insert_relax(); */
#define insertrelax_regmem
void expand(void); /* void expand(); */
#define expand_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void get_x_token(void); /* void get_x_token(); */
#define getxtoken_regmem
void x_token(void); /* void x_token(); */
#define xtoken_regmem
void scan_left_brace(void); /* void scan_left_brace(); */
#define scanleftbrace_regmem
void scan_optional_equals(void); /* void scan_optional_equals(); */
#define scanoptionalequals_regmem
bool scan_keyword_(str_number); /* bool scan_keyword_(); */
#define scan_keyword(s) scan_keyword_((str_number) (s))
#define scankeyword_regmem register memory_word *mem=zmem;
void mu_error(void); /* void mu_error(); */
#define muerror_regmem
void scan_eight_bit_int(void); /* void scan_eight_bit_int(); */
#define scaneightbitint_regmem
void scan_char_num(void); /* void scan_char_num(); */
#define scancharnum_regmem
void scan_four_bit_int(void); /* void scan_four_bit_int(); */
#define scanfourbitint_regmem
void scan_fifteen_bit_int(void); /* void scan_fifteen_bit_int(); */
#define scanfifteenbitint_regmem
void scan_twenty_seven_bit_int(void); /* void scan_twenty_seven_bit_int(); */
#define scantwentysevenbitint_regmem
void scan_font_ident(void); /* void scan_font_ident(); */
#define scanfontident_regmem register memory_word *eqtb=zeqtb;
void find_font_dimen_(bool); /* void find_font_dimen_(); */
#define find_font_dimen(writing) find_font_dimen_((bool) (writing))
#define findfontdimen_regmem
void scan_something_internal_(small_number, bool);
/* void scan_something_internal_(); */
#define scan_something_internal(level, negative) scan_something_internal_((small_number) (level), (bool) (negative))
#define scansomethinginternal_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void scan_int(void); /* void scan_int(); */
#define scanint_regmem
void scan_dimen_(bool, bool, bool); /* void scan_dimen_(); */
#define scan_dimen(mu, inf, shortcut) scan_dimen_((bool) (mu), (bool) (inf), (bool) (shortcut))
#define scandimen_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void scan_glue_(small_number); /* void scan_glue_(); */
#define scan_glue(level) scan_glue_((small_number) (level))
#define scanglue_regmem register memory_word *mem=zmem;
halfword scan_rule_spec(void); /* halfword scan_rule_spec(); */
#define scanrulespec_regmem register memory_word *mem=zmem;
halfword str_toks_(pool_pointer); /* halfword str_toks_(); */
#define str_toks(b) str_toks_((pool_pointer) (b))
#define strtoks_regmem register memory_word *mem=zmem;
halfword the_toks(void); /* halfword the_toks(); */
#define thetoks_regmem register memory_word *mem=zmem;
void ins_the_toks(void); /* void ins_the_toks(); */
#define insthetoks_regmem register memory_word *mem=zmem;
void conv_toks(void); /* void conv_toks(); */
#define convtoks_regmem register memory_word *mem=zmem;
halfword scan_toks_(bool, bool); /* halfword scan_toks_(); */
#define scan_toks(macrodef, xpand) scan_toks_((bool) (macrodef), (bool) (xpand))
#define scantoks_regmem register memory_word *mem=zmem;
void read_toks_(integer, halfword); /* void read_toks_(); */
#define read_toks(n, r) read_toks_((integer) (n), (halfword) (r))
#define readtoks_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void pass_text(void); /* void pass_text(); */
#define passtext_regmem
void change_if_limit_(small_number, halfword); /* void change_if_limit_(); */
#define change_if_limit(l, p) change_if_limit_((small_number) (l), (halfword) (p))
#define changeiflimit_regmem register memory_word *mem=zmem;
void conditional(void); /* void conditional(); */
#define conditional_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void begin_name(void); /* void begin_name(); */
#define beginname_regmem
bool more_name_(ASCII_code); /* bool more_name_(); */
#define more_name(c) more_name_((ASCII_code) (c))
#define morename_regmem
void end_name(void); /* void end_name(); */
#define endname_regmem
void pack_file_name_(str_number, str_number, str_number); /* void pack_file_name_(); */
#define pack_file_name(n, a, e) pack_file_name_((str_number) (n), (str_number) (a), (str_number) (e))
#define packfilename_regmem
void pack_buffered_name_(small_number, integer, integer); /* void pack_buffered_name_(); */
#define pack_buffered_name(n, a, b) pack_buffered_name_((small_number) (n), (integer) (a), (integer) (b))
#define packbufferedname_regmem
str_number make_name_string(void); /* str_number make_name_string(); */
#define makenamestring_regmem
str_number a_make_name_string_(alpha_file *); /* str_number a_make_name_string_(); */
#define a_make_name_string(f) a_make_name_string_((alpha_file *) &(f))
#define amakenamestring_regmem
str_number b_make_name_string_(byte_file *); /* str_number b_make_name_string_(); */
#define b_make_name_string(f) b_make_name_string_((byte_file *) &(f))
#define bmakenamestring_regmem
str_number w_make_name_string_(word_file *); /* str_number w_make_name_string_(); */
#define w_make_name_string(f) w_make_name_string_((word_file *) &(f))
#define wmakenamestring_regmem
void scan_file_name(void); /* void scan_file_name(); */
#define scanfilename_regmem
void pack_job_name_(str_number); /* void pack_job_name_(); */
#define pack_job_name(s) pack_job_name_((str_number) (s))
#define packjobname_regmem
void prompt_file_name_(str_number, str_number); /* void prompt_file_name_(); */
#define prompt_file_name(s, e) prompt_file_name_((str_number) (s), (str_number) (e))
#define promptfilename_regmem
void open_log_file(void); /* void open_log_file(); */
#define openlogfile_regmem register memory_word *eqtb=zeqtb;
void start_input(void); /* void start_input(); */
#define startinput_regmem register memory_word *eqtb=zeqtb;
internal_font_number read_font_info_(halfword, str_number, str_number, scaled);
/* internal_font_number read_font_info_(); */
#define read_font_info(u, nom, aire, s) read_font_info_((halfword) (u), (str_number) (nom), (str_number) (aire), (scaled) (s))
#define readfontinfo_regmem register memory_word *eqtb=zeqtb;
void char_warning_(internal_font_number, eight_bits); /* void char_warning_(); */
#define char_warning(f, c) char_warning_((internal_font_number) (f), (eight_bits) (c))
#define charwarning_regmem register memory_word *eqtb=zeqtb;
halfword new_character_(internal_font_number, eight_bits); /* halfword new_character_(); */
#define new_character(f, c) new_character_((internal_font_number) (f), (eight_bits) (c))
#define newcharacter_regmem register memory_word *mem=zmem;
void dvi_swap(void); /* void dvi_swap(); */
#define dviswap_regmem
void zdvifour(integer); /* void zdvifour(); */
#define dvi_four(x) zdvifour((integer) (x))
#define dvifour_regmem
void zdvipop(integer); /* void zdvipop(); */
#define dvi_pop(l) zdvipop((integer) (l))
#define dvipop_regmem
void zdvifontdef(internal_font_number); /* void zdvifontdef(); */
#define dvi_font_def(f) zdvifontdef((internal_font_number) (f))
#define dvifontdef_regmem
void zmovement(scaled, eight_bits); /* void zmovement(); */
#define movement(w, o) zmovement((scaled) (w), (eight_bits) (o))
#define movement_regmem register memory_word *mem=zmem; 
void prune_movements_(integer); /* void prune_movements_(); */
#define prune_movements(l) prune_movements_((integer) (l))
#define prunemovements_regmem register memory_word *mem=zmem;
void zspecialout(halfword); /* void zspecialout(); */
#define special_out(p) zspecialout((halfword) (p))
#define specialout_regmem register memory_word *mem=zmem;
void write_out_(halfword); /* void write_out_(); */
#define write_out(p) write_out_((halfword) (p))
#define writeout_regmem register memory_word *mem=zmem;
void out_what_(halfword); /* void out_what_(); */
#define out_what(p) out_what_((halfword) (p))
#define outwhat_regmem register memory_word *mem=zmem;
void hlist_out(void); /* void hlist_out(); */
#define hlistout_regmem register memory_word *mem=zmem;
void vlist_out(void); /* void vlist_out(); */
#define vlistout_regmem register memory_word *mem=zmem;
void zshipout(halfword); /* void zshipout(); */
#define ship_out(p) zshipout((halfword) (p))
#define shipout_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void scan_spec_(group_code, bool); /* void scan_spec_(); */
#define scan_spec(c, threecodes) scan_spec_((group_code) (c), (bool) (threecodes))
#define scanspec_regmem
halfword hpack_(halfword, scaled, small_number); /* halfword hpack_(); */
#define hpack(p, w, m) hpack_((halfword) (p), (scaled) (w), (small_number) (m))
#define hpack_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
halfword vpackage_(halfword, scaled, small_number, scaled); /* halfword vpackage_(); */
#define vpackage(p, h, m, l) vpackage_((halfword) (p), (scaled) (h), (small_number) (m), (scaled) (l))
#define vpackage_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void append_to_vlist_(halfword); /* void append_to_vlist_(); */
#define append_to_vlist(b) append_to_vlist_((halfword) (b))
#define appendtovlist_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
halfword new_noad(void); /* halfword new_noad(); */
#define newnoad_regmem register memory_word *mem=zmem;
halfword new_style_(small_number); /* halfword new_style_(); */
#define new_style(s) new_style_((small_number) (s))
#define newstyle_regmem register memory_word *mem=zmem;
halfword new_choice(void); /* halfword new_choice(); */
#define newchoice_regmem register memory_word *mem=zmem;
void show_info(void); /* void show_info(); */
#define showinfo_regmem register memory_word *mem=zmem;
halfword fraction_rule_(scaled); /* halfword fraction_rule_(); */
#define fraction_rule(t) fraction_rule_((scaled) (t))
#define fractionrule_regmem register memory_word *mem=zmem;
halfword overbar_(halfword, scaled, scaled); /* halfword overbar_(); */
#define overbar(b, k, t) overbar_((halfword) (b), (scaled) (k), (scaled) (t))
#define overbar_regmem register memory_word *mem=zmem;
halfword char_box_(internal_font_number, quarterword); /* halfword char_box_(); */
#define char_box(f, c) char_box_((internal_font_number) (f), (quarterword) (c))
#define charbox_regmem register memory_word *mem=zmem;
void stack_into_box_(halfword, internal_font_number, quarterword);
/* void stack_into_box_(); */
#define stack_into_box(b, f, c) stack_into_box_((halfword) (b), (internal_font_number) (f), (quarterword) (c))
#define stackintobox_regmem register memory_word *mem=zmem;

scaled height_plus_depth_(internal_font_number, fquarterword);
#define height_plus_depth(f, c) height_plus_depth_((internal_font_number) (f), (fquarterword) (c))

#define heightplusdepth_regmem
halfword var_delimiter_(halfword, small_number, scaled); /* halfword var_delimiter_(); */
#define var_delimiter(d, s, v) var_delimiter_((halfword) (d), (small_number) (s), (scaled) (v))
#define vardelimiter_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
halfword rebox_(halfword, scaled); /* halfword rebox_(); */
#define rebox(b, w) rebox_((halfword) (b), (scaled) (w))
#define rebox_regmem register memory_word *mem=zmem;
halfword math_glue_(halfword, scaled); /* halfword math_glue_(); */
#define math_glue(g, m) math_glue_((halfword) (g), (scaled) (m))
#define mathglue_regmem register memory_word *mem=zmem;
void math_kern_(halfword, scaled); /* void math_kern_(); */
#define math_kern(p, m) math_kern_((halfword) (p), (scaled) (m))
#define mathkern_regmem register memory_word *mem=zmem;
void flush_math(void); /* void flush_math(); */
#define flushmath_regmem register memory_word *mem=zmem;
halfword clean_box_(halfword, small_number); /* halfword clean_box_(); */
#define clean_box(p, s) clean_box_((halfword) (p), (small_number) (s))
#define cleanbox_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void fetch_(halfword); /* void fetch_(); */
#define fetch(a) fetch_((halfword) (a))
#define fetch_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void make_over_(halfword); /* void make_over_(); */
#define make_over(q) make_over_((halfword) (q))
#define makeover_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void make_under_(halfword); /* void make_under_(); */
#define make_under(q) make_under_((halfword) (q))
#define makeunder_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void make_vcenter_(halfword); /* void make_vcenter_(); */
#define make_vcenter(q) make_vcenter_((halfword) (q))
#define makevcenter_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void make_radical_(halfword); /* void make_radical_(); */
#define make_radical(q) make_radical_((halfword) (q))
#define makeradical_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void make_math_accent_(halfword); /* void make_math_accent_(); */
#define make_math_accent(q) make_math_accent_((halfword) (q))
#define makemathaccent_regmem register memory_word *mem=zmem;
void make_fraction_(halfword); /* void make_fraction_(); */
#define make_fraction(q) make_fraction_((halfword) (q))
#define makefraction_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
scaled make_op_(halfword); /* scaled make_op_(); */
#define make_op(q) make_op_((halfword) (q))
#define makeop_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void make_ord_(halfword); /* void make_ord_(); */
#define make_ord(q) make_ord_((halfword) (q))
#define makeord_regmem register memory_word *mem=zmem;
void make_scripts_(halfword, scaled); /* void make_scripts_(); */
#define make_scripts(q, delta) make_scripts_((halfword) (q), (scaled) (delta))
#define makescripts_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
small_number make_left_right_(halfword, small_number, scaled, scaled);
/* small_number make_left_right_(); */
#define make_left_right(q, style, maxd, max_h) make_left_right_((halfword) (q), (small_number) (style), (scaled) (maxd), (scaled) (max_h))
#define makeleftright_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void mlist_to_hlist(void); /* void mlist_to_hlist(); */
#define mlisttohlist_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void push_alignment(void); /* void push_alignment(); */
#define pushalignment_regmem register memory_word *mem=zmem;
void pop_alignment(void); /* void pop_alignment(); */
#define popalignment_regmem register memory_word *mem=zmem;
void get_preamble_token(void); /* void get_preamble_token(); */
#define getpreambletoken_regmem register memory_word *eqtb=zeqtb;
void init_align(void); /* void init_align(); */
#define initalign_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void init_span_(halfword); /* void init_span_(); */
#define init_span(p) init_span_((halfword) (p))
#define initspan_regmem
void init_row(void); /* void init_row(); */
#define initrow_regmem register memory_word *mem=zmem;
void init_col(void); /* void init_col(); */
#define initcol_regmem register memory_word *mem=zmem;
bool fin_col(void); /* bool fin_col(); */
#define fincol_regmem register memory_word *mem=zmem;
void fin_row(void); /* void fin_row(); */
#define finrow_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void fin_align(void); /* void fin_align(); */
#define finalign_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void align_peek(void); /* void align_peek(); */
#define alignpeek_regmem
halfword finite_shrink_(halfword); /* halfword finite_shrink_(); */
#define finite_shrink(p) finite_shrink_((halfword) (p))
#define finiteshrink_regmem register memory_word *mem=zmem;
void try_break_(integer, small_number); /* void try_break_(); */
#define try_break(pi, breaktype) try_break_((integer) (pi), (small_number) (breaktype))
#define trybreak_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void post_line_break_(integer); /* void post_line_break_(); */
#define post_line_break(finalwidowpenalty) post_line_break_((integer) (finalwidowpenalty))
#define postlinebreak_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
small_number reconstitute_(small_number, small_number, halfword, halfword); 
/* small_number reconstitute_(); */
#define reconstitute(j, n, bchar, hchar) reconstitute_((small_number) (j), (small_number) (n), (halfword) (bchar), (halfword) (hchar))
#define reconstitute_regmem register memory_word *mem=zmem;
void hyphenate(void); /* void hyphenate(); */
#define hyphenate_regmem register memory_word *mem=zmem;
trie_op_code new_trie_op_(small_number, small_number, trie_op_code);
/* trie_op_code new_trie_op_(); */
#define new_trie_op(d, n, v) new_trie_op_((small_number) (d), (small_number) (n), (trie_op_code) (v))
#define newtrieop_regmem
trie_pointer trie_node_(trie_pointer); /* trie_pointer trie_node_(); */
#define trie_node(p) trie_node_((trie_pointer) (p))
#define trienode_regmem
trie_pointer compress_trie_(trie_pointer); /* trie_pointer compress_trie_(); */
#define compress_trie(p) compress_trie_((trie_pointer) (p))
#define compresstrie_regmem
void first_fit_(trie_pointer); /* void first_fit_(); */
#define first_fit(p) first_fit_((trie_pointer) (p))
#define firstfit_regmem
void trie_pack_(trie_pointer); /* void trie_pack_(); */
#define trie_pack(p) trie_pack_((trie_pointer) (p))
#define triepack_regmem
void trie_fix_(trie_pointer); /* void trie_fix_(); */
#define trie_fix(p) trie_fix_((trie_pointer) (p))
#define triefix_regmem
void new_patterns(void); /* void new_patterns(); */
#define newpatterns_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void init_trie(void); /* void init_trie(); */
#define inittrie_regmem
void line_break_(integer); /* void line_break_(); */
#define line_break(finalwidowpenalty) line_break_((integer) (finalwidowpenalty))
#define linebreak_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void new_hyph_exceptions(void); /* void new_hyph_exceptions(); */
#define newhyphexceptions_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
halfword prune_page_top_(halfword); /* halfword prune_page_top_(); */
#define prune_page_top(p) prune_page_top_((halfword) (p))
#define prunepagetop_regmem register memory_word *mem=zmem;
halfword vert_break_(halfword, scaled, scaled); /* halfword vert_break_(); */
#define vert_break(p, h, d) vert_break_((halfword) (p), (scaled) (h), (scaled) (d))
#define vertbreak_regmem register memory_word *mem=zmem;
halfword vsplit_(eight_bits, scaled); /* halfword vsplit_(); */
#define vsplit(n, h) vsplit_((eight_bits) (n), (scaled) (h))
#define vsplit_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void print_totals(void); /* void print_totals(); */
#define printtotals_regmem
void freeze_page_specs_(small_number); /* void freeze_page_specs_(); */
#define freeze_page_specs(s) freeze_page_specs_((small_number) (s))
#define freezepagespecs_regmem register memory_word *eqtb=zeqtb;
void box_error_(eight_bits); /* void box_error_(); */
#define box_error(n) box_error_((eight_bits) (n))
#define boxerror_regmem register memory_word *eqtb=zeqtb;
void ensure_vbox_(eight_bits); /* void ensure_vbox_(); */
#define ensure_vbox(n) ensure_vbox_((eight_bits) (n))
#define ensurevbox_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void fire_up_(halfword); /* void fire_up_(); */
#define fire_up(c) fire_up_((halfword) (c))
#define fireup_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void build_page(void); /* void build_page(); */
#define buildpage_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void app_space(void); /* void app_space(); */
#define appspace_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void insert_dollar_sign(void); /* void insert_dollar_sign(); */
#define insertdollarsign_regmem
void you_cant(void); /* void you_cant(); */
#define youcant_regmem
void report_illegal_case(void); /* void report_illegal_case(); */
#define reportillegalcase_regmem
bool privileged(void); /* bool privileged(); */
#define privileged_regmem
bool its_all_over(void); /* bool its_all_over(); */
#define itsallover_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void append_glue(void); /* void append_glue(); */
#define appendglue_regmem register memory_word *mem=zmem;
void append_kern(void); /* void append_kern(); */
#define appendkern_regmem register memory_word *mem=zmem;
void off_save(void); /* void off_save(); */
#define offsave_regmem register memory_word *mem=zmem;
void extra_right_brace(void); /* void extra_right_brace(); */
#define extrarightbrace_regmem
void normal_paragraph(void); /* void normal_paragraph(); */
#define normalparagraph_regmem register memory_word *eqtb=zeqtb;
void box_end_(integer); /* void box_end_(); */
#define box_end(boxcontext) box_end_((integer) (boxcontext))
#define boxend_regmem register memory_word *mem=zmem;
void begin_box_(integer); /* void begin_box_(); */
#define begin_box(boxcontext) begin_box_((integer) (boxcontext))
#define beginbox_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void scan_box_(integer); /* void scan_box_(); */
#define scan_box(boxcontext) scan_box_((integer) (boxcontext))
#define scanbox_regmem
void package_(small_number); /* void package_(); */
#define package(c) package_((small_number) (c))
#define package_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
small_number norm_min_(integer); /* small_number norm_min_(); */
#define norm_min(h) norm_min_((integer) (h))
#define normmin_regmem
void new_graf_(bool); /* void new_graf_(); */
#define new_graf(indented) new_graf_((bool) (indented))
#define newgraf_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void indent_in_hmode(void); /* void indent_in_hmode(); */
#define indentinhmode_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void head_for_vmode(void); /* void head_for_vmode(); */
#define headforvmode_regmem
void end_graf(void); /* void end_graf(); */
#define endgraf_regmem register memory_word *eqtb=zeqtb;
void begin_insert_or_adjust(void); /* void begin_insert_or_adjust(); */
#define begininsertoradjust_regmem
void make_mark(void); /* void make_mark(); */
#define makemark_regmem register memory_word *mem=zmem;
void append_penalty(void); /* void append_penalty(); */
#define appendpenalty_regmem register memory_word *mem=zmem;
void delete_last(void); /* void delete_last(); */
#define deletelast_regmem register memory_word *mem=zmem;
void unpackage(void); /* void unpackage(); */
#define unpackage_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void append_italic_correction(void); /* void append_italic_correction(); */
#define appenditaliccorrection_regmem register memory_word *mem=zmem;
void append_discretionary(void); /* void append_discretionary(); */
#define appenddiscretionary_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void build_discretionary(void); /* void build_discretionary(); */
#define builddiscretionary_regmem register memory_word *mem=zmem;
void make_accent(void); /* void make_accent(); */
#define makeaccent_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void align_error(void); /* void align_error(); */
#define alignerror_regmem
void noalign_error(void); /* void noalign_error(); */
#define noalignerror_regmem
void omit_error(void); /* void omit_error(); */
#define omiterror_regmem
void do_endv(void); /* void do_endv(); */
#define doendv_regmem
void cs_error(void); /* void cs_error(); */
#define cserror_regmem
void push_math_(group_code); /* void push_math_(); */
#define push_math(c) push_math_((group_code) (c))
#define pushmath_regmem
void init_math(void); /* void init_math(); */
#define initmath_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void start_eq_no(void); /* void start_eq_no(); */
#define starteqno_regmem register memory_word *eqtb=zeqtb;
void scan_math_(halfword); /* void scan_math_(); */
#define scan_math(p) scan_math_((halfword) (p))
#define scanmath_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void set_math_char_(integer); /* void set_math_char_(); */
#define set_math_char(c) set_math_char_((integer) (c))
#define setmathchar_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void math_limit_switch(void); /* void math_limit_switch(); */
#define mathlimitswitch_regmem register memory_word *mem=zmem;
void scan_delimiter_(halfword, bool); /* void scan_delimiter_(); */
#define scan_delimiter(p, r) scan_delimiter_((halfword) (p), (bool) (r))
#define scandelimiter_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void math_radical(void); /* void math_radical(); */
#define mathradical_regmem register memory_word *mem=zmem;
void math_ac(void); /* void math_ac(); */
#define mathac_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void append_choices(void); /* void append_choices(); */
#define appendchoices_regmem register memory_word *mem=zmem;
halfword fin_mlist_(halfword); /* halfword fin_mlist_(); */
#define fin_mlist(p) fin_mlist_((halfword) (p))
#define finmlist_regmem register memory_word *mem=zmem;
void build_choices(void); /* void build_choices(); */
#define buildchoices_regmem register memory_word *mem=zmem;
void sub_sup(void); /* void sub_sup(); */
#define subsup_regmem register memory_word *mem=zmem;
void math_fraction(void); /* void math_fraction(); */
#define mathfraction_regmem register memory_word *mem=zmem;
void math_left_right(void); /* void math_left_right(); */
#define mathleftright_regmem register memory_word *mem=zmem;
void after_math(void); /* void after_math(); */
#define aftermath_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void resume_after_display(void); /* void resume_after_display(); */
#define resumeafterdisplay_regmem
void get_r_token(void); /* void get_r_token(); */
#define getrtoken_regmem
void trap_zero_glue(void); /* void trap_zero_glue(); */
#define trapzeroglue_regmem register memory_word *mem=zmem;
void do_register_command_(small_number); /* void do_register_command(); */
#define do_register_command(a) do_register_command_((small_number) (a))
#define doregistercommand_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void alter_aux(void); /* void alter_aux(); */
#define alteraux_regmem
void alter_prev_graf(void); /* void alter_prev_graf(); */
#define alterprevgraf_regmem
void alter_page_so_far(void); /* void alter_page_so_far(); */
#define alterpage_so_far_regmem
void alter_integer(void); /* void alter_integer(); */
#define alterinteger_regmem
void alter_box_dimen(void); /* void alter_box_dimen(); */
#define alterboxdimen_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void new_font_(small_number); /* void new_font_(); */
#define new_font(a) new_font_((small_number) (a))
#define newfont_regmem register memory_word *eqtb=zeqtb;
void new_interaction(void); /* void new_interaction(); */
#define newinteraction_regmem
void prefixed_command(void); /* void prefixed_command(); */
#define prefixedcommand_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void do_assignments(void); /* void do_assignments(); */
#define doassignments_regmem
void open_or_close_in(void); /* void open_or_close_in(); */
#define openorclosein_regmem
void issue_message(void); /* void issue_message(); */
#define issuemessage_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void shift_case(void); /* void shift_case(); */
#define shiftcase_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void show_whatever(void); /* void show_whatever(); */
#define showwhatever_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
// void storefmtfile(void);
int storefmtfile(void);
#define storefmtfile_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void new_whatsit_(small_number, small_number); /* void new_whatsit_(); */
#define new_whatsit(s, w) new_whatsit_((small_number) (s), (small_number) (w))
#define newwhatsit_regmem register memory_word *mem=zmem;
void new_write_whatsit_(small_number); /* void new_write_whatsit_(); */
#define new_write_whatsit(w) new_write_whatsit_((small_number) (w))
#define newwritewhatsit_regmem register memory_word *mem=zmem;
void do_extension(void); /* void do_extension(); */
#define doextension_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void fix_language(void); /* void fix_language(); */
#define fixlanguage_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void handle_right_brace(void); /* void handle_right_brace(); */
#define handlerightbrace_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
// void main_control(void); /* void main_control(); */
int main_control(void); /* void main_control(); */
#define maincontrol_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void give_err_help(void); /* void give_err_help(); */
#define giveerrhelp_regmem register memory_word *eqtb=zeqtb;
bool open_fmt_file(void); /* bool open_fmt_file(); */
#define openfmtfile_regmem
bool load_fmt_file(void); /* bool load_fmt_file(); */
#define loadfmtfile_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
void close_files_and_terminate(void); /* void close_files_and_terminate(); */
#define closefilesandterminate_regmem register memory_word *eqtb=zeqtb;
// void final_clean_up(void); /* void final_clean_up(); */
int final_clean_up(void); /* void final_clean_up(); */
#define finalcleanup_regmem register memory_word *mem=zmem;
void init_prim(void); /* void init_prim(); */
#define initprim_regmem register memory_word *eqtb=zeqtb;
void debug_help(void);
#define debughelp_regmem register memory_word *mem=zmem, *eqtb=zeqtb;
/* void texbody(void); */
int texbody(void);          /* 1993/Dec/16 bkph */
#define texbody_regmem register memory_word *eqtb=zeqtb;

#else /* end of ifdef USEREGISTERS */

/* in this case, just let mem be alias for zmem */
/* BUT: allow continued use of register for eqtb at least ! */
#define mem zmem
#ifndef ALLOCATEZEQTB
/* if eqtb is NOT allocated, can just let eqtb be an alias for zeqtb */
#define eqtb zeqtb        /* EXPERIMENT 1996/JAN/18 */
#else
/* uncomment `register memory_word *eqtb=zeqtb' */
/* but with present compiler its faster to not do it this way 99/Jan/18 */
#endif

#ifndef ALLOCATEDVIBUF
/* if dvi_buf is NOT allocated, can just let dvi_buf be an alias for zdvibuf */
  #define dvi_buf zdvibuf      /* EXPERIMENT 1996/JAN/18 */
#else
/* uncomment `register memory_word *dvi_buf=zdvibuf' */
/* but with present compiler its faster to not do it this way 99/Jan/18 */
#endif

/* this basically removes all use for these stupid #define foo_regmem s */

void initialize (void);
void print_ln (void);
void print_char_(ASCII_code);
#define print_char(s) print_char_((ASCII_code) (s))
void print_(integer);
#define print(s) print_((integer) (s))
void slow_print_(integer);
#define slow_print(s) slow_print_((integer) (s))
void print_nl_(str_number);
#define print_nl(s) print_nl_((str_number) (s))
void print_esc_(str_number);
#define print_esc(s) print_esc_((str_number) (s))
void print_the_digs_(eight_bits);
#define print_the_digs(k) print_the_digs_((eight_bits) (k))
void print_int_(integer);
#define print_int(n) print_int_((integer) (n))
void print_cs_(integer);
#define print_cs(p) print_cs_((integer) (p))
void sprint_cs_(halfword);
#define sprint_cs(p) sprint_cs_((halfword) (p))
void print_file_name_(integer, integer, integer);
#define print_file_name(n, a, e) print_file_name_((integer) (n), (integer) (a), (integer) (e))
void print_size_(integer);
#define print_size(s) print_size_((integer) (s))
void print_write_whatsit_(str_number, halfword);
#define print_write_whatsit(s, p) print_write_whatsit_((str_number) (s), (halfword) (p))
void jump_out(void);
void error(void);
void fatal_error_(str_number);
#define fatal_error(s) fatal_error_((str_number) (s))
void overflow_(str_number, integer);
#define overflow(s, n) overflow_((str_number) (s), (integer) (n))
void confusion_(str_number);
#define confusion(s) confusion_((str_number) (s))
bool init_terminal(void);
str_number make_string(void);
bool str_eq_buf_(str_number, integer);
#define str_eq_buf(s, k) str_eq_buf_((str_number) (s), (integer) (k))
bool str_eq_str_(str_number, str_number);
#define str_eq_str(s, t) str_eq_str_((str_number) (s), (str_number) (t))
bool get_strings_started(void);
void print_two_(integer);
#define print_two(n) print_two_((integer) (n))
void print_hex_(integer);
#define print_hex(n) print_hex_((integer) (n))
void print_roman_int_(integer);
#define print_roman_int(n) print_roman_int_((integer) (n))
void print_current_string(void);
void term_input(int, int);
void int_error_(integer);
#define int_error(n) int_error_((integer) (n))
void normalize_selector(void);
void pause_for_instructions(void);
integer half_(integer);
#define half(x) half_((integer) (x))
scaled round_decimals_(small_number);
#define round_decimals(k) round_decimals_((small_number) (k))
void print_scaled_(scaled);
#define print_scaled(s) print_scaled_((scaled) (s))
scaled mult_and_add_(integer, scaled, scaled, scaled);
#define mult_and_add(n, x, y, maxanswer) mult_and_add_((integer) (n), (scaled) (x), (scaled) (y), (scaled) (maxanswer))
scaled x_over_n_(scaled, integer);
#define x_over_n(x, n) x_over_n_((scaled) (x), (integer) (n))
scaled xn_over_d_(scaled, integer, integer);
#define xn_over_d(x, n, d) xn_over_d_((scaled) (x), (integer) (n), (integer) (d))
halfword badness_(scaled, scaled);
#define badness(t, s) badness_((scaled) (t), (scaled) (s))
void print_word_(memory_word);
#define print_word(w) print_word_((memory_word) (w))
void show_token_list_(integer, integer, integer);
#define show_token_list(p, q, l) show_token_list_((integer) (p), (integer) (q), (integer) (l))
void runaway(void);
halfword get_avail(void);
void flush_list_(halfword);
#define flush_list(p) flush_list_((halfword) (p))
halfword get_node_(integer);
#define get_node(s) get_node_((integer) (s))
void free_node_(halfword, halfword);
#define free_node(p, s) free_node_((halfword) (p), (halfword) (s))
void sort_avail(void);
halfword new_null_box(void);
halfword new_rule(void);
halfword new_ligature_(quarterword, quarterword, halfword);
#define new_ligature(f, c, q) new_ligature_((quarterword) (f), (quarterword) (c), (halfword) (q))
halfword new_lig_item_(quarterword);
#define new_lig_item(c) new_lig_item_((quarterword) (c))
halfword new_disc(void);
halfword new_math_(scaled, small_number);
#define new_math(w, s) new_math_((scaled) (w), (small_number) (s))
halfword znewspec(halfword);
#define new_spec(p) znewspec((halfword) (p))
halfword new_param_glue_(small_number);
#define new_param_glue(n) new_param_glue_((small_number) (n))
halfword znewglue(halfword);
#define new_glue(q) znewglue((halfword) (q))
halfword znewskipparam(small_number);
#define new_skip_param(n) znewskipparam((small_number) (n))
halfword new_kern_(scaled);
#define new_kern(w) new_kern_((scaled) (w))
halfword new_penalty_(integer);
#define new_penalty(m) new_penalty_((integer) (m))
void check_mem_(bool);
#define check_mem(printlocs) check_mem_((bool) (printlocs))
void search_mem_(halfword);
#define search_mem(p) search_mem_((halfword) (p))
void short_display_(integer);
#define short_display(p) short_display_((integer) (p))
void print_font_and_char_(integer);
#define print_font_and_char(p) print_font_and_char_((integer) (p))
void print_mark_(integer);
#define print_mark(p) print_mark_((integer) (p))
void print_rule_dimen_(scaled);
#define print_rule_dimen(d) print_rule_dimen_((scaled) (d))
void print_glue_(scaled, integer, str_number);
#define print_glue(d, order, s) print_glue_((scaled) (d), (integer) (order), (str_number) (s))
void print_spec_(integer, str_number);
#define print_spec(p, s) print_spec_((integer) (p), (str_number) (s))
void print_fam_and_char_(halfword);
#define print_fam_and_char(p) print_fam_and_char_((halfword) (p))
void print_delimiter_(halfword);
#define print_delimiter(p) print_delimiter_((halfword) (p))
void print_subsidiary_data_(halfword, ASCII_code);
#define print_subsidiary_data(p, c) print_subsidiary_data_((halfword) (p), (ASCII_code) (c))
void print_style_(integer);
#define print_style(c) print_style_((integer) (c))
void print_skip_param_(integer);
#define print_skip_param(n) print_skip_param_((integer) (n))
void show_node_list_(integer);
#define show_node_list(p) show_node_list_((integer) (p))
void show_box_(halfword);
#define show_box(p) show_box_((halfword) (p))
void delete_token_ref_(halfword);
#define delete_token_ref(p) delete_token_ref_((halfword) (p))
void delete_glue_ref_(halfword);
#define delete_glue_ref(p) delete_glue_ref_((halfword) (p))
void flush_node_list_(halfword);
#define flush_node_list(p) flush_node_list_((halfword) (p))
halfword copy_node_list_(halfword);
#define copy_node_list(p) copy_node_list_((halfword) (p))
void print_mode_(integer);
#define print_mode(m) print_mode_((integer) (m))
void push_nest(void);
void pop_nest(void);
void show_activities(void);
void print_param_(integer);
#define print_param(n) print_param_((integer) (n))
void begin_diagnostic(void);
void end_diagnostic_(bool);
#define end_diagnostic(blankline) end_diagnostic_((bool) (blankline))
void print_length_param_(integer);
#define print_length_param(n) print_length_param_((integer) (n))
void print_cmd_chr_(quarterword, halfword);
#define print_cmd_chr(cmd, chrcode) print_cmd_chr_((quarterword) (cmd), (halfword) (chrcode))
void show_eqtb_(halfword);
#define show_eqtb(n) show_eqtb_((halfword) (n))
halfword id_lookup_(integer, integer);
#define id_lookup(j, l) id_lookup_((integer) (j), (integer) (l))
void primitive_(str_number, quarterword, halfword);
#define primitive(s, c, o) primitive_((str_number) (s), (quarterword) (c), (halfword) (o))
void new_save_level_(group_code);
#define new_save_level(c) new_save_level_((group_code) (c))
void eq_destroy_(memory_word);
#define eq_destroy(w) eq_destroy_((memory_word) (w))
void eq_save_(halfword, quarterword);
#define eq_save(p, l) eq_save_((halfword) (p), (quarterword) (l))
void eq_define_(halfword, quarterword, halfword);
#define eq_define(p, t, e) eq_define_((halfword) (p), (quarterword) (t), (halfword) (e))
void eq_word_define_(halfword, integer);
#define eq_word_define(p, w) eq_word_define_((halfword) (p), (integer) (w))
void geq_define_(halfword, quarterword, halfword);
#define geq_define(p, t, e) geq_define_((halfword) (p), (quarterword) (t), (halfword) (e))
void geq_word_define_(halfword, integer);
#define geq_word_define(p, w) geq_word_define_((halfword) (p), (integer) (w))
void save_for_after_(halfword);
#define save_for_after(t) save_for_after_((halfword) (t))
void restore_trace_(halfword, str_number);
#define restore_trace(p, s) restore_trace_((halfword) (p), (str_number) (s))
void unsave(void);
void prepare_mag(void);
void token_show_(halfword);
#define token_show(p) token_show_((halfword) (p))
void print_meaning(void);
void show_cur_cmd_chr(void);
void show_context(void);
void begin_token_list_(halfword, quarterword);
#define begin_token_list(p, t) begin_token_list_((halfword) (p), (quarterword) (t))
void end_token_list(void);
void back_input(void);
void back_error(void);
void ins_error(void);
void begin_file_reading(void);
void end_file_reading(void);
void clear_for_error_prompt(void);
void check_outer_validity(void);
void get_next(void);
void firm_up_the_line(void);
void get_token(void);
void macro_call(void);
void insert_relax(void);
void expand(void);
void get_x_token(void);
void x_token(void);
void scan_left_brace(void);
void scan_optional_equals(void);
bool scan_keyword_(str_number);
#define scan_keyword(s) scan_keyword_((str_number) (s))
void mu_error(void);
void scan_eight_bit_int(void);
void scan_char_num(void);
void scan_four_bit_int(void);
void scan_fifteen_bit_int(void);
void scan_twenty_seven_bit_int(void);
void scan_font_ident(void);
void find_font_dimen_(bool);
#define find_font_dimen(writing) find_font_dimen_((bool) (writing))
void scan_something_internal_(small_number, bool);
#define scan_something_internal(level, negative) scan_something_internal_((small_number) (level), (bool) (negative))
void scan_int(void);
void scan_dimen_(bool, bool, bool);
#define scan_dimen(mu, inf, shortcut) scan_dimen_((bool) (mu), (bool) (inf), (bool) (shortcut))
void scan_glue_(small_number);
#define scan_glue(level) scan_glue_((small_number) (level))
halfword scan_rule_spec(void);
halfword str_toks_(pool_pointer);
#define str_toks(b) str_toks_((pool_pointer) (b))
halfword the_toks(void);
void ins_the_toks(void);
void conv_toks(void);
halfword scan_toks_(bool, bool);
#define scan_toks(macrodef, xpand) scan_toks_((bool) (macrodef), (bool) (xpand))
void read_toks_(integer, halfword);
#define read_toks(n, r) read_toks_((integer) (n), (halfword) (r))
void pass_text(void);
void change_if_limit_(small_number, halfword);
#define change_if_limit(l, p) change_if_limit_((small_number) (l), (halfword) (p))
void conditional(void);
void begin_name(void);
bool more_name_(ASCII_code);
#define more_name(c) more_name_((ASCII_code) (c))
void end_name(void);
void pack_file_name_(str_number, str_number, str_number);
#define pack_file_name(n, a, e) pack_file_name_((str_number) (n), (str_number) (a), (str_number) (e))
void pack_buffered_name_(small_number, integer, integer);
#define pack_buffered_name(n, a, b) pack_buffered_name_((small_number) (n), (integer) (a), (integer) (b))
str_number make_name_string(void);
str_number a_make_name_string_(alpha_file *);
#define a_make_name_string(f) a_make_name_string_((alpha_file *) &(f))
str_number b_make_name_string_(byte_file *);
#define b_make_name_string(f) b_make_name_string_((byte_file *) &(f))
str_number w_make_name_string_(word_file *);
#define w_make_name_string(f) w_make_name_string_((word_file *) &(f))
void scan_file_name(void);
void pack_job_name_(str_number);
#define pack_job_name(s) pack_job_name_((str_number) (s))
void prompt_file_name_(str_number, str_number);
#define prompt_file_name(s, e) prompt_file_name_((str_number) (s), (str_number) (e))
void open_log_file(void);
void start_input(void);
internal_font_number read_font_info_(halfword, str_number, str_number, scaled);
#define read_font_info(u, nom, aire, s) read_font_info_((halfword) (u), (str_number) (nom), (str_number) (aire), (scaled) (s))
void char_warning_(internal_font_number, eight_bits);
#define char_warning(f, c) char_warning_((internal_font_number) (f), (eight_bits) (c))
halfword new_character_(internal_font_number, eight_bits);
#define new_character(f, c) new_character_((internal_font_number) (f), (eight_bits) (c))
#ifdef ALLOCATEDVIBUF
  void dvi_swap(void);
  void zdvifour(integer);
  #define dvi_four(x) zdvifour((integer) (x))
  void zdvipop(integer);
  #define dvi_pop(l) zdvipop((integer) (l))
  void zdvifontdef(internal_font_number);
  #define dvi_font_def(f) zdvifontdef((internal_font_number) (f))
  void zmovement(scaled, eight_bits);
  #define movement(w, o) zmovement((scaled) (w), (eight_bits) (o))
  void zspecialout(halfword);
  #define special_out(p) zspecialout((halfword) (p))
  void hlist_out(void);
  void vlist_out(void);
  void zshipout(halfword);
  #define ship_out(p) zshipout((halfword) (p))
#else /* not ALLOCATEDVIBUF */
  void dvi_swap(void);
  void zdvifour(integer);
  #define dvi_four(x) zdvifour((integer) (x))
  void zdvipop(integer);
  #define dvi_pop(l) zdvipop((integer) (l))
  void zdvifontdef(internal_font_number);
  #define dvi_font_def(f) zdvifontdef((internal_font_number) (f))
  void zmovement(scaled, eight_bits);
  #define movement(w, o) zmovement((scaled) (w), (eight_bits) (o))
  void zspecialout(halfword);
  #define special_out(p) zspecialout((halfword) (p))
  void hlist_out(void);
  void vlist_out(void);
  void zshipout(halfword);
  #define ship_out(p) zshipout((halfword) (p))
#endif
void prune_movements_(integer);
#define prune_movements(l) prune_movements_((integer) (l))
void write_out_(halfword);
#define write_out(p) write_out_((halfword) (p))
void out_what_(halfword);
#define out_what(p) out_what_((halfword) (p))
void scan_spec_(group_code, bool);
#define scan_spec(c, threecodes) scan_spec_((group_code) (c), (bool) (threecodes))
halfword hpack_(halfword, scaled, small_number);
#define hpack(p, w, m) hpack_((halfword) (p), (scaled) (w), (small_number) (m))
halfword vpackage_(halfword, scaled, small_number, scaled);
#define vpackage(p, h, m, l) vpackage_((halfword) (p), (scaled) (h), (small_number) (m), (scaled) (l))
void append_to_vlist_(halfword);
#define append_to_vlist(b) append_to_vlist_((halfword) (b))
halfword new_noad(void);
halfword new_style_(small_number);
#define new_style(s) new_style_((small_number) (s))
halfword new_choice(void);
void show_info(void);
halfword fraction_rule_(scaled);
#define fraction_rule(t) fraction_rule_((scaled) (t))
halfword overbar_(halfword, scaled, scaled);
#define overbar(b, k, t) overbar_((halfword) (b), (scaled) (k), (scaled) (t))
halfword char_box_(internal_font_number, quarterword);
#define char_box(f, c) char_box_((internal_font_number) (f), (quarterword) (c))
void stack_into_box_(halfword, internal_font_number, quarterword);
#define stack_into_box(b, f, c) stack_into_box_((halfword) (b), (internal_font_number) (f), (quarterword) (c))
scaled height_plus_depth_(internal_font_number, fquarterword); 
#define height_plus_depth(f, c) height_plus_depth_((internal_font_number) (f), (fquarterword) (c))
halfword var_delimiter_(halfword, small_number, scaled);
#define var_delimiter(d, s, v) var_delimiter_((halfword) (d), (small_number) (s), (scaled) (v))
halfword rebox_(halfword, scaled);
#define rebox(b, w) rebox_((halfword) (b), (scaled) (w))
halfword math_glue_(halfword, scaled);
#define math_glue(g, m) math_glue_((halfword) (g), (scaled) (m))
void math_kern_(halfword, scaled);
#define math_kern(p, m) math_kern_((halfword) (p), (scaled) (m))
void flush_math(void);
halfword clean_box_(halfword, small_number);
#define clean_box(p, s) clean_box_((halfword) (p), (small_number) (s))
void fetch_(halfword);
#define fetch(a) fetch_((halfword) (a))
void make_over_(halfword);
#define make_over(q) make_over_((halfword) (q))
void make_under_(halfword);
#define make_under(q) make_under_((halfword) (q))
void make_vcenter_(halfword);
#define make_vcenter(q) make_vcenter_((halfword) (q))
void make_radical_(halfword);
#define make_radical(q) make_radical_((halfword) (q))
void make_math_accent_(halfword);
#define make_math_accent(q) make_math_accent_((halfword) (q))
void make_fraction_(halfword);
#define make_fraction(q) make_fraction_((halfword) (q))
scaled make_op_(halfword);
#define make_op(q) make_op_((halfword) (q))
void make_ord_(halfword);
#define make_ord(q) make_ord_((halfword) (q))
void make_scripts_(halfword, scaled);
#define make_scripts(q, delta) make_scripts_((halfword) (q), (scaled) (delta))
small_number make_left_right_(halfword, small_number, scaled, scaled);
#define make_left_right(q, style, maxd, max_h) make_left_right_((halfword) (q), (small_number) (style), (scaled) (maxd), (scaled) (max_h))
void mlist_to_hlist(void);
void push_alignment(void);
void pop_alignment(void);
void get_preamble_token(void);
void init_align(void);
void init_span_(halfword);
#define init_span(p) init_span_((halfword) (p))
void init_row(void);
void init_col(void);
bool fin_col(void);
void fin_row(void);
void fin_align(void);
void align_peek(void);
halfword finite_shrink_(halfword);
#define finite_shrink(p) finite_shrink_((halfword) (p))
void try_break_(integer, small_number);
#define try_break(pi, breaktype) try_break_((integer) (pi), (small_number) (breaktype))
void post_line_break_(integer);
#define post_line_break(finalwidowpenalty) post_line_break_((integer) (finalwidowpenalty))
small_number reconstitute_(small_number, small_number, halfword, halfword); 
#define reconstitute(j, n, bchar, hchar) reconstitute_((small_number) (j), (small_number) (n), (halfword) (bchar), (halfword) (hchar))
void hyphenate(void);
trie_op_code new_trie_op_(small_number, small_number, trie_op_code);
#define new_trie_op(d, n, v) new_trie_op_((small_number) (d), (small_number) (n), (trie_op_code) (v))
trie_pointer trie_node_(trie_pointer);
#define trie_node(p) trie_node_((trie_pointer) (p))
trie_pointer compress_trie_(trie_pointer);
#define compress_trie(p) compress_trie_((trie_pointer) (p))
void first_fit_(trie_pointer);
#define first_fit(p) first_fit_((trie_pointer) (p))
void trie_pack_(trie_pointer);
#define trie_pack(p) trie_pack_((trie_pointer) (p))
void trie_fix_(trie_pointer);
#define trie_fix(p) trie_fix_((trie_pointer) (p))
void new_patterns(void);
void init_trie(void);
void line_break_(integer);
#define line_break(finalwidowpenalty) line_break_((integer) (finalwidowpenalty))
void new_hyph_exceptions(void);
halfword prune_page_top_(halfword);
#define prune_page_top(p) prune_page_top_((halfword) (p))
halfword vert_break_(halfword, scaled, scaled);
#define vert_break(p, h, d) vert_break_((halfword) (p), (scaled) (h), (scaled) (d))
halfword vsplit_(eight_bits, scaled);
#define vsplit(n, h) vsplit_((eight_bits) (n), (scaled) (h))
void print_totals(void);
void freeze_page_specs_(small_number);
#define freeze_page_specs(s) freeze_page_specs_((small_number) (s))
void box_error_(eight_bits);
#define box_error(n) box_error_((eight_bits) (n))
void ensure_vbox_(eight_bits);
#define ensure_vbox(n) ensure_vbox_((eight_bits) (n))
void fire_up_(halfword);
#define fire_up(c) fire_up_((halfword) (c))
void build_page(void);
void app_space(void);
void insert_dollar_sign(void);
void you_cant(void);
void report_illegal_case(void);
bool privileged(void);
bool its_all_over(void);
void append_glue(void);
void append_kern(void);
void off_save(void);
void extra_right_brace(void);
void normal_paragraph(void);
void box_end_(integer);
#define box_end(boxcontext) box_end_((integer) (boxcontext))
void begin_box_(integer);
#define begin_box(boxcontext) begin_box_((integer) (boxcontext))
void scan_box_(integer);
#define scan_box(boxcontext) scan_box_((integer) (boxcontext))
void package_(small_number);
#define package(c) package_((small_number) (c))
small_number norm_min_(integer);
#define norm_min(h) norm_min_((integer) (h))
void new_graf_(bool);
#define new_graf(indented) new_graf_((bool) (indented))
void indent_in_hmode(void);
void head_for_vmode(void);
void end_graf(void);
void begin_insert_or_adjust(void);
void make_mark(void);
void append_penalty(void);
void delete_last(void);
void unpackage(void);
void append_italic_correction(void);
void append_discretionary(void);
void build_discretionary(void);
void make_accent(void);
void align_error(void);
void noalign_error(void);
void omit_error(void);
void do_endv(void);
void cs_error(void);
void push_math_(group_code);
#define push_math(c) push_math_((group_code) (c))
void init_math(void);
void start_eq_no(void);
void scan_math_(halfword);
#define scan_math(p) scan_math_((halfword) (p))
void set_math_char_(integer);
#define set_math_char(c) set_math_char_((integer) (c))
void math_limit_switch(void);
void scan_delimiter_(halfword, bool);
#define scan_delimiter(p, r) scan_delimiter_((halfword) (p), (bool) (r))
void math_radical(void);
void math_ac(void);
void append_choices(void);
halfword fin_mlist_(halfword);
#define fin_mlist(p) fin_mlist_((halfword) (p))
void build_choices(void);
void sub_sup(void);
void math_fraction(void);
void math_left_right(void);
void after_math(void);
void resume_after_display(void);
void get_r_token(void);
void trap_zero_glue(void);
void do_register_command_(small_number);
#define do_register_command(a) do_register_command_((small_number) (a))
void alter_aux(void);
void alter_prev_graf(void);
void alter_page_so_far(void);
void alter_integer(void);
void alter_box_dimen(void);
void new_font_(small_number);
#define new_font(a) new_font_((small_number) (a))
void new_interaction(void);
void prefixed_command(void);
void do_assignments(void);
void open_or_close_in(void);
void issue_message(void);
void shift_case(void);
void show_whatever(void);
void storefmtfile(void);
void new_whatsit_(small_number, small_number);
#define new_whatsit(s, w) new_whatsit_((small_number) (s), (small_number) (w))
void new_write_whatsit_(small_number);
#define new_write_whatsit(w) new_write_whatsit_((small_number) (w))
void do_extension(void);
void fix_language(void);
void handle_right_brace(void);
void main_control(void);
void give_err_help(void);
bool open_fmt_file(void);
bool load_fmt_file(void);
void close_files_and_terminate(void);
#ifdef ALLOCATEDVIBUF
#define closefilesandterminate_regmem  register eight_bits *dvi_buf=zdvibuf;
#else
#define closefilesandterminate_regmem  /* register memory_word *eqtb=zeqtb; */
#endif
void final_clean_up(void);
void init_prim(void);
void debug_help(void);
int texbody(void);          /* 1993/Dec/16 bkph */

#endif /* end of NOT ifdef USEREGISTERS */

/* may want to consider copying other addresses to local registers ... */

/*
 * The C compiler ignores most unnecessary casts (i.e., casts of something
 * to its own type).  However, for structures, it doesn't.  Therefore,
 * we have to redefine these two macros so that they don't try to cast
 * the argument (a memory_word) as a memory_word.
 */
#undef  eq_destroy
#define eq_destroy(x)  eq_destroy_(x)
#undef  print_word
#define print_word(x)  print_word_(x)
