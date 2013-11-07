/*
 * Common TeX:align.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

struct align_stack_t {
  struct align_stack_t *align_stack_field;
  ptr	preamble_field;
  ptr	cur_align_field;
  ptr	cur_span_field;
  ptr	cur_loop_field;
  ptr	cur_tail_field;
  ptr	cur_head_field;
  int	align_state_field;
};
typedef struct align_stack_t align_stack_t;

#define ALIGN_STACK_NODE_SIZE	sizeof(align_stack_t)

global	align_stack_t *align_stack;

void	push_alignment();
void	pop_alignment();

global	ptr	align_head;

#define preamble	link(align_head)

global	ptr	cur_align;
global	ptr	cur_span;
global	ptr	cur_loop;
global	ptr	cur_head;
global	ptr	cur_tail;

struct span_t {
  int	span_count_field;
  ptr	span_link_field;
  scal	span_width_field;
  ptr	fill_field;
};
typedef struct span_t span_t;

#define SPAN_NODE_SIZE		sizeof(span_t)
#define span_count(P)		((span_t *) (P))->span_count_field
#define span_link(P)		((span_t *) (P))->span_link_field
#define span_width(P)		((span_t *) (P))->span_width_field

global	ptr	end_span;

#define SPAN_CODE		256
#define CR_CODE			257
#define CR_CR_CODE		CR_CODE + 1

void	init_align();
void	get_preamble_token();
void	align_peek();
void	init_row();
void	init_span();
void	init_col();
bool	fin_col();
void	fin_row();
void	fin_align();

void	_align_init();
void	_align_init_once();

/*
 * Common TeX:alloc.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define null 0

global	ptr	avail;

ptr	new_avail();

#ifdef STAT
#define fast_new_avail(M) \
{M = avail; if (M == NULL) M = new_avail(); \
  else {avail = link(M); link(M) = NULL; incr(dyn_used);}}
#define free_avail(M) \
{link(M) = avail; avail = M; decr(dyn_used);}
#else
#define fast_new_avail(M) \
{M = avail; if (M == NULL) M = new_avail(); \
  else {avail = link(M); link(M) = NULL;}}
#define free_avail(M) \
{link(M) = avail; avail = M;}
#endif

global	ptr	rover;

ptr	new_node();
void	free_node();

#ifdef STAT
global	int	dyn_used;
global	int	var_used;
#endif

void	_alloc_init();
void	_alloc_init_once();

/*
 * Common TeX:arith.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define UNITY	0200000
#define TWO	0400000

int	half();
scal	round_decimals();
void	print_scaled();

global	bool	arith_error;
global	scal	remainder;

scal	mult_and_add();

#define nx_plus_y(N,X,Y)	mult_and_add(N,X,Y,07777777777)
#define mult_integers(N,X)	mult_and_add(N,X,0,017777777777)

scal	x_over_n();
scal	xn_over_d();

#define INF_BAD	10000 

int	badness();

void	_arith_init();
void	_arith_init_once();

/*
 * Common TeX:box.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define is_char_node(P)		((P) != null && type(P) >= FONT_BASE)
#define safe_is_char_node(P)	(type(P) >= FONT_BASE)

#define font			type
#define character		subtype

#define HLIST_NODE		0
#define VLIST_NODE		1

struct box_t {
  mcell	node_field;
  scal	width_field;
  scal	depth_field;
  scal	height_field;
  scal	shift_field;
  mcell	node_list_field;
  ratio	glue_set_field;
};
typedef struct box_t box_t;

#define BOX_NODE_SIZE		(sizeof(box_t))
#define box_width(B)		((box_t *) (B))->width_field
#define box_depth(B)		((box_t *) (B))->depth_field
#define box_height(B)		((box_t *) (B))->height_field
#define shift_amount(B)		((box_t *) (B))->shift_field
#define node_list(B)		((ptr) & ((box_t *) (B))->node_list_field)
#define list_ptr(B)		link(node_list(B))
#define glue_order(B)		type(node_list(B))
#define glue_sign(B)		subtype(node_list(B))
#define glue_set(B)		((box_t *) (B))->glue_set_field
#define NORMAL			0
#define STRETCHING		1
#define SHRINKING		2

ptr	new_null_box();

#define RULE_NODE		2

struct rule_t {
  mcell	node_field;
  scal	width_field;
  scal	depth_field;
  scal	height_field;
};
typedef struct rule_t rule_t;

#define RULE_NODE_SIZE		(sizeof(rule_t))
#define rule_width(R)		((rule_t *) (R))->width_field
#define rule_depth(R)		((rule_t *) (R))->depth_field
#define rule_height(R)		((rule_t *) (R))->height_field
#define NULL_FLAG		-010000000000
#define is_running(R)		((R) == NULL_FLAG)

ptr	new_rule();

#define INS_NODE		3

struct ins_t {
  mcell	node_field;
  int	cost_field;
  scal	height_field;
  scal	depth_field;
  ptr	ins_field;
  ptr	split_top_field;
};
typedef struct ins_t ins_t;

#define INS_NODE_SIZE		(sizeof(ins_t))
#define float_cost(I)		((ins_t *) (I))->cost_field
#define ins_height(I)		((ins_t *) (I))->height_field
#define ins_depth(I)		((ins_t *) (I))->depth_field
#define ins_ptr(I)		((ins_t *) (I))->ins_field
#define split_top_ptr(I)	((ins_t *) (I))->split_top_field

#define SMALL_NODE_SIZE		(2 * sizeof(mcell))

#define MARK_NODE		4

struct mark_t {
  mcell	node_field;
  ptr	mark_field;
  int	fill_field;
};
typedef struct mark_t mark_t;

#define mark_ptr(M)		((mark_t *) (M))->mark_field

#define ADJUST_NODE		5

struct adjust_t {
  mcell	node_field;
  ptr	adjust_field;
  int	fill_field;
};
typedef struct adjust_t adjust_t;

#define adjust_ptr(A)		((adjust_t *) (A))->adjust_field

#define LIGATURE_NODE		6

struct lig_t {
  mcell	node_field;
  mcell	lig_field;
};
typedef struct lig_t lig_t;

ptr	new_ligature();
ptr	make_char_from_lig();

#define lig_char(L)		((ptr) &((lig_t *) (L))->lig_field)
#define lig_ptr(L)		link(lig_char(L))

struct disc_t {
  mcell	node_field;
  ptr	pre_field;
  ptr	post_field;
};
typedef struct disc_t disc_t;

ptr	new_disc();

#define DISC_NODE		7
#define replace_count		subtype
#define pre_break(D)		((disc_t *) (D))->pre_field
#define post_break(D)		((disc_t *) (D))->post_field

#define WHATSIT_NODE		8

#define MATH_NODE		9

struct math_t {
  mcell	node_field;
  scal	width_field;
  int	fill_field;
};
typedef struct math_t math_t;

#define math_width(M)		((math_t *) (M))->width_field
#define BEFORE			0
#define AFTER			1

ptr	new_math();

#define precedes_break(M)	(type(M) < MATH_NODE)
#define non_discardable(M)	(type(M) < MATH_NODE)

#define GLUE_NODE		10

struct glue_t {
  mcell	node_field;
  ptr	glue_field;
  ptr	leader_field;
};
typedef struct glue_t glue_t;

#define glue_ptr(G)		((glue_t *) (G))->glue_field
#define leader_ptr(G)		((glue_t *) (G))->leader_field
#define COND_MATH_GLUE		98
#define MU_GLUE			99
#define A_LEADERS		100
#define C_LEADERS		101
#define X_LEADERS		102

struct spec_t {
  int	ref_count_field;
  short	stretch_order_field;
  short	shrink_order_field;
  scal	width_field;
  scal	stretch_field;
  scal	shrink_field;
};
typedef struct spec_t spec_t;

#define GLUE_SPEC_SIZE		sizeof(spec_t)
#define stretch_order(S)	((spec_t *) (S))->stretch_order_field
#define shrink_order(S)		((spec_t *) (S))->shrink_order_field
#define glue_ref_count(S)	((spec_t *) (S))->ref_count_field
#define glue_width(S)		((spec_t *) (S))->width_field
#define stretch(S)		((spec_t *) (S))->stretch_field
#define shrink(S)		((spec_t *) (S))->shrink_field
#define FIL			1
#define FILL			2
#define FILLL			3

ptr	new_spec();
ptr	new_glue();
ptr	new_param_glue();
ptr	new_skip_param();

#define KERN_NODE		11

struct kern_t {
  mcell	node_field;
  scal	kern_field;
  int	fill_field;
};
typedef struct kern_t kern_t;

#define kern_width(K)		((kern_t *) (K))->kern_field
#define EXPLICIT		1
#define ACC_KERN		2

ptr	new_kern();

#define PENALTY_NODE		12

struct pen_t {
  mcell	node_field;
  int	penalty_field;
  int	fill_field;
};
typedef struct pen_t pen_t;

#define penalty(P)		((pen_t *) (P))->penalty_field
#define INF_PENALTY		10000
#define EJECT_PENALTY		-INF_PENALTY

ptr	new_penalty();

#define UNSET_NODE		13

struct unset_t {
  mcell	node_field;
  scal	width_field;
  ptr	u_ptr_field;
  ptr	v_ptr_field;
  scal	stretch_field;
  mcell	info_field;
  scal	shrink_field;
};
typedef struct unset_t unset_t;

#define unset_span_count	subtype
#define unset_span_list		llink
#define	end_link		llink
#define unset_width(U)		((unset_t *) (U))->width_field
#define u_part(U)		((unset_t *) (U))->u_ptr_field
#define v_part(U)		((unset_t *) (U))->v_ptr_field
#define unset_info(U)		info((ptr)&((unset_t *) (U))->info_field)
#define unset_stretch(U)	((unset_t *) (U))->stretch_field
#define unset_shrink(U)		((unset_t *) (U))->shrink_field

void	print_short_display();
void	print_font_and_char();
void	print_mark();
void	print_rule_dimen();
void	print_glue();
void	print_spec(); 
void	print_skip_param();

#define node_list_display(N) \
{append_char('.'); show_node_list(N); flush_char();}

global	fnt	font_in_short_display;
global	int	depth_threshold;
global	int	breadth_max;

void	show_box();
void	short_display();
void	show_node_list();
ptr	copy_node_list();
void	flush_node_list();

void	delete_glue_ref();
#define fast_delete_glue_ref(G)	\
{	if (glue_ref_count(G) == 0) { \
  free_node(G, GLUE_SPEC_SIZE); \
                              } else { \
                                decr(glue_ref_count(G)); \
                              } \
}

#define add_glue_ref(G) \
  (incr(glue_ref_count(G)))

void	_box_init();
void	_box_init_once();

/*
 * Common TeX:boxlist.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

void	handle_right_brace();
void	extra_right_brace();

void	append_glue();

#define FIL_CODE	0
#define FILL_CODE	1
#define SS_CODE		2
#define FIL_NEG_CODE	3
#define SKIP_CODE	4
#define MSKIP_CODE	5

void	append_kern();

global	ptr	cur_box;

void	scan_box();
void	begin_box();
void	box_end();
void	package();

#define BOX_FLAG	010000000000
#define SHIP_OUT_FLAG	(BOX_FLAG + 512)
#define LEADER_FLAG	(BOX_FLAG + 513)
#define BOX_CODE	0
#define COPY_CODE	1
#define LAST_BOX_CODE	2
#define VSPLIT_CODE	3
#define VTOP_CODE	4

void	normal_paragraph();
void	indent_in_hmode();
int	norm_min();
void	new_graf();
void	end_graf();
void	head_for_vmode();
void	append_to_vlist();
void	begin_insert_or_adjust();
void	make_mark();
void	append_penalty();
void	delete_last();
void	unpackage();
void	append_italic_correction();
void	append_discretionary();
void	build_discretionary();
void	flush_discretionary();
void	make_accent();
void	align_error();
void	no_align_error();
void	omit_error();
void	do_endv();
void	cs_error();

void	_boxlist_init();
void	_boxlist_init_once();

/*
 * Common TeX:chr.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	byte	xord[];

#define FIRST_ASCII_CODE	0
#define LAST_ASCII_CODE		255

global	byte	xchr[];

#define FIRST_TEXT_CHAR		0
#define LAST_TEXT_CHAR		255

#define TAB			011
#define FORM_FEED		014
#define CARRIAGE_RETURN		015
#define NULL_CODE		000
#define INVALID_CODE		0177

void	_chr_init();
void	_chr_init_once();

/*
 * Common TeX:cmd.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define ESCAPE			0
#define RELAX			0
#define LEFT_BRACE		1
#define RIGHT_BRACE		2
#define MATH_SHIFT		3
#define TAB_MARK		4
#define CAR_RET			5
#define OUT_PARAM		5
#define MAC_PARAM		6
#define SUP_MARK		7
#define SUB_MARK		8
#define IGNORE			9
#define ENDV			9
#define SPACER			10
#define LETTER			11
#define OTHER_CHAR		12
#define ACTIVE_CHAR		13
#define PAR_END			13
#define MATCH			13
#define COMMENT			14
#define END_MATCH		14
#define STOP			14
#define INVALID_CHAR		15
#define DELIM_NUM		15

#define MAX_CHAR_CODE		15

#define CHAR_NUM		16
#define MATH_CHAR_NUM		17
#define MARK			18
#define XRAY			19
#define MAKE_BOX		20
#define HMOVE			21
#define VMOVE			22
#define UN_HBOX			23
#define UN_VBOX			24
#define REMOVE_ITEM		25
#define HSKIP			26
#define VSKIP			27
#define MSKIP			28
#define KERN			29
#define MKERN			30
#define LEADER_SHIP		31
#define HALIGN			32
#define VALIGN			33
#define NO_ALIGN		34
#define VRULE			35
#define HRULE			36
#define INSERT			37
#define VADJUST			38
#define IGNORE_SPACES		39
#define AFTER_ASSIGNMENT	40
#define AFTER_GROUP		41
#define BREAK_PENALTY		42
#define START_PAR		43
#define ITAL_CORR		44
#define ACCENT			45
#define MATH_ACCENT		46
#define DISCRETIONARY		47
#define EQ_NO			48
#define LEFT_RIGHT		49
#define MATH_COMP		50
#define LIMIT_SWITCH		51
#define ABOVE			52
#define MATH_STYLE		53
#define MATH_CHOICE		54
#define NON_SCRIPT		55
#define VCENTER			56
#define CASE_SHIFT		57
#define MESSAGE			58
#define EXTENSION		59
#define IN_STREAM		60
#define BEGIN_GROUP		61
#define END_GROUP		62
#define OMIT			63
#define EX_SPACE		64
#define NO_BOUNDARY		65
#define RADICAL			66
#define END_CS_NAME		67
#define MIN_INTERNAL		68
#define CHAR_GIVEN		68
#define MATH_GIVEN		69
#define LAST_ITEM		70

#define MAX_NON_PREFIXED_COMMAND	70

#define TOKS_REGISTER		71
#define ASSIGN_TOKS		72
#define ASSIGN_INT		73
#define ASSIGN_DIMEN		74
#define ASSIGN_GLUE		75
#define ASSIGN_MU_GLUE		76
#define ASSIGN_FONT_DIMEN	77
#define ASSIGN_FONT_INT		78
#define SET_AUX			79
#define SET_PREV_GRAF		80
#define SET_PAGE_DIMEN		81
#define SET_PAGE_INT		82
#define SET_BOX_DIMEN		83
#define SET_SHAPE		84
#define DEF_CODE		85
#define DEF_FAMILY		86
#define SET_FONT		87
#define DEF_FONT		88
#define REGISTER		89

#define MAX_INTERNAL		89

#define ADVANCE			90
#define MULTIPLY		91
#define DIVIDE			92
#define PREFIX			93
#define LET			94
#define SHORTHAND_DEF		95
#define READ_TO_CS		96
#define DEF			97
#define SET_BOX			98
#define HYPH_DATA		99
#define SET_INTERACTION		100

#define MAX_COMMAND		100

#define UNDEFINED_CS		(MAX_COMMAND + 1)
#define EXPAND_AFTER		(MAX_COMMAND + 2)
#define NO_EXPAND		(MAX_COMMAND + 3)
#define INPUT			(MAX_COMMAND + 4)
#define IF_TEST			(MAX_COMMAND + 5)
#define FI_OR_ELSE		(MAX_COMMAND + 6)
#define CS_NAME			(MAX_COMMAND + 7)
#define CONVERT			(MAX_COMMAND + 8)
#define THE			(MAX_COMMAND + 9)
#define TOP_BOT_MARK		(MAX_COMMAND + 10)
#define CALL			(MAX_COMMAND + 11)
#define LONG_CALL		(MAX_COMMAND + 12)
#define OUTER_CALL		(MAX_COMMAND + 13)
#define LONG_OUTER_CALL		(MAX_COMMAND + 14)
#define END_TEMPLATE		(MAX_COMMAND + 15)
#define DONT_EXPAND		(MAX_COMMAND + 16)
#define BOX_REG			(MAX_COMMAND + 17)
#define MU_SKIP_REG		(MAX_COMMAND + 18)
#define SKIP_REG		(MAX_COMMAND + 19)
#define DIMEN_REG		(MAX_COMMAND + 20)
#define INT_REG			(MAX_COMMAND + 21)
#define TOKS_REG		(MAX_COMMAND + 22)
#define FNT_REG			(MAX_COMMAND + 23)
#define CAT_CODE_REG		(MAX_COMMAND + 24)
#define LC_CODE_REG		(MAX_COMMAND + 25)
#define UC_CODE_REG		(MAX_COMMAND + 26)
#define SF_CODE_REG		(MAX_COMMAND + 27)
#define MATH_CODE_REG		(MAX_COMMAND + 28)
#define DEL_CODE_REG		(MAX_COMMAND + 29)

void	show_cur_cmd_chr();
void	print_cmd_chr();
void	print_mu_skip_param();
void	print_skip_param();
void	print_param();
void	print_length_param();
void	print_toks_param();

void	_cmd_init();
void	_cmd_init_once();

/*
 * Common TeX:cond.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	ptr	cond_ptr;
global	int	cur_if;
global	int	if_limit;
global	int	if_line;

struct if_t {
  mcell	node;
  int	line_field;
  ptr	fill_field;
};
typedef struct if_t if_t;

#define IF_NODE_SIZE		(sizeof(if_t))
#define if_line_field(I)	((if_t *) (I))->line_field

global	int	skip_line;

#define IF_CODE			1
#define FI_CODE			2
#define ELSE_CODE		3
#define OR_CODE			4

#define IF_CHAR_CODE		0
#define IF_CAT_CODE		1
#define IF_INT_CODE		2
#define IF_DIM_CODE		3
#define IF_ODD_CODE		4
#define IF_VMODE_CODE		5
#define IF_HMODE_CODE		6
#define IF_MMODE_CODE		7
#define IF_INNER_CODE		8
#define IF_VOID_CODE		9
#define IF_HBOX_CODE		10
#define IF_VBOX_CODE		11
#define IFX_CODE		12
#define IF_EOF_CODE		13
#define IF_TRUE_CODE		14
#define IF_FALSE_CODE		15
#define IF_CASE_CODE		16

void	conditional();

void	push_cond();
void	pop_cond();
void	pass_text();
void	change_if_limit();

void	_cond_init();
void	_cond_init_once();

/*
 * Common TeX:def.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	tok	after_token;
global	int	mag_set;
global	bool	long_help_seen;

void	get_r_token();
void	prefixed_command();

#define CHAR_DEF_CODE		0
#define MATH_CHAR_DEF_CODE	1
#define COUNT_DEF_CODE		2
#define DIMEN_DEF_CODE		3
#define SKIP_DEF_CODE		4
#define MU_SKIP_DEF_CODE	5
#define TOKS_DEF_CODE		6

void	do_register_command();
void	trap_zero_glue();
void	alter_aux();
void	alter_prev_graf();
void	alter_page_so_far();
void	alter_integer();
void	alter_box_dimen();

#define WD_CODE			0
#define HT_CODE			1
#define DP_CODE			2

void	new_font();
void	prepare_mag();
void	new_interaction();
void	do_assignments();
void	clopen_stream();
void	issue_message();
void	give_err_help();
void	shift_case();
void	show_whatever();

#define SHOW_CODE		0
#define SHOW_BOX_CODE		1
#define SHOW_THE_CODE		2
#define SHOW_LISTS		3

void	_def_init();
void	_def_init_once();

/*
 * Common TeX:dvi.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	file	dvi_file;
global	str	dvi_name;

#define SET_CHAR_0	0
#define SET1		128
#define SET_RULE	132
#define PUT1		133
#define PUT_RULE	137
#define NOP		138
#define BOP		139
#define EOP		140
#define PUSH		141
#define POP		142
#define RIGHT1		143
#define W0		147
#define W1		148
#define X0		152
#define X1		153
#define DOWN1		157
#define Y0		161
#define Y1		162
#define Z0		166
#define Z1		167
#define FNT_NUM_0	171
#define FNT1		235
#define XXX1		239
#define XXX4		242
#define FNT_DEF1	243
#define PRE		247
#define POST		248
#define POST_POST	249
#define ID_BYTE		2

global	int	dead_cycles;
global	bool	doing_leaders;
global	int	last_bop;
global	scal	max_h;
global	scal	max_v;
global	int	max_push;
global	scal	rule_ht;
global	scal	rule_dp;
global	scal	rule_wd;

global	fnt	dvi_f;
global	scal	dvi_h;
global	scal	dvi_v;
global	scal	cur_h;
global	scal	cur_v;
global	int	cur_s;

global	ptr	down_ptr;
global	ptr	right_ptr;

global	byte	*dvi_buf;
global	ptr	dvi_limit; 
global	ptr	dvi_ptr;
global	int	dvi_offset;
global	int	dvi_gone;
global	bool	output_active;
global	int	total_pages;

struct move_t {
  mcell	node_field;
  scal	movement_field;
  int	location_field;
};
typedef struct move_t move_t;

#define MOVEMENT_NODE_SIZE	sizeof(move_t)
#define move_amount(P)		((move_t *) (P))->movement_field
#define location(P)		((move_t *) (P))->location_field

#define synch_h() \
{if (cur_h != dvi_h) {movement(cur_h - dvi_h, RIGHT1); dvi_h = cur_h;}}

#define synch_v() \
{if (cur_v != dvi_v) {movement(cur_v - dvi_v, DOWN1); dvi_v = cur_v;}}

#define dvi_out(B) \
{dvi_buf[dvi_ptr++] = B; if (dvi_ptr == dvi_limit) dvi_swap();}

#define write_dvi(a, b) \
{fwrite((char *)&dvi_buf[a], sizeof(dvi_buf[0]), b - a, dvi_file);}

void	ship_out();
void	hlist_out();
void	hskip_out();
void	hrule_out();
void	vlist_out();
void	vskip_out();
void	vrule_out();
void	fnt_out();
void	dvi_font_def();
void	dvi_swap();
void	dvi_four();
void	dvi_pop();
void	movement();
void	prune_movements();
void	start_dvi();
void	fin_dvi();

void	_dvi_init();
void	_dvi_init_once();

/*
 * Common TeX:error.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	int	interaction;

#define BATCH_MODE		0
#define NONSTOP_MODE		1
#define SCROLL_MODE		2
#define ERROR_STOP_MODE		3

#define wake_up_terminal()

void	begin_diagnostic();
void	end_diagnostic();

void	print_err();

global	bool	deletions_allowed;
global	int	error_cnt;

#ifdef NOHELP 
#define help0()
#define help1(h0)
#define help2(h0, h1)
#define help3(h0, h1, h2)
#define help4(h0, h1, h2, h3)
#define help5(h0, h1, h2, h3, h4) 
#define help6(h0, h1, h2, h3, h4, h5)
#else
#define help0() \
{help_cnt = 0;}

#define help1(h0) \
{help_cnt = 1; help_line[0] = h0;}

#define help2(h0, h1) \
{help_cnt = 2; \
  help_line[0] = h0; help_line[1] = h1;}

#define help3(h0, h1, h2) \
{help_cnt = 3; help_line[0] = h0; \
  help_line[1] = h1; help_line[2] = h2;}

#define help4(h0, h1, h2, h3) \
{help_cnt = 4; \
  help_line[0] = h0; help_line[1] = h1; \
  help_line[2] = h2; help_line[3] = h3;}

#define help5(h0, h1, h2, h3, h4) \
{help_cnt = 5; help_line[0] = h0; \
  help_line[1] = h1; help_line[2] = h2; \
  help_line[3] = h3; help_line[4] = h4;}

#define help6(h0, h1, h2, h3, h4, h5) \
{help_cnt = 6; \
  help_line[0] = h0; help_line[1] = h1; \
  help_line[2] = h2; help_line[3] = h3; \
  help_line[4] = h4; help_line[5] = h5;}
#endif

global	str	help_line[];
global	int	help_cnt;
global	bool	use_err_help;

void	jump_out();
void	error();
void	normalize_selector();

global	int	history;

#define SPOTLESS		0
#define WARNING_ISSUED		1
#define ERROR_MESSAGE_ISSUED	2
#define FATAL_ERROR_STOP	3

void	int_error();
void	fatal_error();
void	overflow();
void	confusion();
void	succumb();

global	bool	interrupted;
global	bool	OK_to_interrupt;

void	pause_for_instructions();
#define check_interrupt() \
{if (interrupted) pause_for_instructions();}

void	_error_init();
void	_error_init_once();

/*
 * Common TeX:eval.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define VMODE		1
#define HMODE		(VMODE + MAX_COMMAND + 1)
#define MMODE		(HMODE + MAX_COMMAND + 1)

global	ptr	lig_stack;
global	int	cur_l;
global	int	cur_r;
global	ptr	cur_q;
global	bool	lft_hit;
global	bool	rt_hit;
global	bool	ligature_present;

global	void	main_control();
global	void	app_space();

global void	push_nest();
global void	pop_nest();
global bool	realloc_nest();
global void	print_mode();

#define IGNORE_DEPTH	-65536000

struct list {
  int	mode_field;
  ptr	head_field;
  ptr	tail_field;
  int	pg_field;
  int	aux_field;
  int	ml_field;
  int	clang_field;
  short	lhm_field;
  short	rhm_field;
};
typedef struct list list;

global	list	cur_list;

global	list	*nest;
global	list	*nest_end;
global	list	*nest_ptr;
global	list	*max_nest_stack;

#define mode			cur_list.mode_field
#define head			cur_list.head_field
#define tail			cur_list.tail_field
#define prev_graf		cur_list.pg_field
#define aux			cur_list.aux_field
#define prev_depth		aux
#define space_factor		aux
#define incompleat_noad		aux
#define mode_line		cur_list.ml_field
#define clang			cur_list.clang_field
#define lhmin			cur_list.lhm_field
#define rhmin			cur_list.rhm_field

global	int		shown_mode;

global void	show_activities();

#define tail_append(N)		{tail = link(tail) = N;}

global void	insert_dollar_sign();
global void	you_cant();
global void	report_illegal_case();
global bool	privileged();
global void	missing_font();
global bool	its_all_over();

global void	_eval_init();
global void	_eval_init_once();

/*
 * Common TeX:expand.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

void	get_x_token();
void	expand();
void	insert_relax();

#define TOP_MARK_CODE		0
#define FIRST_MARK_CODE		1
#define BOT_MARK_CODE		2
#define SPLIT_FIRST_MARK_CODE	3
#define SPLIT_BOT_MARK_CODE	4

#define top_mark		cur_mark[TOP_MARK_CODE]
#define first_mark		cur_mark[FIRST_MARK_CODE]
#define bot_mark		cur_mark[BOT_MARK_CODE]
#define split_first_mark	cur_mark[SPLIT_FIRST_MARK_CODE]
#define split_bot_mark		cur_mark[SPLIT_BOT_MARK_CODE]

global	ptr	cur_mark[];

global	int	long_state;
global	ptr	pstack[];

void	macro_call();
void	x_token();
void	runaway_arg();

void	_expand_init();
void	_expand_init_once();

/*
 * Common TeX:fileio.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	int	name_length;
global	str	name_of_file;

global	str	cur_name;
global	str	cur_area;
global	str	cur_ext;

global	int	area_delimiter;
global	int	ext_delimiter;
global	int	path_delimiter;

void	print_file_name();
void	pack_file_name();

str	make_name_str();

global	bool	name_in_progress;

void	prompt_file_name();
void	scan_file_name();

#define pack_cur_name() \
{pack_file_name(cur_name, cur_area, cur_ext);}

void	pack_job_name();
void	prompt_job_name();

global	str	job_name;
global	str	job_area;

global	bool	log_opened;

void	start_input();

#define null_file ((FILE *) 0)

FILE	*a_open_in();
FILE	*a_open_out();

FILE	*b_open_in();
FILE	*b_open_out();

#define a_close(FP)		{fflush(FP);fclose(FP);}
#define b_close(FP)		{fflush(FP);fclose(FP);}

bool 	init_terminal();
bool	input_ln();
void	term_input();
#define prompt_input(S)		{print(S); term_input();}

#define term_in			stdin
#define term_out		stdout
#define t_open_in()
#define t_open_out()
#define update_terminal()	fflush(stdout)
#define clear_terminal()

void	open_log_file();

global	file	read_file[];
global	int	read_open[];

#define OPENED			1
#define CLOSED			2

bool	test_access();

#define READ_ACCESS		4
#define WRITE_ACCESS 		2

global	char	*input_path;
global	char	*format_path;
global	char	*font_path;

void	set_paths();

#define default_font_path	":/usr/local/lib/tex/fonts/tfm"
#define default_input_path 	":/usr/local/lib/tex/inputs"

#define NO_FILE_PATH 		0
#define INPUT_FILE_PATH 	1
#define FONT_FILE_PATH 		2

global	str	str_dvi;
global	str	str_tex;
global	str	str_log;
global	str	str_tfm;
global	str	str_texput;

void	_fileio_init();
void	_fileio_init_once();

/*
 * Common TeX:hyph.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	ptr	ha;
global	ptr	hb;
global	fnt	hf;
global	int	hn;
global	int	hc[];
global	int	hu[];

global	int	l_hyf;
global	int	r_hyf;
global	int	cur_lang;

global	ptr	init_list;
global	bool	init_lig;
global	bool	init_lft;

struct trie_t {
  short	s;
  union {
    short	s;
    struct {
      byte	b0;
      byte	b1;
    } bb;
  } u_s;
};
typedef struct trie_t trie_t;

#define trie_op(T)		trie[T].u_s.bb.b0
#define trie_char(T)		trie[T].u_s.bb.b1
#define trie_back(T)		trie[T].u_s.s
#define trie_link(T)		trie[T].s

global	trie_t	*trie;

global	int	*op_start;
global	int	*hyf_distance;
global	int	*hyf_num;
global	int	*hyf_next;

global	int	hyph_count;
global	str	*hyph_word;
global	ptr	*hyph_list;

void	new_hyph_exceptions();
void	enter_hyph_exception();

void	init_hyph();
void	try_hyph();
void	hyphenate();

global	int	hyf[];
global	int	hyf_char;

global	int	hyphen_passed;

int	reconstitute();

global	int	trie_op_ptr;
global	int	*trie_op_hash;
global	int	*trie_op_val;
global	int	*trie_op_lang;
global	int	*trie_op_used;

int	new_trie_op();

#define trie_root	trie_l[0]

global	int	trie_ptr;
global	int	trie_max;
global	int	*trie_c;
global	int	*trie_o;
global	int	*trie_l;
global	int	*trie_r;
global	int	*trie_hash;
global	int	*trie_min;

int	trie_compress();
int	trie_node();

#define trie_ref	trie_hash

global	bool	*trie_taken;

void	first_fit();
void	trie_pack();
void	trie_fix();

void	new_patterns();
void	init_trie();

void	init_pattern_memory();
void	free_pattern_memory();

global	bool	trie_not_ready;

void	_hyph_init();
void	_hyph_init_once();

/*
 * Common TeX:math.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

struct style_t {
  mcell	noad_field;
  ptr	display_field;
  ptr	text_field;
  ptr	script_field;
  ptr	script_script_field;
};
typedef struct style_t style_t;

ptr	new_style();

#define STYLE_NODE		(UNSET_NODE + 1)
#define STYLE_NODE_SIZE		sizeof(style_t)
#define DISPLAY_STYLE		0
#define TEXT_STYLE		2
#define SCRIPT_STYLE		4
#define SCRIPT_SCRIPT_STYLE	6
#define CRAMPED			1

ptr	new_choice();

#define CHOICE_NODE		(UNSET_NODE + 2)
#define display_mlist(C)	((style_t *) (C))->display_field
#define text_mlist(C)		((style_t *) (C))->text_field
#define script_mlist(C)		((style_t *) (C))->script_field
#define script_script_mlist(C)	((style_t *) (C))->script_script_field

struct noad_t {
  mcell	noad_field;
  mcell	nucleus_field;
  mcell	supscr_field;
  mcell	subscr_field;
};
typedef struct noad_t noad_t;

ptr	new_noad();

#define NOAD_SIZE		sizeof(noad_t)
#define nucleus(N)		((ptr) &((noad_t *) (N))->nucleus_field)
#define supscr(N)		((ptr) &((noad_t *) (N))->supscr_field)
#define subscr(N)		((ptr) &((noad_t *) (N))->subscr_field)
#define math_type		rinfo
#define math_link		llink

#define MATH_CHAR		1
#define SUB_BOX			2
#define SUB_MLIST		3
#define MATH_TEXT_CHAR		4

#define fam			font

#define ORD_NOAD		(UNSET_NODE + 3)
#define OP_NOAD			(ORD_NOAD + 1)
#define BIN_NOAD		(ORD_NOAD + 2)
#define REL_NOAD		(ORD_NOAD + 3)
#define OPEN_NOAD		(ORD_NOAD + 4)
#define CLOSE_NOAD		(ORD_NOAD + 5)
#define PUNCT_NOAD		(ORD_NOAD + 6)
#define INNER_NOAD		(ORD_NOAD + 7)

#define LIMITS			1
#define NO_LIMITS		2

struct delimiter_t {
  mcell	noad_field;
  mcell	nucleus_field;
  mcell	supscr_field;
  mcell	subscr_field;
  mcell	left_field;
  mcell	right_field;
};
typedef struct delimiter_t delimiter_t;

#define left_delimiter(D)	((ptr) &((delimiter_t *) (D))->left_field)
#define right_delimiter(D)	((ptr) &((delimiter_t *) (D))->right_field)

#define small_fam		q0
#define small_char		q1
#define large_fam		q2
#define large_char		q3

#define RADICAL_NOAD		(INNER_NOAD + 1)
#define RADICAL_NOAD_SIZE	(5 * sizeof(mcell))
#define FRACTION_NOAD		(RADICAL_NOAD + 1)
#define FRACTION_NOAD_SIZE	(6 * sizeof(mcell))
#define DEFAULT_CODE		010000000000
#define thickness		box_width
#define numerator		supscr
#define denominator		subscr

#define UNDER_NOAD		(FRACTION_NOAD + 1)
#define OVER_NOAD		(UNDER_NOAD + 1)

#define ACCENT_NOAD		(OVER_NOAD + 1)

struct accent_t {
  mcell	noad_field;
  mcell	nucleus_field;
  mcell	supscr_field;
  mcell	subscr_field;
  mcell	accent_field;
};
typedef struct accent_t accent_t;

#define ACCENT_NOAD_SIZE	(sizeof(accent_t))
#define accent_chr(A)		((ptr) &((accent_t *) (A))->accent_field)

#define VCENTER_NOAD		(ACCENT_NOAD + 1)
#define LEFT_NOAD		(VCENTER_NOAD + 1)
#define RIGHT_NOAD		(LEFT_NOAD + 1)
#define delimiter		nucleus	

#define scripts_allowed(N)	(type(N) >= ORD_NOAD && type(N) < LEFT_NOAD)

void	print_fam_and_char();
void	print_delimiter();
void	print_subsidiary_data();
void	print_style();
void	print_size();

#define TEXT_SIZE		0
#define SCRIPT_SIZE		16
#define SCRIPT_SCRIPT_SIZE	32

void	show_choice_node();
void	show_normal_noad();
void	show_fraction_noad();

#define mathsy(S, P)		param(fam_fnt(2 + S), P)

#define math_x_height(S)	mathsy(S, 5)
#define math_quad(S)		mathsy(S, 6)
#define num1(S)			mathsy(S, 8)
#define num2(S)			mathsy(S, 9)
#define num3(S)			mathsy(S, 10)
#define denom1(S)		mathsy(S, 11)
#define denom2(S)		mathsy(S, 12)
#define sup1(S)			mathsy(S, 13)
#define sup2(S)			mathsy(S, 14)
#define sup3(S)			mathsy(S, 15)
#define sub1(S)			mathsy(S, 16)
#define sub2(S)			mathsy(S, 17)
#define sup_drop(S)		mathsy(S, 18)
#define sub_drop(S)		mathsy(S, 19)
#define delim1(S)		mathsy(S, 20)
#define delim2(S)		mathsy(S, 21)
#define axis_height(S)		mathsy(S, 22)

#define TOTAL_MATHSY_PARAMS	22

#define mathex(P) 		param(fam_fnt(3 + cur_size), P)

#define default_rule_thickness	mathex(8)
#define big_op_spacing1		mathex(9)
#define big_op_spacing2		mathex(10)
#define big_op_spacing3		mathex(11)
#define big_op_spacing4		mathex(12)
#define big_op_spacing5		mathex(13)

#define TOTAL_MATHEX_PARAMS	13

#define cramped_style(S)	(2 * ((S) / 2) + CRAMPED)
#define sub_style(S)		(2 * ((S) / 4) + SCRIPT_STYLE + CRAMPED)
#define sup_style(S)		(2 * ((S) / 4) + SCRIPT_STYLE + ((S) % 2))
#define num_style(S)		((S) + 2 - 2 * ((S) / 6))
#define denom_style(S)		(2 * ((S) / 2) + CRAMPED + 2 - 2 * ((S) / 6))

ptr	fraction_rule();
ptr	overbar();
ptr	var_delimiter();
void	stack_into_box();
scal	height_plus_depth();
ptr	char_box();
ptr	rebox();
ptr	math_glue();
void	math_kern();
void	flush_math();

void	_math_init();
void	_math_init_once();

/*
 * Common TeX:mathlist.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define ABOVE_CODE	0
#define OVER_CODE	1
#define ATOP_CODE	2
#define DELIMITED_CODE	3
#define VAR_CODE	070000

void	push_math();
void	init_math();
void	start_eq_no();
void	scan_math();
void	set_math_char();
void	math_limit_switch();
void	scan_delimiter();
void	math_radical();
void	math_ac();
void	append_choices();
void	build_choices();
void	sub_sup();
void	math_fraction();
ptr	fin_mlist();
void	math_left_right();
void	after_math();
void	check_dollar();
bool	check_font_params();
void	resume_after_display();

void	_mathlist_init();
void	_mathlist_init_once();

/*
 * Common TeX:mlst-hlst.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	int	cur_c;
global	fnt	cur_f;
global	qcell	cur_i;
global	scal	cur_mu;
global	int	cur_size;

ptr	mlist_to_hlist();
void	make_over();
void	make_under();
void	make_vcenter();
void	make_radical();
void	make_math_accent();
void	make_fraction();
scal	make_op();
void	make_ord();
void	make_scripts();
int	make_left_right();
void	fetch();
ptr	clean_box();

void	_mlst_hlst_init();
void	_mlst_hlst_init_once();

/*
 * Common TeX:pack.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define EXACTLY		0
#define ADDITIONAL	1
#define NATURAL		0, ADDITIONAL

global	ptr	adjust_head;
global	ptr	adjust_tail;

global	scal	total_stretch[];
global	scal	total_shrink[];

#define get_stretch_order() \
  (total_stretch[FILLL] != 0) ? FILLL : \
(total_stretch[FILL] != 0) ? FILL : \
(total_stretch[FIL] != 0) ? FIL : \
NORMAL

#define get_shrink_order() \
  (total_shrink[FILLL] != 0) ? FILLL : \
(total_shrink[FILL] != 0) ? FILL : \
(total_shrink[FIL] != 0) ? FIL : \
NORMAL

global	int	last_badness;
global	int	pack_begin_line;

ptr	hpack();
#define vpack(P, H)	vpackage(P, H, MAX_DIMEN)
ptr	vpackage();

void	_pack_init();
void	_pack_init_once();

/*
 * Common TeX:page.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

void	build_page();
void	insert_page();
ptr	vert_break();
ptr	vsplit();
ptr	prune_page_top();

global	ptr	contrib_head;

#define contrib_tail	 nest[0].tail_field

global	ptr	page_tail;
global	ptr	page_head;
global	ptr	page_ins_head;
global	int	page_contents;

#define INSERTS_ONLY	1
#define BOX_THERE	2

#define active_height	active_width
#define cur_height	active_height[1]


global	scal	best_height_plus_depth;

#define DEPLORABLE	100000

global	ptr	best_page_break;
global	scal	best_size;
global	scal	page_max_depth;
global	int	least_page_cost;

struct page_ins_t {
  mcell	node_field;
  ptr	broken_ptr_field;
  ptr	broken_ins_field;
  ptr	last_ins_ptr_field;
  ptr	best_ins_ptr_field;
  scal	page_ins_height_field;
};
typedef struct page_ins_t page_ins_t;

#define PAGE_INS_NODE_SIZE	sizeof(page_ins_t)
#define INSERTING		0
#define SPLIT_UP		1
#define broken_ptr(P)		((page_ins_t *) (P))->broken_ptr_field
#define broken_ins(P)		((page_ins_t *) (P))->broken_ins_field
#define last_ins_ptr(P)		((page_ins_t *) (P))->last_ins_ptr_field
#define best_ins_ptr(P)		((page_ins_t *) (P))->best_ins_ptr_field
#define page_ins_height(P)	((page_ins_t *) (P))->page_ins_height_field

global	scal	page_so_far[];

#define page_goal	page_so_far[0]
#define page_total	page_so_far[1]
#define page_shrink	page_so_far[6]
#define page_depth	page_so_far[7]

global	ptr	last_glue;
global	int	last_penalty;
global	scal	last_kern;
global	int	insert_penalties;

void	print_totals();

#define start_new_page() \
{page_contents = EMPTY; \
  page_tail = page_head; \
  link(page_head) = null; \
  last_glue = null; \
  last_penalty = 0; \
  last_kern = 0; \
  page_depth = 0; \
  page_max_depth = 0;}

void	freeze_page_specs();

global	bool	output_active;

int	page_badness();
int	vert_badness();
void	ensure_vbox();
void	box_error();
void	show_page_stats();
void	show_split();

void	fire_up();
bool	insert_box();

void	_page_init();
void	_page_init_once();

/*
 * Common TeX:par.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

struct passive_t {
  mcell	node_field;
  ptr	cur_break_field;
  ptr	prev_break_field;
};
typedef struct passive_t passive_t;

#define PASSIVE_NODE_SIZE	sizeof(passive_t)
#define serial			info
#define cur_break(P)		((passive_t *) (P))->cur_break_field
#define prev_break(P)		((passive_t *) (P))->prev_break_field
#define next_break		prev_break

global	ptr	passive;

struct active_t {
  mcell	node_field;
  ptr	break_node_field;
  int	line_number_field;
  int	total_demerits_field;
};
typedef struct active_t active_t;

#define ACTIVE_NODE_SIZE	sizeof(active_t)
#define UNHYPHENATED		0
#define HYPHENATED		1
#define fitness			subtype
#define VERY_LOOSE_FIT		0
#define LOOSE_FIT		1
#define DECENT_FIT		2
#define TIGHT_FIT		3
#define break_node(P)		((active_t *) (P))->break_node_field
#define line_number(P)		((active_t *) (P))->line_number_field
#define total_demerits(P)	((active_t *) (P))->total_demerits_field

global	ptr	active;
#define last_active		active

#define DELTA_NODE		2

struct delta_t {
  mcell	node_field;
  scal	delta_field[7];
};
typedef struct delta_t delta_t;

#define DELTA_NODE_SIZE		sizeof(delta_t)
#define deltas(P)		((delta_t *) (P))->delta_field

#define do_all_six(F) \
{F(1); F(2); F(3); F(4); F(5); F(6);}

global	ptr	just_box;

void	line_break();

global	ptr	printed_node;
global	int	pass_number;

global	scal	active_width[];
global	scal	cur_active_width[];
global	scal	background[];
global	scal	break_width[];
global	bool	no_shrink_error_yet;

ptr	finite_shrink();

global	ptr	cur_p;
global	bool	second_pass;
global	bool	final_pass;
global	int	threshold;

void	try_break();

#define AWFUL_BAD		07777777777

global	int	fit_class;
global	int	minimal_demerits[];
global	int	minimum_demerits;
global	ptr	best_place[];
global	int	best_pl_line[];

global	int	easy_line;
global	int	last_special_line;
global	scal	first_width;
global	scal	second_width;
global	scal	first_indent;
global	scal	second_indent;
global	scal	line_width;
global	scal	disc_width;
global	ptr	best_bet;
global	int	fewest_demerits;
global	int	best_line;
global	int	line_diff;
global	int	actual_looseness;

void	post_line_break();

void	set_disc_width();
void	set_act_width();
void	set_break_width();
int	get_best_bet();
int	get_badness();
ptr	get_break_node();
void	show_break_node();
void	show_break_status();
void	update_printed_node();

void	_par_init();
void	_par_init_once();

/*
 * Common TeX:print.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	file	log_file;
global	str	log_name;

global	int	selector;

#define NO_PRINT	16 
#define TERM_ONLY	17
#define LOG_ONLY	18
#define TERM_AND_LOG	19
#define PSEUDO		20
#define NEW_STRING	21
#define MAX_SELECTOR	21

global	int	term_offset;
global	int	file_offset;

global	int	tally;
global	int	first_count;
global	int	trick_count;
global	int	trick_buf[];

int	begin_pseudoprint();
void	set_trick_count();

#define HALF_ERROR_LINE		32
#define ERROR_LINE		64
#define MAX_PRINT_LINE		72

global	int	dig[];

void	print();
void	print_char();
void	print_ASCII();
void	print_ln();
void	print_esc();
void	print_nl();
void	print_the_digs();
void	print_int();
void	print_two();
void	print_hex();
void	print_roman_int();

#define wterm(c)	putchar(c);
#define wterm_ln(c)	{putchar(c); putchar('\n');}
#define wterm_cr()	putchar('\n');
#define wlog(c)		putc(c, log_file)
#define wlog_ln(c)	{putc(c, log_file); putc('\n', log_file);}
#define wlog_cr()	putc('\n', log_file);
#define wfile(c)	putc(c, write_file[selector])
#define wfile_ln(c)	{putc(c, write_file[selector]); \
  putc('\n', write_file[selector]);}
#define wfile_cr()	putc('\n', write_file[selector]);

void	_print_init();
void	_print_init_once();

/*
 * Common TeX:scan.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

void	scan_left_brace();
void	scan_optional_equals();
bool	scan_keyword();

global	int	cur_val_level;
global	int	cur_val;

void	scan_something_internal();

void	mu_error();

#define INT_VAL		0
#define DIMEN_VAL	1
#define GLUE_VAL	2
#define MU_VAL		3
#define IDENT_VAL	4
#define TOK_VAL		5

#define INPUT_LINE_NO_CODE	3
#define BADNESS_CODE		4

void	scan_eight_bit_int();
void	scan_char_num();
void	scan_four_bit_int();
void	scan_fifteen_bit_int();
void	scan_twenty_seven_bit_int();

void	scan_int();
#define INFINITY	017777777777

global	int	radix;

#define PLUS_TOKEN		(OTHER_TOKEN + '+')
#define MINUS_TOKEN		(OTHER_TOKEN + '-')
#define ZERO_TOKEN		(OTHER_TOKEN + '0')
#define A_TOKEN			(LETTER_TOKEN + 'A')
#define OTHER_A_TOKEN		(OTHER_TOKEN + 'A')
#define OCTAL_TOKEN		(OTHER_TOKEN + '\'')
#define HEX_TOKEN		(OTHER_TOKEN + '"')
#define ALPHA_TOKEN		(OTHER_TOKEN + '`')
#define POINT_TOKEN		(OTHER_TOKEN + '.')
#define EURO_POINT_TOKEN	(OTHER_TOKEN + ',')

void	scan_dimen();
#define MAX_DIMEN	07777777777

#define scan_normal_dimen()	scan_dimen(FALSE, FALSE, FALSE)

global	int	cur_order;

void	scan_glue();

#define scan_optional_space() \
{get_x_token(); if (cur_cmd != SPACER) back_input();}

#define get_nbx_token() \
{do get_x_token(); while (cur_cmd == SPACER);}

#define get_nbrx_token() \
{do get_x_token(); while (cur_cmd == SPACER || cur_cmd == RELAX);}

void	scan_spec();
ptr	scan_rule_spec();

void	_scan_init();
void	_scan_init_once();

/*
 * Common TeX:str.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	str	null_str;

global	str	ASCII[];

global	str	cur_str;
global	str	cur_str_end;
global	str	cur_str_ptr;

#define STR_SIZE		4096

str	new_str();
void	free_str();
void	str_room();

#define append_char(C) 		(*cur_str_ptr++ = C)
#define flush_char()		(decr(cur_str_ptr))
#define cur_length()		(cur_str_ptr - cur_str)
#define flush_str()		(cur_str_ptr = cur_str)
#define print_str()		{*cur_str_ptr = 0; print(cur_str);}

str	make_str();

#define str_length(S)		(strlen(S))
#define str_eq(S1,S2)		(strcmp(S1,S2)==0)
#define str_eqn(S1,S2,N)	(memcmp(S1,S2,N)==0)
#define make_str_given(S)	(strcpy(new_str(str_length(S)), S))

void	_str_init();
void	_str_init_once();

/*
 * Common TeX:sym.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

struct reg_t {
  short	type_field;
  short	level_field;
  int	equiv_field;
};
typedef struct reg_t reg_t, *reg;

#define reg_type(R)		((reg) (R))->type_field
#define reg_level(R)		((reg) (R))->level_field
#define reg_equiv(R)		((reg) (R))->equiv_field

struct sym_t {
  reg_t	reg_field;
  int	length_field;
  str	text_field;
  struct sym_t *hash_field;
};
typedef struct sym_t sym_t, *sym;

#define sym2reg(S)		((reg) & ((sym) (S))->reg_field)
#define reg2sym(R)		((sym) R)
#define eq_type(S)		reg_type(sym2reg(S))
#define eq_level(S)		reg_level(sym2reg(S))
#define equiv(S)		reg_equiv(sym2reg(S))
#define length(S)		((sym) (S))->length_field
#define text(S)			((sym) (S))->text_field
#define hash(S)			((sym) (S))->hash_field

#define null_sym ((sym) 0)

global	sym	cur_cs;
global	sym	warning_cs;
global	sym	par_cs;
global	sym	par_shape_cs;
global	sym	undefined_cs;
global	sym	null_cs;

global	sym	*active_base;
global	sym	*single_base;
global	sym	*hash_base;
global	sym	*frozen_cs_base;

global	sym	*font_id_base;

#define font_id(F)		font_id_base[F]
#define font_id_text(F)		text(font_id_base[F])
#define font_id_length(F)	length(font_id_base[F])

global	reg	*skip_reg;
global	reg	*mu_skip_reg;
global	reg	*box_reg;
global	reg	*int_reg;
global	reg	*dimen_reg;
global	reg	*toks_reg;
global	reg	*fnt_reg;
global	reg	*cat_code_reg;
global	reg	*lc_code_reg;
global	reg	*uc_code_reg;
global	reg	*sf_code_reg;
global	reg	*math_code_reg;
global	reg	*del_code_reg;

#define MU_GLUE_PARS			3
#define mu_glue_par(G)			((ptr) reg_equiv(mu_skip_reg[G]))

#define THIN_MU_SKIP_CODE		0
#define MED_MU_SKIP_CODE		1
#define THICK_MU_SKIP_CODE		2

#define thin_mu_skip			mu_glue_par(THIN_MU_SKIP_CODE)
#define med_mu_skip			mu_glue_par(MED_MU_SKIP_CODE)
#define thick_mu_skip			mu_glue_par(THICK_MU_USKIP_CODE)

#define GLUE_PARS			15
#define glue_par(G)			((ptr) reg_equiv(skip_reg[G]))

#define LINE_SKIP_CODE			0
#define BASELINE_SKIP_CODE		1
#define PAR_SKIP_CODE			2
#define ABOVE_DISPLAY_SKIP_CODE		3
#define BELOW_DISPLAY_SKIP_CODE		4
#define ABOVE_DISPLAY_SHORT_SKIP_CODE	5
#define BELOW_DISPLAY_SHORT_SKIP_CODE	6
#define LEFT_SKIP_CODE			7
#define RIGHT_SKIP_CODE			8
#define TOP_SKIP_CODE			9
#define SPLIT_TOP_SKIP_CODE		10
#define TAB_SKIP_CODE			11
#define SPACE_SKIP_CODE			12
#define XSPACE_SKIP_CODE		13
#define PAR_FILL_SKIP_CODE		14

#define line_skip			glue_par(LINE_SKIP_CODE)
#define baseline_skip			glue_par(BASELINE_SKIP_CODE)
#define par_skip			glue_par(PAR_SKIP_CODE)
#define above_display_skip		glue_par(ABOVE_DISPLAY_SKIP_CODE)
#define below_display_skip		glue_par(BELOW_DISPLAY_SKIP_CODE)
#define above_display_short_skip	glue_par(ABOVE_DISPLAY_SHORT_SKIP_CODE)
#define below_display_short_skip	glue_par(BELOW_DISPLAY_SHORT_SKIP_CODE)
#define left_skip			glue_par(LEFT_SKIP_CODE)
#define right_skip			glue_par(RIGHT_SKIP_CODE)
#define top_skip			glue_par(TOP_SKIP_CODE)
#define split_top_skip			glue_par(SPLIT_TOP_SKIP_CODE)
#define tab_skip			glue_par(TAB_SKIP_CODE)
#define space_skip			glue_par(SPACE_SKIP_CODE)
#define xspace_skip			glue_par(XSPACE_SKIP_CODE)
#define par_fill_skip			glue_par(PAR_FILL_SKIP_CODE)

#define DIMEN_PARS			21
#define dimen_par(D)			((scal) reg_equiv(dimen_reg[D]))

#define PAR_INDENT_CODE			0
#define MATH_SURROUND_CODE		1
#define LINE_SKIP_LIMIT_CODE		2
#define HSIZE_CODE			3
#define VSIZE_CODE			4
#define MAX_DEPTH_CODE			5
#define SPLIT_MAX_DEPTH_CODE		6
#define BOX_MAX_DEPTH_CODE		7
#define HFUZZ_CODE			8
#define VFUZZ_CODE			9
#define DELIMITER_SHORTFALL_CODE	10
#define NULL_DELIMITER_SPACE_CODE	11
#define SCRIPT_SPACE_CODE		12
#define PRE_DISPLAY_SIZE_CODE		13
#define DISPLAY_WIDTH_CODE		14
#define DISPLAY_INDENT_CODE		15
#define OVERFULL_RULE_CODE		16
#define HANG_INDENT_CODE		17
#define H_OFFSET_CODE			18
#define V_OFFSET_CODE			19
#define EMERGENCY_STRETCH_CODE		20

#define par_indent			dimen_par(PAR_INDENT_CODE)
#define math_surround			dimen_par(MATH_SURROUND_CODE)
#define line_skip_limit			dimen_par(LINE_SKIP_LIMIT_CODE)
#define hsize				dimen_par(HSIZE_CODE)
#define vsize				dimen_par(VSIZE_CODE)
#define max_depth			dimen_par(MAX_DEPTH_CODE)
#define split_max_depth			dimen_par(SPLIT_MAX_DEPTH_CODE)
#define box_max_depth			dimen_par(BOX_MAX_DEPTH_CODE)
#define hfuzz				dimen_par(HFUZZ_CODE)
#define vfuzz				dimen_par(VFUZZ_CODE)
#define delimiter_shortfall		dimen_par(DELIMITER_SHORTFALL_CODE)
#define null_delimiter_space		dimen_par(NULL_DELIMITER_SPACE_CODE)
#define script_space			dimen_par(SCRIPT_SPACE_CODE)
#define pre_display_size		dimen_par(PRE_DISPLAY_SIZE_CODE)
#define display_width			dimen_par(DISPLAY_WIDTH_CODE)
#define display_indent			dimen_par(DISPLAY_INDENT_CODE)
#define overfull_rule			dimen_par(OVERFULL_RULE_CODE)
#define hang_indent			dimen_par(HANG_INDENT_CODE)
#define h_offset			dimen_par(H_OFFSET_CODE)
#define v_offset			dimen_par(V_OFFSET_CODE)
#define emergency_stretch		dimen_par(EMERGENCY_STRETCH_CODE)

#define INT_PARS			55
#define int_par(I)			((int) reg_equiv(int_reg[I]))

#define PRETOLERANCE_CODE		0
#define TOLERANCE_CODE			1
#define LINE_PENALTY_CODE		2
#define HYPHEN_PENALTY_CODE		3		
#define CLUB_PENALTY_CODE		4
#define EX_HYPHEN_PENALTY_CODE		5
#define WIDOW_PENALTY_CODE		6
#define DISPLAY_WIDOW_PENALTY_CODE	7
#define BROKEN_PENALTY_CODE		8
#define BIN_OP_PENALTY_CODE		9
#define REL_PENALTY_CODE		10
#define PRE_DISPLAY_PENALTY_CODE	11
#define POST_DISPLAY_PENALTY_CODE	12
#define INTER_LINE_PENALTY_CODE		13
#define DOUBLE_HYPHEN_DEMERITS_CODE	14
#define FINAL_HYPHEN_DEMERITS_CODE	15
#define ADJ_DEMERITS_CODE		16
#define MAG_CODE			17
#define DELIMITER_FACTOR_CODE		18
#define LOOSENESS_CODE			19
#define TIME_CODE			20
#define DAY_CODE			21
#define MONTH_CODE			22
#define YEAR_CODE			23
#define SHOW_BOX_BREADTH_CODE		24
#define SHOW_BOX_DEPTH_CODE		25
#define HBADNESS_CODE			26
#define VBADNESS_CODE			27
#define PAUSING_CODE			28
#define TRACING_ONLINE_CODE		29
#define TRACING_MACROS_CODE		30
#define TRACING_STATS_CODE		31
#define TRACING_PARAGRAPHS_CODE		32
#define TRACING_PAGES_CODE		33
#define TRACING_OUTPUT_CODE		34
#define TRACING_LOST_CHARS_CODE		35
#define TRACING_COMMANDS_CODE		36
#define TRACING_RESTORES_CODE		37
#define UC_HYPH_CODE			38
#define OUTPUT_PENALTY_CODE		39
#define MAX_DEAD_CYCLES_CODE		40
#define HANG_AFTER_CODE			41
#define FLOATING_PENALTY_CODE		42
#define GLOBAL_DEFS_CODE		43
#define CUR_FAM_CODE			44
#define ESCAPE_CHAR_CODE		45
#define DEFAULT_HYPHEN_CHAR_CODE	46
#define DEFAULT_SKEW_CHAR_CODE		47
#define END_LINE_CHAR_CODE		48
#define NEW_LINE_CHAR_CODE		49
#define LANGUAGE_CODE			50
#define LEFT_HYPHEN_MIN_CODE		51
#define RIGHT_HYPHEN_MIN_CODE		52
#define HOLDING_INSERTS_CODE		53
#define ERROR_CONTEXT_LINES_CODE	54

#define pretolerance			int_par(PRETOLERANCE_CODE)
#define tolerance			int_par(TOLERANCE_CODE)
#define line_penalty			int_par(LINE_PENALTY_CODE)
#define hyphen_penalty			int_par(HYPHEN_PENALTY_CODE)
#define ex_hyphen_penalty		int_par(EX_HYPHEN_PENALTY_CODE)
#define club_penalty			int_par(CLUB_PENALTY_CODE)
#define widow_penalty			int_par(WIDOW_PENALTY_CODE)
#define display_widow_penalty		int_par(DISPLAY_WIDOW_PENALTY_CODE)
#define broken_penalty			int_par(BROKEN_PENALTY_CODE)
#define bin_op_penalty			int_par(BIN_OP_PENALTY_CODE)
#define rel_penalty			int_par(REL_PENALTY_CODE)
#define pre_display_penalty		int_par(PRE_DISPLAY_PENALTY_CODE)
#define post_display_penalty		int_par(POST_DISPLAY_PENALTY_CODE)
#define inter_line_penalty		int_par(INTER_LINE_PENALTY_CODE)
#define double_hyphen_demerits		int_par(DOUBLE_HYPHEN_DEMERITS_CODE)
#define final_hyphen_demerits		int_par(FINAL_HYPHEN_DEMERITS_CODE)
#define adj_demerits			int_par(ADJ_DEMERITS_CODE)
#define mag				int_par(MAG_CODE)
#define delimiter_factor		int_par(DELIMITER_FACTOR_CODE)
#define looseness			int_par(LOOSENESS_CODE)
#define time				int_par(TIME_CODE)
#define day				int_par(DAY_CODE)
#define month				int_par(MONTH_CODE)
#define year				int_par(YEAR_CODE)
#define show_box_breadth		int_par(SHOW_BOX_BREADTH_CODE)
#define show_box_depth			int_par(SHOW_BOX_DEPTH_CODE)
#define hbadness			int_par(HBADNESS_CODE)
#define vbadness			int_par(VBADNESS_CODE)
#define pausing				int_par(PAUSING_CODE)
#define tracing_online			int_par(TRACING_ONLINE_CODE)
#define tracing_macros			int_par(TRACING_MACROS_CODE)
#define tracing_stats			int_par(TRACING_STATS_CODE)
#define tracing_paragraphs		int_par(TRACING_PARAGRAPHS_CODE)
#define tracing_pages			int_par(TRACING_PAGES_CODE)
#define tracing_output			int_par(TRACING_OUTPUT_CODE)
#define tracing_lost_chars		int_par(TRACING_LOST_CHARS_CODE)
#define tracing_commands		int_par(TRACING_COMMANDS_CODE)
#define tracing_restores		int_par(TRACING_RESTORES_CODE)
#define uc_hyph				int_par(UC_HYPH_CODE)
#define max_dead_cycles			int_par(MAX_DEAD_CYCLES_CODE)
#define output_penalty			int_par(OUTPUT_PENALTY_CODE)
#define hang_after			int_par(HANG_AFTER_CODE)
#define floating_penalty		int_par(FLOATING_PENALTY_CODE)
#define global_defs			int_par(GLOBAL_DEFS_CODE)
#define cur_fam				int_par(CUR_FAM_CODE)
#define escape_char			int_par(ESCAPE_CHAR_CODE)
#define default_hyphen_char		int_par(DEFAULT_HYPHEN_CHAR_CODE)
#define default_skew_char		int_par(DEFAULT_SKEW_CHAR_CODE)
#define end_line_char			int_par(END_LINE_CHAR_CODE)
#define new_line_char			int_par(NEW_LINE_CHAR_CODE)
#define language			int_par(LANGUAGE_CODE)
#define left_hyphen_min			int_par(LEFT_HYPHEN_MIN_CODE)
#define right_hyphen_min		int_par(RIGHT_HYPHEN_MIN_CODE)
#define holding_inserts			int_par(HOLDING_INSERTS_CODE)
#define error_context_lines		int_par(ERROR_CONTEXT_LINES_CODE)

#define TOK_PARS			9
#define tok_par(T)			((ptr) reg_equiv(toks_reg[T]))

#define OUTPUT_ROUTINE_CODE		0
#define EVERY_PAR_CODE			1
#define EVERY_MATH_CODE			2
#define EVERY_DISPLAY_CODE		3
#define EVERY_HBOX_CODE			4
#define EVERY_VBOX_CODE			5
#define EVERY_JOB_CODE			6
#define EVERY_CR_CODE			7
#define ERR_HELP_CODE			8

#define output_routine_reg		toks_reg[OUTPUT_ROUTINE_CODE]
#define output_routine			tok_par(OUTPUT_ROUTINE_CODE)
#define every_par			tok_par(EVERY_PAR_CODE)
#define every_math			tok_par(EVERY_MATH_CODE)
#define every_display			tok_par(EVERY_DISPLAY_CODE)
#define every_hbox			tok_par(EVERY_HBOX_CODE)
#define every_vbox			tok_par(EVERY_VBOX_CODE)
#define every_job			tok_par(EVERY_JOB_CODE)
#define every_cr			tok_par(EVERY_CR_CODE)
#define err_help			tok_par(ERR_HELP_CODE)

#define FROZEN_CONTROLS			11
#define FROZEN_CONTROL_SEQUENCE		frozen_cs_base
#define FROZEN_PROTECTION		FROZEN_CONTROL_SEQUENCE[0]
#define FROZEN_CR			FROZEN_CONTROL_SEQUENCE[1]
#define FROZEN_END_GROUP		FROZEN_CONTROL_SEQUENCE[2]
#define FROZEN_RIGHT			FROZEN_CONTROL_SEQUENCE[3]
#define FROZEN_FI			FROZEN_CONTROL_SEQUENCE[4]
#define FROZEN_END_TEMPLATE		FROZEN_CONTROL_SEQUENCE[5]
#define FROZEN_ENDV			FROZEN_CONTROL_SEQUENCE[6]
#define FROZEN_RELAX			FROZEN_CONTROL_SEQUENCE[7]
#define FROZEN_END_WRITE		FROZEN_CONTROL_SEQUENCE[8]
#define FROZEN_DONT_EXPAND		FROZEN_CONTROL_SEQUENCE[9]
#define FROZEN_NULL_FONT		FROZEN_CONTROL_SEQUENCE[10]

#define cur_font_ptr		fnt_reg[0]
#define cur_font		reg_equiv(cur_font_ptr)
#define fam_fnt(R)		reg_equiv(fnt_reg[(R)+1])

#define mu_skip(R)		reg_equiv(mu_skip_reg[MU_GLUE_PARS + (R)])
#define skip(R)			reg_equiv(skip_reg[GLUE_PARS + (R)])
#define dimen(R)		reg_equiv(dimen_reg[DIMEN_PARS + (R)])
#define count(R)		reg_equiv(int_reg[INT_PARS + (R)])
#define toks(R)			reg_equiv(toks_reg[TOK_PARS + (R)])
#define box(R)			reg_equiv(box_reg[R])

#define CAT_CODE		0
#define LC_CODE			1
#define UC_CODE			2
#define SF_CODE			3
#define MATH_CODE		4
#define DEL_CODE		5

#define cat_code(R)		reg_equiv(cat_code_reg[R])
#define lc_code(R)		reg_equiv(lc_code_reg[R])
#define uc_code(R)		reg_equiv(uc_code_reg[R])
#define sf_code(R)		reg_equiv(sf_code_reg[R])
#define math_code(R)		reg_equiv(math_code_reg[R])
#define del_code(R)		reg_equiv(del_code_reg[R])

#define par_shape_ptr		equiv(par_shape_cs)

struct shape_t {
  scal	indent_field;
  scal	width_field;
};
typedef struct shape_t shape_t;

struct par_shape_t {
  mcell 	node_field;
  shape_t	par_shape_field[1];
};
typedef struct par_shape_t par_shape_t;

#define par_shape	((par_shape_t *) par_shape_ptr)->par_shape_field

#define par_shape_indent(I)	par_shape[I - 1].indent_field
#define par_shape_width(I)	par_shape[I - 1].width_field

global	ptr	hold_head;
global	ptr	zero_glue;
global	ptr	fil_glue;
global	ptr	fill_glue;
global	ptr	ss_glue;
global	ptr	fil_neg_glue;

global	bool	no_new_control_sequence;
global	int	cs_count;

sym	id_lookup();
sym	new_sym();

sym	primitive();
sym	freeze();

void	print_cs();
void	sprint_cs();

bool	issingle();
bool	isactive();
bool	isfrozen();

global	int	cur_level;
global	int	cur_group;

global	int	nsaves;
global	reg	save_stack;
global	reg	save_end;
global	reg	save_ptr;
global	reg	max_save_stack;

#define save_type	reg_type
#define save_level	reg_level
#define save_index	reg_equiv

#define saved(I)	save_index(save_ptr+I)

#define RESTORE_OLD_VALUE		0
#define INSERT_TOKEN			1
#define LEVEL_BOUNDARY			2

#define LEVEL_ZERO			MIN_QUARTERWORD
#define LEVEL_ONE			(LEVEL_ZERO + 1)
#define BOTTOM_LEVEL			0
#define SIMPLE_GROUP			1
#define HBOX_GROUP			2
#define ADJUSTED_HBOX_GROUP		3
#define VBOX_GROUP			4
#define VTOP_GROUP			5
#define ALIGN_GROUP			6
#define NO_ALIGN_GROUP			7
#define OUTPUT_GROUP			8
#define MATH_GROUP			9
#define DISC_GROUP			10
#define INSERT_GROUP			11
#define VCENTER_GROUP			12
#define MATH_CHOICE_GROUP		13
#define SEMI_SIMPLE_GROUP		14
#define MATH_SHIFT_GROUP		15
#define MATH_LEFT_GROUP			16
#define MAX_GROUP_CODE			16

void	new_save_level();
bool	realloc_save_stack();
void	reg_destroy();
void	reg_save();
void	eq_define();
void	reg_define();
void	eq_gdefine();
void	reg_gdefine();
void	save_for_after();
void	unsave();
void	off_save();
void	restore_trace();
void	show_reg();

void	_sym_init();
void	_sym_init_once();

/*
 * Common TeX:tex.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#include	<stdio.h>
#include	<string.h>
#include	<signal.h>
#include	<time.h>
#include	<malloc.h>

/*
 * constants in the outer block
 */

