typedef unsigned char uchar;
typedef signed char schar;
typedef unsigned char boolean;
typedef uchar Char;
#define true 1
#define false 0
#define Static static
#define qi(A) (A+min_quarterword) /* to put an |eight_bits| item into a quarterword */
#define qo(A) (A-min_quarterword) /* to take an |eight_bits| item out of a quarterword */
#define hi(A) (A+min_halfword) /* to put a sixteen-bit item into a halfword */
#define ho(A) (A-min_halfword) /* to take a sixteen-bit item from a halfword */
typedef unsigned char ASCII_code; /* eight-bit numbers: 0..255 */
typedef unsigned long KANJI_code; /* codes for KANJI */
typedef unsigned char eight_bits; /* unsigned one-byte quantity */
typedef unsigned short sixteen_bits; /* unsigned two-bytes quantity */
typedef unsigned char packed_ASCII_code; /* elements of str pool array: 0..255 */
typedef long scaled; /* this type is used for scaled integers */
typedef unsigned long non_negative_integer; /* 0..'17777777777 */
typedef unsigned char small_number; /* this type is self-explanatory: 0..63 */
typedef double glue_ratio; /* one-word representation of a glue expansion factor */
typedef unsigned short quarterword; /* 1/4 of a word */
typedef int halfword; /* 1/2 of a word */
typedef int pointer; /* a flag or a location in |mem| or |eqtb| */
typedef char two_choices; /* used when there are two variants in a record */
typedef char four_choices; /* used when there are four variants in a record */
typedef unsigned char glue_ord; /* infinity to the 0, 1, 2, or 3 power */
typedef unsigned char group_code; /* |save_level| for a level boundary */
typedef unsigned int trie_pointer; /* an index into |trie| */
typedef unsigned int hyph_pointer; /* an index into the ordered hash table */
typedef short dvi_index; /* an index into the output buffer */

typedef struct two_halves { 
  halfword rh;
  union {
    halfword lh;
    struct {
      quarterword b0, b1;
    } U2;
  } UU;
} two_halves;

typedef struct four_quarters {
  quarterword b0, b1, b2, b3;
} four_quarters;

typedef union memory_word {
  long int_;
  glueratio gr;
  twohalves hh;
  fourquarters qqqq;
  long sc;
} memory_word;

#define mem_max 30000 /* greatest index in \TeX's internal |mem| array;
  must be strictly less than |max_halfword|;
  must be equal to |mem_top| in \.{INITEX}, otherwise |> mem_top| */
#define mem_min 0 /* smallest index in \TeX's internal |mem| array;
  must be |min_halfword| or more;
  must be equal to |mem_bot| in \.{INITEX}, otherwise |< mem_bot| */
#define buf_size 500 /* maximum number of characters simultaneously present in
  current lines of open files and in control sequences between
  \.{\\csname} and \.{\\endcsname}; must not exceed |max_halfword| */
#define error_line 72 /* width of context lines on terminal error messages */
#define half_error_line 42 /* width of first lines of contexts in terminal
  error messages; should be between 30 and |error_line-15| */
#define max_print_line 79 /* width of longest text lines output; should be at least 60 */
#define stack_size 200 /* maximum number of simultaneous input sources */
#define max_in_open 6 /* maximum number of input files and error insertions that
  can be going on simultaneously */
#define font_max 75 /* maximum internal font number; must not exceed |max_quarterword|
  and must be at most |font_base+256| */
#define font_mem_size 20000 /* number of words of |font_info| for all fonts */
#define param_size 60 /* maximum number of simultaneous macro parameters */
#define nest_size 40 /* maximum number of semantic levels simultaneously active */
#define save_size 600 /* space for saving values outside of current group; must be
  at most |max_halfword| */
#define trie_size 8000 /* space for hyphenation patterns; should be larger for
  \.{INITEX} than it is in production versions of \TeX */
#define trie_op_size 500 /* space for ``opcodes'' in the hyphenation patterns */
#define dvi_buf_size 800 /* size of the output buffer; must be a multiple of 8 */
#define file_name_size 40 /* file names shouldn't be longer than this */