#define NUL			'\0'
#define EOLN			'\n'
#define FALSE			0
#define TRUE			1
#define EMPTY			0

#define MIN_QUARTERWORD		0
#define MAX_QUARTERWORD		255
#define MIN_HALFWORD		0
#define MAX_HALFWORD		65535

/*
 *	types in the outer block
 */

#define global		extern

typedef int		bool;
typedef unsigned char	byte;
typedef int		tok;
typedef int		fnt;
typedef float		ratio;
typedef int 		scal;
typedef char		*str;
typedef FILE		*file;

typedef int		ptr;

struct qcell {
  short	b0;
  short	b1;
  short	b2;
  short	b3;
};
typedef struct qcell qcell;

union hcell { 
  int	i;   
  ptr	p;
  struct {
    short	b0;
    short	b1;
  } qq;
};
typedef union hcell hcell;

struct mcell { 
  hcell	h1;
  hcell	h2;
};
typedef struct mcell mcell;

#define rinfo(P)	((mcell *) (P))->h1.i
#define link(P)		((mcell *) (P))->h1.p
#define info(P)		((mcell *) (P))->h2.i
#define llink(P)	((mcell *) (P))->h2.p
#define type(P)		((mcell *) (P))->h2.qq.b0
#define subtype(P)	((mcell *) (P))->h2.qq.b1
#define	q0(P)		((mcell *) (P))->h1.qq.b0
#define	q1(P)		((mcell *) (P))->h1.qq.b1
#define	q2(P)		((mcell *) (P))->h2.qq.b0
#define	q3(P)		((mcell *) (P))->h2.qq.b1

#ifdef NOLCASTS
#define mcopy(x,y)	memcpy((void *)x, (void *)y, sizeof(mcell))
#else
#define mcopy(x,y)	*(mcell *)x = *(mcell *)y
#endif
#define mzero(m)	memset(m,0,sizeof(mcell))

/*
 *	variables in the outer block
 */

global	char	*banner;
global	str	format_ident;
global	int	ready_already;

/*
 *	functions in the outer block
 */

void	initialize();
void	initialize_once();
bool	decode_args();
void	fix_date_and_time();
void	handle_int();
void	final_cleanup();
void	close_files_and_terminate();
void	call_edit();

void	_tex_init ();
void	_tex_init_once ();

/*
 * some common programming idioms
 */

#define incr(i)		++(i)
#define decr(i)	 	--(i)
#define odd(i)		((i) & 1)
#define abs(i)		((i) >= 0 ? (i) : -(i))
#define round(x)	(int) ((x) > 0.0 ? ((x) + 0.5) : ((x) - 0.5))
#define negate(x)	(x) = -(x)
#define loop		while (1)

#include "align.h"
#include "alloc.h"
#include "arith.h"
#include "box.h"
#include "boxlist.h"
#include "chr.h"
#include "cmd.h"
#include "cond.h"
#include "def.h"
#include "dvi.h"
#include "sym.h"
#include "error.h"
#include "eval.h"
#include "expand.h"
#include "fileio.h"
#include "hyph.h"
#include "math.h"
#include "mathlist.h"
#include "mlst-hlst.h"
#include "pack.h"
#include "page.h"
#include "par.h"
#include "print.h"
#include "scan.h"
#include "str.h"
#include "tfm.h"
#include "tok.h"
#include "toklist.h"
#include "texext.h"