#define page_ins_head (mem_top) /* list of insertion data for current page */
#define contrib_head (mem_top-1) /* vlist of items not yet on current page */
#define page_head (mem_top-2) /* vlist for current page */
#define temp_head (mem_top-3) /* head of a temporary list of some kind */
#define hold_head (mem_top-4) /* head of a temporary list of another kind */
#define adjust_head (mem_top-5) /* head of adjustment list returned by |hpack| */
#define active (mem_top-7) /* head of active list in |line_break|, needs two words */
#define align_head (mem_top-8) /* head of preamble list for alignments */
#define end_span (mem_top-9) /* tail of spanned-width lists */
#define omit_template (mem_top-10) /* a constant token list */
#define null_list (mem_top-11) /* permanently empty list */
#define lig_trick (mem_top-12) /* a ligature masquerading as a |char_node| */
#define garbage (mem_top-12) /* used for scrap information */
#define backup_head (mem_top-13) /* head of token list built by |scan_keyword| */
#define hi_mem_stat_min (mem_top-13) /* smallest statically allocated word in
  the one-word |mem| */
#define hi_mem_stat_usage 14 /* the number of one-word nodes always present */

#define max_dimen 1073741823L /* $2^{30}-1$ */
#define null_flag (-1073741824L) /* $-2^{30}$, signifies a missing item */
#define infinity 2147483647L /* the largest positive value that \TeX\ knows */
#define defaultcode 1073741824L /* denotes |default_rule_thickness| */
#define awful_bad 1073741823L /* more than a billion demerits */
#define box_flag 1073741824L /* context code for `\.{\\setbox0}' */
#define ignore_depth (-65536000L) /* |prev_depth| value that is ignored */

#define unity 65536L /* $2^{16}$, represents 1.00000 */
#define two 131072L /* $2^{17}$, represents 2.00000 */

#define first_text_char 0 /* ordinal number of the smallest element of |text_char| */
#define last_text_char 255 /* ordinal number of the largest element of |text_char| */

#define batch_mode 0 /* omits all stops and omits terminal output */
#define nonstop_mode 1 /* omits all stops */
#define scroll_mode 2 /* omits error stops */
#define error_stop_mode 3 /* stops at every opportunity to interact */

#define hlist_node 0 /* |type| of hlist nodes */
#define box_node_size 7 /* number of words to allocate for a box node */
#define width_offset 1 /* position of |width| field in a box node */
#define depth_offset 2 /* position of |depth| field in a box node */
#define height_offset 3 /* position of |height| field in a box node */

#define list_offset 5 /* position of |list_ptr| field in a box node */
#define normal 0 /* the most common case when several cases are named */
#define stretching 1 /* glue setting applies to the stretch components */
#define shrinking 2 /* glue setting applies to the shrink components */
#define glue_offset 6 /* position of |glue_set| in a box node */

#define vlist_node 1 /* |type| of vlist nodes */

#define rule_node 2 /* |type| of rule nodes */
#define rule_node_size 4 /* number of words to allocate for a rule node */

#define ins_node 3 /* |type| of insertion nodes */
#define ins_node_size 5 /* number of words to allocate for an insertion */

#define mark_node 4 /* |type| of a mark node */
#define small_node_size 2 /* number of words to allocate for most node types */

#define adjust_node 5 /* |type| of an adjust node */

#define ligature_node 6 /* |type| of a ligature node */

#define disc_node 7 /* |type| of a discretionary node */

#define whatsit_node 8 /* |type| of special extension nodes */

#define math_node 9 /* |type| of a math node */
#define before 0 /* |subtype| for math node that introduces a formula */
#define after 1 /* |subtype| for math node that winds up a formula */

#define glue_node 10 /* |type| of node that points to a glue specification */
#define cond_math_glue 98 /* special |subtype| to suppress glue in the next node */
#define mu_glue 99 /* |subtype| for math glue */
#define a_leaders 100 /* |subtype| for aligned leaders */
#define c_leaders 101 /* |subtype| for centered leaders */
#define x_leaders 102 /* |subtype| for expanded leaders */

#define glue_spec_size 4 /* number of words to allocate for a glue specification */
#define fil 1 /* first-order infinity */
#define fill 2 /* second-order infinity */
#define filll 3 /* third-order infinity */

#define kern_node 11 /* |type| of a kern node */
#define explicit 1 /* |subtype| of kern nodes from \./* \\kern} and \./* \\/} */
#define acc_kern 2 /* |subtype| of kern nodes from accents */

#define penalty_node 12 /* |type| of a penalty node */
#define inf_penalty inf_bad /* ``infinite'' penalty value */
#define eject_penalty -inf_penalty /* ``negatively infinite'' penalty value */

#define unset_node 13 /* |type| for an unset node */