/*
 * Common TeX:texext.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

void	do_extension();

void	show_whatsit();
ptr	copy_whatsit();
void	free_whatsit();
void	out_whatsit();
void	line_break_whatsit();
void	try_hyph_whatsit();
void	append_whatsit();

#define OPEN_NODE		0

struct _open_t {
  mcell	node_field;
  int	open_stream_field;
  str	open_name_field;
  str	open_area_field;
  str	open_ext_field;
};
typedef struct _open_t _open_t;

#define OPEN_NODE_SIZE		sizeof(_open_t)
#define open_stream(P)		((_open_t *) (P))->open_stream_field
#define open_name(P)		((_open_t *) (P))->open_name_field
#define open_area(P)		((_open_t *) (P))->open_area_field
#define open_ext(P)		((_open_t *) (P))->open_ext_field

void	do_open();
void	show_open();
ptr	copy_open();
void	free_open();
void	out_open();

#define WRITE_NODE		1

struct _write_t {
  mcell	node_field;
  int	write_stream_field;
  ptr	write_toks_field;
};
typedef struct _write_t _write_t;

#define WRITE_NODE_SIZE		sizeof(_write_t)
#define write_stream(P)		((_write_t *) (P))->write_stream_field
#define write_toks(P)		((_write_t *) (P))->write_toks_field

#define END_WRITE_TOKEN		sym2tok(FROZEN_END_WRITE)

global	file	write_file[];
global	bool	write_open[];
global	sym	write_cs;

void	do_write();
void	show_write();
ptr	copy_write();
void	free_write();
void	out_write();

#define CLOSE_NODE		2

struct _close_t {
  mcell	node_field;
  int	close_stream_field;
  ptr	fill_field;
};
typedef struct _close_t _close_t;

#define CLOSE_NODE_SIZE		sizeof(_close_t)
#define close_stream(P)		((_close_t *) (P))->close_stream_field

void	do_close();
void	show_close();
ptr	copy_close();
void	free_close();
void	out_close();

#define SPECIAL_NODE		3

struct _special_t {
  mcell	node_field;
  ptr	special_toks_field;
  ptr	fill_field;
};
typedef struct _special_t _special_t;

#define SPECIAL_NODE_SIZE	sizeof(_special_t)
#define special_toks(P)		((_special_t *) (P))->special_toks_field

void	do_special();
void	show_special();
ptr	copy_special();
void	free_special();
void	out_special();

#define IMMEDIATE_CODE		4

void	do_immediate();

#define SET_LANGUAGE_CODE	5
#define LANGUAGE_NODE		4

struct _language_t {
  mcell	node_field;
  int	lang_field;
  short	lhm_field;
  short	rhm_field;
};
typedef struct _language_t _language_t;

#define LANGUAGE_NODE_SIZE	sizeof(_language_t)
#define what_lang(P)		((_language_t *)(P))->lang_field
#define what_lhm(P)		((_language_t *)(P))->lhm_field
#define what_rhm(P)		((_language_t *)(P))->rhm_field

void	do_set_language();
void	show_language();
ptr	copy_language();
void	free_language();
void	out_language();
void	fix_language();

void	_texext_init();
void	_texext_init_once();

/*
 * Common TeX:tfm.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

global	file	tfm_file;

struct font_struct {
  int	font_length_field;
  qcell	font_check_field;
  scal	font_size_field;
  scal	font_dsize_field;
  int	font_params_field;
  str	font_name_field;
  str	font_area_field;
  int	font_bc_field;
  int	font_ec_field;
  ptr	font_glue_field;
  bool	font_used_field;
  int	hyphen_char_field;
  int	skew_char_field;
  int	font_bchar_field;
  int	font_false_bchar_field;
  qcell	*char_base_field;
  scal	*width_base_field;
  scal	*height_base_field;
  scal	*depth_base_field;
  scal	*italic_base_field;
  qcell	*lig_kern_base_field;
  qcell	*bchar_label_field;
  scal	*kern_base_field;
  qcell	*exten_base_field;
  scal	*param_base_field;
};
typedef struct font_struct font_t;

global	int	nfonts;
global	font_t	**font_info;
global	font_t	**font_end;
global	font_t	**font_ptr;

#define FONT_BASE	32
#define FONT_MAX	1250
#define null_font	FONT_BASE

global	qcell	null_character;

#define fmem(F)	(font_info[(F) - null_font])

#define font_length(F)		fmem(F)->font_length_field
#define font_check(F)		fmem(F)->font_check_field
#define font_size(F)		fmem(F)->font_size_field
#define font_dsize(F)		fmem(F)->font_dsize_field
#define font_params(F)		fmem(F)->font_params_field
#define font_name(F)		fmem(F)->font_name_field
#define font_area(F)		fmem(F)->font_area_field
#define font_bc(F)		fmem(F)->font_bc_field
#define font_ec(F)		fmem(F)->font_ec_field
#define font_glue(F)		fmem(F)->font_glue_field
#define font_used(F)		fmem(F)->font_used_field
#define hyphen_char(F)		fmem(F)->hyphen_char_field
#define skew_char(F)		fmem(F)->skew_char_field
#define font_bchar(F)		fmem(F)->font_bchar_field
#define font_false_bchar(F)	fmem(F)->font_false_bchar_field
#define bchar_label(F)		fmem(F)->bchar_label_field
#define char_base(F)		fmem(F)->char_base_field
#define width_base(F)		fmem(F)->width_base_field
#define height_base(F)		fmem(F)->height_base_field
#define depth_base(F)		fmem(F)->depth_base_field
#define italic_base(F)		fmem(F)->italic_base_field
#define lig_kern_base(F)	fmem(F)->lig_kern_base_field
#define kern_base(F)		fmem(F)->kern_base_field
#define param_base(F)		fmem(F)->param_base_field
#define exten_base(F)		fmem(F)->exten_base_field

#define NON_CHAR		256
#define NON_ADDRESS		0

#define char_info(F, C)		char_base(F)[C]
#define char_exists(CI)		((CI).b0 > 0)
#define char_width(F, CI)	width_base(F)[(CI).b0]
#define height_depth(CI)	(CI).b1
#define char_height(F, CB)	height_base(F)[(CB) / 16]
#define char_depth(F, CB)	depth_base(F)[(CB) % 16]
#define char_italic(F, CI)	italic_base(F)[(CI).b2 / 4]
#define char_tag(CI)		((CI).b2 % 4)

#define NO_TAG			0
#define LIG_TAG			1
#define LIST_TAG		2
#define EXT_TAG			3

#define skip_byte(LK)		(LK).b0
#define next_char(LK)		(LK).b1
#define op_byte(LK)		(LK).b2
#define rem_byte(LK)		(LK).b3
#define lig_offset(LK)		((op_byte(LK) * 256) + rem_byte(LK))
#define kern_offset(LK)		(((op_byte(LK) - 128) * 256) + rem_byte(LK))
#define char_kern(F, LK)	kern_base(F)[kern_offset(LK)]
#define lig_kern_start(F, LK)	(lig_kern_base(F) + rem_byte(LK))
#define lig_kern_restart(F, LK)	(lig_kern_base(F) + lig_offset(LK))

#define STOP_FLAG		128
#define KERN_FLAG		128

#define ext_top(M)		M.b0
#define ext_mid(M)		M.b1
#define ext_bot(M)		M.b2
#define ext_rep(M)		M.b3

#define SLANT_CODE		1
#define SPACE_CODE		2
#define SPACE_STRETCH_CODE	3
#define SPACE_SHRINK_CODE	4
#define X_HEIGHT_CODE		5
#define QUAD_CODE		6
#define EXTRA_SPACE_CODE	7

#define param(F, CODE)		param_base(F)[CODE]
#define slant(F)		param(F, SLANT_CODE)
#define space(F)		param(F, SPACE_CODE)
#define space_stretch(F)	param(F, SPACE_STRETCH_CODE)
#define space_shrink(F)		param(F, SPACE_SHRINK_CODE)
#define x_height(F)		param(F, X_HEIGHT_CODE)
#define quad(F)			param(F, QUAD_CODE)
#define extra_space(F)		param(F, EXTRA_SPACE_CODE)

fnt	read_font_info();
void	scan_font_ident();
scal	*find_font_dimen();
ptr	find_font_glue();

ptr	new_character();
void	char_warning();

void	_tfm_init();
void	_tfm_init_once();

/*
 * Common TeX:tok.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define token_link(T)	link(T)
#define token(T)	info(T)

#define LEFT_BRACE_TOKEN	0400
#define LEFT_BRACE_LIMIT	01000
#define RIGHT_BRACE_TOKEN	01000
#define RIGHT_BRACE_LIMIT	01400
#define MATH_SHIFT_TOKEN	01400
#define TAB_TOKEN		02000
#define OUT_PARAM_TOKEN		02400
#define SPACE_TOKEN		05040
#define LETTER_TOKEN		05400
#define OTHER_TOKEN		06000
#define MATCH_TOKEN		06400
#define END_MATCH_TOKEN		07000
#define CS_TOKEN_FLAG		010000
#define is_sym(TOK)		((TOK) >= CS_TOKEN_FLAG)
#define sym2tok(SYM)		((tok) (CS_TOKEN_FLAG + (char *) (SYM)))
#define tok2sym(TOK)		((sym) ((TOK) - CS_TOKEN_FLAG))
#define END_TEMPLATE_TOKEN	sym2tok(FROZEN_END_TEMPLATE)

#define NO_EXPAND_FLAG		257

#define null_tok ((tok) 0)

global	tok	cur_tok;
global	tok	par_tok;

global	int	cur_cmd;
global	int	cur_chr;

void	get_token();
void	get_next();
void	get_cs();
void	check_outer_validity();
void	firm_up_the_line();

#define end_line_char_active	(end_line_char >= 0 && end_line_char <= 255)

global	bool	force_eof;
global	int	open_parens;
global	int	align_state;
global	int	scanner_status;

#define SKIPPING	1
#define DEFINING	2
#define MATCHING	3
#define ALIGNING	4
#define ABSORBING	5

global	ptr	null_list;
global	ptr	def_ref;
global	ptr	match_toks;
global	ptr	align_toks;
global	ptr	omit_template;

struct input {
  short	state_field;
  short	type_field;
  union {
    struct {
      sym	in_cs_field;
      ptr	start_field;
      ptr	loc_field;
      ptr	*param_field;
    } t;
    struct {
      ptr	in_open_field;
      byte	*buf_field;
      byte	*next_field;
      byte	*limit_field;
    } f;
  } obj_field;
};
typedef struct input input;

global	input	cur_input;
global	input	*input_stack;
global	input	*input_end;
global	input	*input_ptr;
global	input	*base_ptr;
global	input	*max_in_stack;

#define state		cur_input.state_field

#define MID_LINE	1
#define SKIP_BLANKS	(2 + MAX_CHAR_CODE)
#define NEW_LINE	(3 + MAX_CHAR_CODE + MAX_CHAR_CODE)

#define file_state	(state != TOKEN_LIST)

#define index		cur_input.type_field
#define in_file		cur_input.obj_field.f.in_open_field
#define buffer		cur_input.obj_field.f.buf_field
#define next		cur_input.obj_field.f.next_field
#define limit		cur_input.obj_field.f.limit_field

#define BUF_SIZE		4096

#define terminal_input	(index == 0)

struct infile {
  file	file_field;
  str	name_field;
  int	line_field;
};
typedef struct infile infile;

#define cur_file	((infile *) in_file)->file_field
#define file_name	((infile *) in_file)->name_field
#define file_line	((infile *) in_file)->line_field

global	str	name;
global	int	line;

global	infile	*file_stack;
global	infile	*file_end;
global	infile	*file_ptr;
global	infile	*max_file_stack;

#define TOKEN_LIST	0

#define token_type	cur_input.type_field
#define in_cs		((sym_t *)cur_input.obj_field.t.in_cs_field)
#define start		cur_input.obj_field.t.start_field
#define loc		cur_input.obj_field.t.loc_field
#define param_start	cur_input.obj_field.t.param_field

#define cs_name		text(in_cs)

#define PARAMETER		0
#define U_TEMPLATE		1
#define V_TEMPLATE		2
#define BACKED_UP		3
#define INSERTED		4
#define MACRO			5
#define OUTPUT_TEXT		6
#define EVERY_PAR_TEXT		7
#define EVERY_MATH_TEXT		8
#define EVERY_DISPLAY_TEXT	9
#define EVERY_HBOX_TEXT		10
#define EVERY_VBOX_TEXT		11
#define EVERY_JOB_TEXT		12
#define EVERY_CR_TEXT		13
#define MARK_TEXT		14
#define WRITE_TEXT		15

global	int	nparams;
global	ptr	*param_stack;
global	ptr	*param_end;
global	ptr	*param_ptr;
global	ptr	*max_param_stack;

void	push_input();
void	pop_input();
void	begin_token_list();
void	end_token_list();
void	begin_file_reading();
void	end_file_reading();
void	back_input();
void	back_error();
void	ins_error();
void	runaway();
void	show_context();
void	clear_for_error_prompt();

#define new_token	new_avail
#define free_token	free_avail

#define back_list(L)	begin_token_list(L, BACKED_UP)
#define ins_list(L)	begin_token_list(L, INSERTED)

void	_tok_init();
void	_tok_init_once();

/*
 * Common TeX:toklist.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

ptr	str_toks();
ptr	the_toks();
void	conv_toks();

#define NUMBER_CODE		0
#define ROMAN_NUMERAL_CODE	1
#define STRING_CODE		2
#define MEANING_CODE		3
#define FONT_NAME_CODE		4
#define JOB_NAME_CODE		5

void	scan_toks();
void	read_toks();
void	print_meaning();
void	token_show();
void	show_token_list();
void	flush_list();

#define store_new_token(T) \
{q = new_token(); token_link(p) = q; token(q) = T; p = q;}

#define token_ref_count(T)	token(T)
#define add_token_ref(T) incr(token_ref_count(T))
#define delete_token_ref(T) \
{if (token_ref_count(T) == 0) \
  flush_list(T); \
  else decr(token_ref_count(T));}

void	_toklist_init();
void	_toklist_init_once();
