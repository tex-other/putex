/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef MSDOS
  /* allocate iniTeX (550 k) trie_c, trie_o, trie_l, trie_r, trie_hash, trie_taken */
  #define ALLOCATEINI
  /* allocate main memory for TeX (2 Meg) zmem = zzzaa */
  #define ALLOCATEMAIN
  /* NOT *//* allow increasing high area of main memory */ /* FLUSH */
  #undef ALLOCATEHIGH  
  /* NOT *//* allow increasing low area of main memory */ /* FLUSH */
  #undef ALLOCATELOW  
  /* allocate font_info (800 k) (dynamically now) */
  #define ALLOCATEFONT 
  /* allocate hyphenation trie stuff (270 k) trie_trl, trie_tro, trie_trc */
  #define ALLOCATETRIES
  /* allocate hyphenation exception tables */
  #define ALLOCATEHYPHEN 
  /* allow trie_size to be variable */
  #define VARIABLETRIESIZE
  /* allocate strings and string pointers (184 k)str_pool & str_start */
  #define ALLOCATESTRING 
  /* NOT */ /* allocate hash table (zzzae) (78k) */
  #undef ALLOCATEHASH
  /* NOT */ /* allocate dvi_buf (16k) */ /* changed in 1.3 1996/Jan/18 */
  /* #define ALLOCATEDVIBUF */
  #undef ALLOCATEDVIBUF 
  /* experiment to dynamically deal with save_stack 99/Jan/4 */
  #define ALLOCATESAVESTACK
  /* experiment to dynamically deal with input_stack 99/Jan/21 */
  #define ALLOCATEINPUTSTACK
  /* experiment to dynamically deal with neststack 99/Jan/21 */
  #define ALLOCATENESTSTACK
  /* experiment to dynamically deal with param_stack 99/Jan/21 */
  #define ALLOCATEPARAMSTACK
  /* experiment to dynamically deal with input buffer 99/Jan/22 */
  #define ALLOCATEBUFFER
  /* increase fixed allocations */
  #define INCREASEFIXED
  /* increase number of fonts - quarterword 16 bit - max_quarterword limit */
  /* there may still be some bugs with this however ... also may slow down */
  /* also: should split use of quarterword for (i) font from (ii) char */
  /* for example, xeq_level ? hyphenation trie_trc ? */
  #define INCREASEFONTS 
  /* NOT NOT *//* allocate eqtb (108k) */ /* changed in 1.3 1996/Jan/18 */
  #undef ALLOCATEZEQTB
  /* make font_info array fmemoryword == 32 bit instead of memory_word = 64 bit */
  #define SHORTFONTINFO
  /* make hash table htwohalves == 32 bit instead of twohalves == 64 bit */
  // #define SHORTHASH  --- changed 2000/Feb/22 increase max_strings from 64K to 512M
  #undef SHORTHASH
  /* increase trie_op_size from 751 to 3001 96/Oct/12 */
  #define INCREASETRIEOP
#endif

/* With old PharLap linker it was important to avoid large fixed allocation */
/* Now may be better to use fixed arrays rather than allocate them */
/* hence ALLOCATEZQTB, ALLOCATEDVIBUF and ALLOCATEMINOR are NOT defined */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

#undef  TRIP
#undef  TRAP
#define STAT
#undef  DEBUG
#include "texmf.h"

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

#define MAXLINE 256         // for log_line buffer

/* #define max_halfword 65535L  */ /* for 32 bit memory word */
/* #define max_halfword 262143L */ /* for 36 bit memory word */
#define min_halfword -2147483647L  /* for 64 bit memory word (signed) */
#define max_halfword  2147483647L   /* for 64 bit memory word (signed) */

#define block_size 1000 /* block_size for variable length node alloc */

/* min_quarterword assumed 0 -- i.e. we use unsigned types for quarterword */
#define min_quarterword 0
#ifdef INCREASEFONTS
  #define max_quarterword 65535L
#else
  #define max_quarterword 255
#endif

/* #define default_mem_top 262140L */ /* usual big TeX allocation 2 Meg bytes */
/* #define default_mem_top 131070L */ /* usual big TeX allocation 1 Meg bytes */
#define default_mem_top 65534L      /* usual small TeX allocation 0.5 Meg   */

/* mem_bot smallest index in mem array dumped by iniTeX mem_top >= mem_min */
#define mem_bot 0
/* mem_top largest index in mem array dumped by iniTeX mem_top <= mem_max */
#ifdef ALLOCATEMAIN
  EXTERN integer mem_top;
  #define max_mem_size (max_halfword / sizeof(memory_word) -1)
#else
  #define mem_top 262140L 
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* mem_max == mem_top in iniTeX, otherwise mem_max >= mem_top */
/* if ALLOCATEMAIN is true, then mem_max is a variable */
/* otherwise it is a pre-processor defined constant */
#ifdef ALLOCATEMAIN
  EXTERN integer mem_max;
#else
  /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  /* #define mem_max 262140L */
  #define mem_max mem_top
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* if ALLOCATEMAIN is true, then mem_min is a variable */
/* otherwise it is a pre-processor defined constant */
/* mem_min == mem_bot in iniTeX, otherwise mem_min <= mem_bot */
#ifdef ALLOCATEMAIN
  EXTERN integer mem_min;
#else
  /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  #define mem_min 0
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

#define pool_name TEXPOOLNAME

/* #define mem_top 262140L */

/* type definitions follow */

typedef unsigned char ASCII_code; 
typedef unsigned char eight_bits; 
typedef integer pool_pointer; 
typedef integer str_number; 
typedef unsigned char packed_ASCII_code; 
typedef integer scaled; 
typedef integer nonnegative_integer; 
typedef char small_number; 

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* buf_size is max size of input line and max size of csname */
/* make sure its multiple of four bytes long */
/* want to increase this so it can handle whole paragraph imported from WP */
#ifdef INCREASEFIXED
/* #define buf_size 8192 */
/* #define buf_size 12000 */    /* 1996/Jan/17 */
/* #define buf_size 16384 */      /* 1998/June/30 */
/* #define buf_size 20000 */      /* 1999/Jan/7 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* #define buf_size 3000  */
#endif

#ifdef ALLOCATEBUFFER
  #define initial_buf_size   1000
  #define increment_buf_size 2000
  #define buf_size           2000000L /* arbitrary limit <= max_halfword */
  EXTERN ASCII_code *        buffer;
#else
  #define buf_size           20000 /* 1999/Jan/7 */
  EXTERN ASCII_code          buffer[buf_size + 4]; /* padded out to ...  + 4 */
#endif
EXTERN integer first; 
EXTERN integer last; 
EXTERN integer max_buf_stack; 

#define error_line      79
#define half_error_line 50
#define max_print_line  79

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFIXED
/* maximum number of simultaneous input sources (?) */
/* #define stack_size 600 */    /* expanded again 1999/Jan/21 */
/* #define stack_size 800 */
/* maximum number of input files and insertions that can be going on */
/* #define max_in_open 15 */    /* for Y&Y TeX 1.2 */
/* #define max_in_open 31 */    /* 1996/Jan/17 */
/* #define max_in_open 63 */    /* 1996/Jan/18 */
  #define max_in_open 127     /* 1996/Jan/20 - really ? */
/* save_size space for saving values outside of current group */
/* #define save_size 6000 */
/* #define save_size 8000 */      /* 1999/Jan/6 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* #define stack_size 300 */    /* Unix C version default */
  #define max_in_open 15
/* #define save_size 1000 */    /* 3.14159 C version */
/* #define save_size 4000 */    /* 3.14159 C version */
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* maximum internal font number - cannot be greated than max_quarter_word ! */
#ifdef INCREASEFONTS
/* #define font_max 511 */
  #define font_max 1023     /* 1996/Jan/17 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  #define font_max 255 
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* free the limit on font memory ! */ /* (2^32 - 1) / sizeof(memory_word) */
#ifdef ALLOCATEFONT
/* #define font_mem_size 262140L */
  #define font_mem_size (max_halfword / sizeof(memory_word) -1)
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  #define font_mem_size 100000L
#endif

/* our real font_mem_size is 268435456 --- ridiculously large, of course */

/* #define non_address font_mem_size */   /* 3.141 */
#define non_address 0           /* 3.14159 96/Jan/16 */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

/* below is new dynamic allocation - bkph 93/Nov/28 */  /* enough for lplain */
#ifdef ALLOCATEFONT
  #define initial_font_mem_size   20000
  #define increment_font_mem_size 40000
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* param_size maximum number of simultaneous macro parameters */
/* nest_size  maximum number of semantic levels simultaneously active */
#ifdef INCREASEFIXED
/* #define param_size 60 */     /* 3.14159 C version */
/* #define param_size 120 */
/* #define param_size 200 */      /* 1994/Oct/11 */
/* #define param_size 300 */      /* 1995/May/15 */
/* #define param_size 500 */      /* 1997/Jan/17 */
/* #define nest_size 40 */      /* 3.14159 C version */
/* #define nest_size 80 */
/* #define nest_size 100  */      /* 1994/Oct/11 */
/* #define nest_size 120 */     /* 1995/May/15 */
/* #define nest_size 200 */     /* 1999/Jan/7 */
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* #define param_size 60 */     /* Unix C default */
/* #define nest_size 40 */      /* Unix C default */
/* #define nest_size 100 */     /* Unix C default */
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* max_strings max number of strings */ /* (2^32 - 1) / sizeof (integer) */
#ifdef ALLOCATESTRING
/* #define max_strings 262140L */
// #define max_strings (max_halfword / sizeof(integer) -1)
  #define max_strings (max_halfword / sizeof(pool_pointer) -1)
/* #define pool_size 4032000L */
  #define pool_size (max_halfword - 1)
/* #define stringmargin 32768L */
  #define stringmargin 10000
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* #define max_strings 15000 */
#define max_strings 16384
#define pool_size 124000L 
#endif
#define stringvacancies 100000L 

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* #if defined (ALLOCATEINITRIE) && defined (ALLOCATEHYPHEN) */
#ifdef VARIABLETRIESIZE
  EXTERN integer trie_size;
  #define default_trie_size 60000
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#else
  #define trie_size 30000     /* 3.14159 C version */
#endif

/* increase trieop to accomadate more hyphenation patterns 96/OCt/12 */

#ifdef INCREASETRIEOP
  #define trie_op_size      3001
  #define neg_trie_op_size -3001
  #define min_trie_op       0
  #define max_trie_op       1000
#else
  #define trie_op_size      751
  #define neg_trie_op_size  -751
  #define min_trie_op       0
  #define max_trie_op       500
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* dvi_buf_size must be multiple of 8 - half is written out at a time */
#ifdef ALLOCATEDVIBUF
  #define default_dvi_buf_size 16384 
/* #define default_dvi_buf_size 32768 */    /* ? */
  EXTERN int dvi_buf_size;
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  #define dvi_buf_size 16384      /* 3.14159 C version */
/* #define dvi_buf_size 32768 */        /* ? */
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* WARNING: increasing hash table for cs names is not trivial */
/* size of hash table for control sequence name  < (mem_max - mem_min) / 10 */
/* #define hash_size 9500 */
/* #define hash_size 25000 */   /* 96/Jan/10 */
#define hash_size 32768       /* 96/Jan/17 */
/* trick to try and get around eqtb_extra problem */
/* #define hash_extra -256 */
#define hash_extra (255 - font_max) 
/* hash prime about 85% of hash_size (+ hash_extra) */
/* #define hashprime 7919  */
/* #define hash_prime 21247 */    /* 96/Jan/10 */
#define hash_prime 27197      /* 96/Jan/17 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

/* CONSTRAINT: reconcile increase font use by stealing from hash table ... */

/* Probably require eqtb_extra to be zero, so hash_extra = 255 - font_max */
#if (hash_extra != 255 - font_max)
  #error ERROR: hash_extra not equal to (255 - font_max)
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

/* some sanity check when using narrow hash table ... */

/* using SHORTHASH limits hash_size to be less than 65536 */
/* using SHORTHASH limits max_strings to be less than 65536 */
/* if you ever need more string pointers, then #undef SHORTHASH --- */
/* you'll use more memory (about 130k byte) and format file larger (8k) */

#ifdef SHORTHASH
/* can only do this if INCREASEFONTS defined up above ... */
#if (max_quarterword < 65535L)
#error ERROR: max_quarterword < 65535L
#endif
/* with narrowed hash table can only have 65535 string pointers */
/* #if (max_strings > max_quarterword) */ /* this test does not work */
#undef max_strings
#define max_strings max_quarterword
/* #endif */
/* with narrowed hash table can only have 65535 hash table entries */
#if (hash_size > max_quarterword)
#undef hash_size
#define hash_size max_quarterword
#endif
#endif /* end of if SHORTHASH */

/* NOTE: if you define/undefine SHORTFONT have to redo formats */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef INCREASEFONTS
  typedef unsigned short quarterword;
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  typedef unsigned char quarterword;
#endif

/* possible alternative ? */ /* min_halfword = 0 and double max_halfword ? */
/* typedef unsigned long halfword; NO NO: since mem_min may be < 0 */
typedef integer halfword;
typedef char twochoices;
typedef char fourchoices;

#include "texmfmem.h"

typedef char glue_ord; 

typedef struct {
/*  short mode_field;  */
  int mode_field; 
  halfword head_field, tail_field; 
  integer pg_field, ml_field; 
  memory_word aux_field; 
} list_state_record; 

typedef char group_code; 

typedef struct {
  quarterword state_field, index_field; 
  halfword start_field, loc_field, limit_field, name_field;
} in_state_record; 

typedef integer internal_font_number; 
typedef integer font_index; 
typedef integer dvi_index; 
typedef integer trie_op_code; 
typedef integer trie_pointer; 

/* typedef short hyph_pointer; */   /* increased 1996/Oct/20 ??? */
typedef integer hyph_pointer; 

EXTERN integer bad; 
EXTERN ASCII_code xord[256]; 
EXTERN ASCII_code xchr[256]; 
/* EXTERN char name_of_file[PATHMAX + 1]; */
// EXTERN char name_of_file[PATHMAX + 4]; /* padded out 512 + 4 */
EXTERN unsigned char name_of_file[PATHMAX + 4]; // fix 2000 June 18 
EXTERN integer name_length; 

/* EXTERN ASCII_code buffer[buf_size + 1]; */
/* EXTERN ASCII_code buffer[buf_size + 4]; */ /* padded out to ...  + 4 */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
  #define initial_pool_size     40000
  #define increment_pool_size   80000
  EXTERN packed_ASCII_code *    str_pool;
  #define initial_max_strings   5000
  #define increment_max_strings 10000
  EXTERN pool_pointer *         str_start;
#else
  EXTERN packed_ASCII_code      str_pool[pool_size + 1]; 
  EXTERN pool_pointer           str_start[max_strings + 1]; 
#endif
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

EXTERN pool_pointer pool_ptr;
EXTERN str_number   str_ptr;
EXTERN pool_pointer init_pool_ptr;
EXTERN str_number   init_str_ptr;

#ifdef INITEX
  EXTERN alpha_file pool_file; 
#endif /* INITEX */

EXTERN alpha_file log_file; 
/* EXTERN char selector; */
/* EXTERN integer selector; */ /* padded out */
EXTERN int selector;      /* padded out */ /* 95/Jan/7 */
/* EXTERN char dig[23]; */
EXTERN char dig[23 + 1];  /* padded out */
EXTERN integer tally; 
EXTERN integer term_offset; 
EXTERN integer file_offset; 
EXTERN ASCII_code trick_buf[error_line + 1]; /* already padded 79 + 1 */
EXTERN integer trick_count; 
EXTERN integer first_count; 
/* EXTERN char interaction;  */
/* EXTERN integer interaction; */ /* padded out */
EXTERN int interaction; /* padded out */      /* 95/Jan/7 */
EXTERN bool deletions_allowed; 
EXTERN bool set_box_allowed; 
/* EXTERN char history; */
/* EXTERN integer history; */ /* padded out */
EXTERN int history; /* padded out */        /* 95/Jan/7 */
/* EXTERN schar error_count;  */
/* EXTERN integer error_count; */ /* padded out */
EXTERN int error_count; /* padded out */      /* 95/Jan/7 */
/* EXTERN str_number help_line[6]; */
EXTERN char * help_line[6];
/* EXTERN char help_ptr; */
/* EXTERN integer help_ptr; */ /* padded out */
EXTERN int help_ptr; /* padded out */       /* 95/Jan/7 */
EXTERN bool use_err_help; 
/* EXTERN integer interrupt;  */
EXTERN volatile integer interrupt;  /* bkph - do avoid compiler optimization */
EXTERN bool OK_to_interrupt; 
EXTERN bool arith_error; 
EXTERN scaled tex_remainder; 
EXTERN halfword temp_ptr; 

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEMAIN
  EXTERN memory_word * mainmemory;   /* remembered so can be free() later */
  EXTERN memory_word * zzzaa;
  #define zmem zzzaa
#else
  /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  EXTERN memory_word 
  /* #define zmem (zzzaa - (int)(mem_min)) */
  /*  zzzaa[mem_max - mem_min + 1]; */
  #define zmem (zzzaa - (int)(mem_bot))
  zzzaa[mem_max - mem_bot + 1];
#endif

EXTERN halfword lo_mem_max;
EXTERN halfword hi_mem_min;
EXTERN integer var_used, dyn_used;
EXTERN halfword avail; 
EXTERN halfword mem_end; 
EXTERN halfword mem_start;      /* new in 3.14159 ??? */
EXTERN halfword rover; 

/* NOTE: the following really also need to be dynamically allocated */
#ifdef DEBUG
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEMAIN
  EXTERN char * zzzab;
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* EXTERN bool  */    /* save (4 - 1) * mem_max - mem_min */
EXTERN char
/* #define freearr (zzzab - (int)(mem_min)) */
/*  zzzab[mem_max - mem_min + 1];  */
#define freearr (zzzab - (int)(mem_bot))
  zzzab[mem_max - mem_bot + 1]; 
#endif

#ifdef ALLOCATEMAIN
  EXTERN char *zzzac;
#else
/* EXTERN bool */   /* save (4 - 1) * mem_max - mem_min */
EXTERN char
/* #define wasfree (zzzac - (int)(mem_min)) */
#define wasfree (zzzac - (int)(mem_bot))
/*  zzzac[mem_max - mem_min + 1];  */
  zzzac[mem_max - mem_bot + 1]; 
#endif

EXTERN halfword was_mem_end, was_lo_max, was_hi_min; 
EXTERN bool panicking; 
#endif /* DEBUG */

EXTERN integer font_in_short_display; 
EXTERN integer depth_threshold; 
EXTERN integer breadth_max; 
/* EXTERN list_state_record nest[nest_size + 1];  */
/* EXTERN short shown_mode; */
/* EXTERN integer shown_mode; */ /* padded out */
EXTERN int shown_mode; /* padded out */   /* 95/Jan/7 */
/* EXTERN char old_setting; */
/* EXTERN integer old_setting; */ /* padded out */
EXTERN int old_setting; /* padded out */    /* 95/Jan/7 */

/* eqtn_extra is no longer used --- it should be 0 96/Jan/10 */
#ifdef INCREASEFONTS
  #define eqtb_extra (font_max - 255 + hash_extra) 
#else
  #define eqtb_extra 0
#endif

/* Probably require eqtb_extra to be zero, so hash_extra = 255 - font_max */
#if (eqtb_extra != 0)
  #error ERROR: eqtb_extra is not zero (need hash_extra equal 255 - font_max)
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEZEQTB
EXTERN memory_word * zeqtb; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#else
#ifdef INCREASEFONTS
/* EXTERN memory_word zeqtb[13507 + eqtb_extra];  */
EXTERN memory_word eqtb[(hash_size + 4007) + eqtb_extra]; 
#else
/* EXTERN memory_word zeqtb[13507];  */ /* hash_size =  9500 */
/* EXTERN memory_word zeqtb[29007];  */ /* hash_size = 25000 */
EXTERN memory_word zeqtb[(hash_size + 4007)]; 
#endif
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* EXTERN quarterword 
#define xeq_level (zzzad -12663)    hash_size =  9500
#define xeq_level (zzzad -28163)    hash_size = 25000
  zzzad[844]; */

#ifdef INCREASEFONTS
/* #define xeq_level (zzzad - (12663 + eqtb_extra))  */
#define xeq_level (zzzad - ((hash_size + 3163) + eqtb_extra))  
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#else
/* #define xeq_level (zzzad -12663) */
#define xeq_level (zzzad - (hash_size + 3163)) 
#endif
/* zzzad[844]; */
EXTERN quarterword zzzad[844]; /* ??? attempt to fix 99/Jan/5 */
/* region 5 & 6 int_base to eqtb_size: 13507 - 12663 */

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* EXTERN twohalves 
#define hash (zzzae - 514)
    zzzae[9767];  hash_size =  9500
    zzzae[25267]; hash_size = 25000 */

#ifdef ALLOCATEHASH
#ifdef SHORTHASH
EXTERN htwohalves *zzzae;
#else
EXTERN twohalves *zzzae;
#endif
#define hash (zzzae - 514)
#else /* not allocating hash table */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef SHORTHASH
EXTERN htwohalves 
#else
EXTERN twohalves 
#endif
#define hash (zzzae - 514)
/*  zzzae[9767];  */
#ifdef INCREASEFONTS
  zzzae[hash_size + 267 + eqtb_extra]; 
#else
  zzzae[hash_size + 267]; 
#endif
#endif

EXTERN halfword hash_used; 
EXTERN bool no_new_control_sequence; 
EXTERN integer cs_count; 

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* using allocated save stack slows it down 1% to 2% */
/* despite reallocation, we still limit it to something finite */
/* to avoid soaking up all of machine memory in case of infinite loop */
#ifdef ALLOCATESAVESTACK
  #define save_size 65536     /* arbitrary - ridiculously large */
  #define initial_save_size 1000
  #define increment_save_size 2000
  EXTERN memory_word *save_stack; 
#else
  #define save_size 8000        /* 1999/Jan/6 enough for even CRC */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  EXTERN memory_word save_stack[save_size + 1]; 
#endif

EXTERN integer save_ptr; 
EXTERN integer max_save_stack; 

/* The following could really be char instead of quarterword ... */
/* EXTERN quarterword cur_level;  */
/* EXTERN integer cur_level; */ /* padded out */
EXTERN int cur_level; /* padded out */    /* 95/Jan/7 */

/* EXTERN group_code cur_group;  */
/* EXTERN integer cur_group;  */ /* padded out */
EXTERN int cur_group; /* padded out */      /* 95/Jan/7 */

EXTERN integer cur_boundary; 
EXTERN integer mag_set; 

/* EXTERN eight_bits cur_cmd;  */
/* EXTERN integer cur_cmd;  */ /* padded out */
EXTERN int cur_cmd; /* padded out */      /* 95/Jan/7 */

/* EXTERN halfword cur_chr;  */ /* itex, tex0, tex, tex3, tex5, tex6, tex7, tex8 */
EXTERN int cur_chr;             /* 95/Jan/7 */

EXTERN halfword cur_cs; 
EXTERN halfword cur_tok; 

#ifdef ALLOCATENESTSTACK
  #define nest_size 65536         /* arbitrary - ridiculously large */
  #define initial_nest_size 100
  #define increment_nest_size 200
  EXTERN list_state_record * nest; 
#else
  #define nest_size 200       /* 1999/Jan/7 */
  EXTERN list_state_record nest[nest_size + 1]; 
#endif
EXTERN integer nest_ptr; 
EXTERN integer max_nest_stack; 
EXTERN list_state_record cur_list; 

#ifdef ALLOCATEPARAMSTACK
  #define param_size 65536        /* arbitrary - ridiculously large */
  #define initial_param_size 100
  #define increment_param_size 200
  EXTERN halfword * param_stack; 
#else
#define param_size 500        /* 1997/Jan/17 */
EXTERN halfword param_stack[param_size + 1]; 
#endif
EXTERN integer param_ptr; 
EXTERN integer max_param_stack; 

#ifdef ALLOCATEINPUTSTACK
  #define stack_size 65536          /* arbitrary - ridiculously large */
  #define initial_stack_size 100
  #define increment_stack_size 200
  EXTERN in_state_record *input_stack; 
#else
  #define stack_size 800
  EXTERN in_state_record input_stack[stack_size + 1]; 
#endif
EXTERN integer input_ptr; 
EXTERN integer max_in_stack; 

EXTERN integer high_in_open;      /* 1997/Jan/17 */
EXTERN in_state_record cur_input; 

/* EXTERN integer in_open;  */      /* used in itex, tex2, texmf */
EXTERN int in_open;         /* 95/Jan/7 */

EXTERN integer open_parens;
EXTERN integer max_open_parens; 
EXTERN alpha_file input_file[max_in_open + 1]; 
EXTERN integer line; 
EXTERN integer line_stack[max_in_open + 1]; 

/* EXTERN char scanner_status;  */
/* EXTERN integer scanner_status;  */ /* padded out */
EXTERN int scanner_status; /* padded out */ /* 95/Jan/7 */

EXTERN halfword warning_index; 
EXTERN halfword def_ref; 

EXTERN integer align_state; 
EXTERN integer base_ptr; 
EXTERN halfword par_loc; 
EXTERN halfword par_token; 
EXTERN bool force_eof; 
/* EXTERN halfword cur_mark[5];  */
EXTERN halfword cur_mark[6]; 

/* EXTERN char long_state; */
/* EXTERN integer long_state; */ /* padded out */
EXTERN int long_state; /* padded out */ /* 95/Jan/7 */

/* EXTERN halfword pstack[9];  */
EXTERN halfword pstack[10]; 

/* EXTERN integer cur_val; */
EXTERN int cur_val;           /* 95/Jan/7 */

/* EXTERN char cur_val_level;  */
/* EXTERN integer cur_val_level; */ /* padded out */
EXTERN int cur_val_level; /* padded out */ /* 95/Jan/7 */

/* EXTERN small_number radix;  */
/* EXTERN integer radix;  */ /* padded out */
EXTERN int radix; /* padded out */    /* 95/Jan/7 */

/* EXTERN glue_ord cur_order;  */
/* EXTERN integer cur_order;  */ /* padded out */
EXTERN int cur_order; /* padded out */    /* 95/Jan/7 */

EXTERN alpha_file read_file[16];  /* hard wired limit in TeX */
/* EXTERN char read_open[17];  */
EXTERN char read_open[20]; /* padded out */

EXTERN halfword cond_ptr; 

/* EXTERN char if_limit; */
/* EXTERN integer if_limit; */ /* padded out */
EXTERN int if_limit; /* padded out */   /* 95/Jan/7 */

/* EXTERN small_number cur_if; */
/* EXTERN integer cur_if; */ /* padded out */
EXTERN int cur_if; /* padded out */   /* 95/Jan/7 */

EXTERN integer if_line; 
EXTERN integer skip_line; 
EXTERN str_number cur_name; 
EXTERN str_number cur_area; 
EXTERN str_number cur_ext; 
EXTERN pool_pointer area_delimiter; 
EXTERN pool_pointer ext_delimiter; 
EXTERN integer format_default_length; 
EXTERN ccharpointer TEX_format_default; 
EXTERN bool name_in_progress; 
EXTERN bool log_opened; 
EXTERN bool quoted_file_name;
EXTERN str_number job_name; 
EXTERN str_number output_file_name;   // DVI file
EXTERN str_number texmf_log_name;   // LOG file
EXTERN byte_file dvi_file; 
EXTERN byte_file tfm_file;
EXTERN char * dvi_file_name;
EXTERN char * log_file_name;

/*******************************************************************/

/* SHORTFONTINFO halves the memory word used to store font info */
/* if it is not defined we use ordinary TeX memory words */

#ifdef SHORTFONTINFO
/* keep definition of fmemoryword in texmfmem.h */
/* keep definition of ffourquarters in texfmem.h */
/* keep definition of fquarterword in texfmem.h */
#else
/* go back to original definition as TeX memory word */
#undef fmemoryword
#define fmemoryword memory_word
/* go back to original definition as fourquarters of a TeX word */
#undef ffourquarters
#define ffourquarters fourquarters
/* go back to original definition as quaterword */
#undef fquarterword
#define fquarterword quarterword
#endif

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEFONT
  EXTERN fmemoryword * font_info; 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#else
/* EXTERN memory_word font_info[font_mem_size + 1];  */
  EXTERN fmemoryword font_info[font_mem_size + 1]; 
#endif

EXTERN font_index fmem_ptr; 
EXTERN internal_font_number font_ptr;
EXTERN internal_font_number frozenfontptr;        /* 99/Mar/26 */
/* There are about 24 integer size items per font, or about 100 bytes */
EXTERN ffourquarters font_check[font_max + 1]; 

EXTERN scaled font_size[font_max + 1]; 
EXTERN scaled font_dsize[font_max + 1]; 
EXTERN font_index font_params[font_max + 1];    /* in 3.14159 */
EXTERN str_number font_name[font_max + 1]; 
EXTERN str_number font_area[font_max + 1]; 
EXTERN eight_bits font_bc[font_max + 1]; /* already padded 511 + 1 = 512 */
EXTERN eight_bits font_ec[font_max + 1]; /* already padded 511 + 1 = 512 */
EXTERN halfword font_glue[font_max + 1]; 
/* use char instead of bool to save space, but is it worth slow down ? */
/* EXTERN char font_used[font_max + 1];  */
EXTERN bool font_used[font_max + 1]; 

/* might want to make some of following only one character wide also ? */
/* except some use -1 as special case value */
/* well, at least make them short instead of integer */
EXTERN integer hyphen_char[font_max + 1]; 
EXTERN integer skew_char[font_max + 1]; 
EXTERN font_index bchar_label[font_max + 1]; 
EXTERN short font_bchar[font_max + 1];  /* already padded 1023 + 1 = 1024 */
/* don't change above to int or format file will be incompatible */
EXTERN short font_false_bchar[font_max + 1];  /* already padded 1023 + 1 = 1024 */
/* don't change above to int or format file will be incompatible */
EXTERN integer char_base[font_max + 1]; 
EXTERN integer width_base[font_max + 1]; 
EXTERN integer height_base[font_max + 1]; 
EXTERN integer depth_base[font_max + 1]; 
EXTERN integer italic_base[font_max + 1]; 
EXTERN integer lig_kern_base[font_max + 1]; 
EXTERN integer kern_base[font_max + 1]; 
EXTERN integer exten_base[font_max + 1]; 
EXTERN integer param_base[font_max + 1]; 

EXTERN ffourquarters null_character; 

EXTERN integer total_pages; 
EXTERN scaled max_v; 
EXTERN scaled max_h; 
EXTERN integer max_push; 
EXTERN integer last_bop; 
EXTERN integer dead_cycles; 
EXTERN bool doing_leaders; 

/* EXTERN quarterword c, f;  */
/* EXTERN integer c, f */; /* padded out */
EXTERN int c, f; /* padded out */       /* 95/Jan/7 */

EXTERN scaled rule_ht, rule_dp, rule_wd; 
EXTERN halfword g; 
EXTERN integer lq, lr; 

#ifdef ALLOCATEDVIBUF
  EXTERN eight_bits *zdvibuf; 
#else
  /* EXTERN eight_bits dvi_buf[dvi_buf_size + 1]; */
  /* EXTERN eight_bits dvi_buf[dvi_buf_size + 4]; */ /* padded out  */
  EXTERN eight_bits zdvibuf[dvi_buf_size + 4];  /* padded out 1996/Jan/18 */
#endif

EXTERN dvi_index half_buf; 
EXTERN dvi_index dvi_limit; 
EXTERN dvi_index dvi_ptr; 
EXTERN integer dvi_offset; 
EXTERN integer dvi_gone; 
EXTERN halfword down_ptr, right_ptr; 
EXTERN scaled dvi_h, dvi_v; 
EXTERN scaled cur_h, cur_v; 
EXTERN internal_font_number dvi_f; 
EXTERN integer cur_s; 
EXTERN scaled totalstretch[4], totalshrink[4]; /* padded already */
EXTERN integer last_badness; 
EXTERN halfword adjust_tail; 
EXTERN integer pack_begin_line; 
EXTERN twohalves empty_field; 
EXTERN fourquarters null_delimiter; 
EXTERN halfword cur_mlist; 

/* EXTERN small_number cur_style; */
/* EXTERN integer cur_style; */  /* padded out */ /* tex5.c, tex7.c */
EXTERN int cur_style;  /* padded out */     /* 95/Jan/7 */

/* EXTERN small_number cur_size; */
/* EXTERN integer cur_size;  */ /* padded out */
EXTERN int cur_size;  /* padded out */        /* 95/Jan/7 */

EXTERN scaled cur_mu; 
EXTERN bool mlist_penalties; 
EXTERN internal_font_number cur_f; 

/* EXTERN quarterword cur_c; */
/* EXTERN integer cur_c; */  /* padded out */
EXTERN int cur_c;  /* padded out */     /* 95/Jan/7 */

EXTERN ffourquarters cur_i; 

EXTERN integer magic_offset; 
EXTERN halfword cur_align; 
EXTERN halfword cur_span; 
EXTERN halfword cur_loop; 
EXTERN halfword align_ptr; 
EXTERN halfword cur_head, cur_tail; 
EXTERN halfword just_box; 
EXTERN halfword passive; 
EXTERN halfword printed_node; 
EXTERN halfword pass_number; 
/* EXTERN scaled active_width[7]; */
EXTERN scaled active_width[8]; 
/* EXTERN scaled cur_active_width[7]; */
EXTERN scaled cur_active_width[8]; 
/* EXTERN scaled background[7]; */
EXTERN scaled background[8]; 
/* EXTERN scaled break_width[7]; */
EXTERN scaled break_width[8]; 
EXTERN bool noshrinkerroryet; 
EXTERN halfword cur_p; 
EXTERN bool second_pass; 
EXTERN bool final_pass; 
EXTERN integer threshold; 
EXTERN integer minimal_demerits[4]; 
EXTERN integer minimum_demerits; 
EXTERN halfword best_place[4]; 
EXTERN halfword best_pl_line[4]; 
EXTERN scaled disc_width; 
EXTERN halfword easyline; 
EXTERN halfword last_special_line; 
EXTERN scaled first_width; 
EXTERN scaled second_width; 
EXTERN scaled first_indent; 
EXTERN scaled second_indent; 
EXTERN halfword best_bet; 
EXTERN integer fewest_demerits; 
EXTERN halfword best_line; 
EXTERN integer actual_looseness; 
EXTERN integer line_diff; 
/* EXTERN short hc[64+2]; */  /* padded OK 66 * 2 = 132 which is divisible by 4 */
EXTERN int hc[66];  /* padded OK 66 * 2 = 132 which is divisible by 4 */

/* EXTERN small_number hn; */
/* EXTERN integer hn; */  /* padded out */
EXTERN int hn;  /* padded out */      /* 95/Jan/7 */

EXTERN halfword ha, hb; 

/* EXTERN internal_font_number hf;  */
EXTERN int hf;              /* 95/Jan/7 */

/* EXTERN short hu[64+2]; */    /* padded OK */
EXTERN int hu[66];    /* padded OK */ 

/* EXTERN integer hyf_char;  */
EXTERN int hyf_char;            /* 95/Jan/7 */

/* init_cur_lang new in 3.14159 */
/* EXTERN ASCII_code cur_lang, init_cur_lang; */
/* EXTERN integer cur_lang, init_cur_lang; */ /* padded out */
EXTERN int cur_lang, init_cur_lang; /* padded out */    /* 95/Jan/7 */

EXTERN integer lhyf, rhyf;
/* EXTERN init_l_hyf, init_r_hyf; */ /* new in 3.14159 */
EXTERN integer init_l_hyf, init_r_hyf;  /* new in 3.14159 */

EXTERN halfword hyfbchar; 
/* EXTERN char hyf[65];  */
EXTERN char hyf[68]; /* padded out */
EXTERN halfword init_list; 
EXTERN bool init_lig; 
EXTERN bool init_lft; 

/* EXTERN small_number hyphen_passed; */
/* EXTERN integer hyphen_passed; */  /* padded out */
EXTERN int hyphen_passed;  /* padded out */     /* 95/Jan/7 */

/* EXTERN halfword cur_l, cur_r; */   /* make int's ? 95/Jan/7 */
EXTERN int cur_l, cur_r;    /* make int's ? 95/Jan/7 */

EXTERN halfword cur_q; 
EXTERN halfword lig_stack; 
EXTERN bool ligature_present; 
EXTERN bool lft_hit, rt_hit; 

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* could perhaps use packed_ASCII_code for trie_trc ? */
#ifdef ALLOCATETRIES
  EXTERN halfword *trie_trl; 
  EXTERN halfword *trie_tro; 
  EXTERN quarterword *trie_trc; 
#else
  /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  EXTERN halfword trie_trl[trie_size + 1]; 
  EXTERN halfword trie_tro[trie_size + 1]; 
  EXTERN quarterword trie_trc[trie_size + 1]; 
#endif
EXTERN small_number hyf_distance[trie_op_size + 1]; /* already padded 751 + 1 */
EXTERN small_number hyf_num[trie_op_size + 1];    /* already padded 751 + 1 */
EXTERN trie_op_code hyf_next[trie_op_size + 1]; 
EXTERN integer op_start[256]; 

/* if ALLOCATEHYPHEN is true, then hyphen_prime is a variable */
/* otherwise it is a pre-processor defined constant */
#ifdef ALLOCATEHYPHEN
  #define default_hyphen_prime 1009
  EXTERN str_number * hyph_word;
  EXTERN halfword * hyph_list; 
  /* EXTERN hyphen_prime; */
  EXTERN integer hyphen_prime;
#else
  #define hyphen_prime 607
  /* EXTERN str_number hyph_word[608];  */
  EXTERN str_number hyph_word[hyphen_prime + 1]; 
  /* EXTERN halfword hyph_list[608];  */
  EXTERN halfword hyph_list[hyphen_prime + 1]; 
#endif

/* EXTERN hyph_pointer hyph_count;  */
/* EXTERN integer hyph_count; */  /* padded out */
EXTERN int hyph_count;  /* padded out */    /* 95/Jan/7 */

#ifdef INITEX
EXTERN integer trie_op_hash_C[trie_op_size - neg_trie_op_size + 1];
#define trie_op_hash (trie_op_hash_C - (int)(neg_trie_op_size)) 
EXTERN trie_op_code trie_used[256]; 
EXTERN ASCII_code trie_op_lang[trie_op_size + 1];   /* already padded 751 + 1 */
EXTERN trie_op_code trie_op_val[trie_op_size + 1]; 
EXTERN integer trie_op_ptr; 
#endif /* INITEX */

EXTERN trie_op_code max_op_used; 
EXTERN bool smallop; 

#ifdef INITEX
  #ifdef ALLOCATEINI
    EXTERN packed_ASCII_code *trie_c;  
    EXTERN trie_op_code *trie_o; 
    EXTERN trie_pointer *trie_l; 
    EXTERN trie_pointer *trie_r; 
    EXTERN trie_pointer *trie_hash; 
  #else /* end ALLOCATEINI */
    EXTERN packed_ASCII_code trie_c[trie_size + 1]; 
    EXTERN trie_op_code trie_o[trie_size + 1]; 
    EXTERN trie_pointer trie_l[trie_size + 1]; 
    EXTERN trie_pointer trie_r[trie_size + 1]; 
    EXTERN trie_pointer trie_hash[trie_size + 1]; 
  #endif /* end not ALLOCATEINI */
  EXTERN trie_pointer trie_ptr; 
#endif /* INITEX */

#ifdef INITEX
  #ifdef ALLOCATEINI
    /* EXTERN bool *trie_taken; */ /* save (4 - 1) * trie_size = 90,000 byte */
    EXTERN char *trie_taken; 
  #else
    EXTERN bool trie_taken[trie_size + 1]; 
  #endif

  EXTERN trie_pointer trie_min[256]; 
  EXTERN trie_pointer trie_max; 
  EXTERN bool trie_not_ready; 
#endif /* INITEX */
EXTERN scaled best_height_plus_depth; 
EXTERN halfword page_tail; 

/* EXTERN char page_contents; */
/* EXTERN integer page_contents; */ /* padded out */
EXTERN int page_contents; /* padded out */      /* 95/Jan/7 */

#define cs_token_flag 4095    /* if we should want to use this ... */

/* ********************************************************************* */

/* do *some* sanity checking here --- rather than in TeX later 96/Jan/18 */
/* (cannot catch everything here, since some is now dynamic) */

#if (half_error_line < 30) || (half_error_line > error_line - 15)
  #error ERROR: (half_error_line < 30) || (half_error_line > error_line - 15) BAD 1
#endif

#if (max_print_line < 60)
  #error ERROR: (max_print_line < 60) BAD 2
#endif

#if (hash_prime > hash_size)
  #error ERROR: (hash_prime > hash_size) BAD 5
#endif

#if (max_in_open > 127)
  #error ERROR: (max_in_open > 127) BAD 6
#endif

#if (min_quarterword > 0) || (max_quarterword < 127)
  #error ERROR: (min_quarterword > 0) || (max_quarterword < 127) BAD 11
#endif

#if (min_halfword > 0) || (max_halfword < 32767)
  #error ERROR:  (min_halfword > 0) || (max_halfword < 32767) BAD 12
#endif

#if (min_quarterword < min_halfword) || (max_quarterword > max_halfword)
  #error ERROR: (min_quarterword < min_halfword) || (max_quarterword > max_halfword) BAD 13
#endif

#if (font_max > max_quarterword)
  #error ERROR: (font_max > max_quarterword) BAD 15
#endif

#if (save_size > max_halfword)
  #error ERROR: (save_size > max_halfword) BAD 17
#endif

#if (buf_size > max_halfword)
  #error ERROR:  (buf_size > max_halfword) BAD 18
#endif

#if (max_quarterword - min_quarterword) < 255
  #error (max_quarterword - min_quarterword) < 255 BAD 19
#endif

/* ********************************************************************* */

EXTERN scaled page_max_depth; 
EXTERN halfword best_page_break; 
EXTERN integer least_page_cost; 
EXTERN scaled best_size; 
EXTERN scaled page_so_far[8]; 
EXTERN halfword last_glue; 
EXTERN integer last_penalty; 
EXTERN scaled last_kern; 
EXTERN integer insert_penalties; 
EXTERN bool output_active; 
EXTERN internal_font_number main_f; 

EXTERN ffourquarters main_i; 
EXTERN ffourquarters main_j; 

EXTERN font_index main_k; 
EXTERN halfword main_p; 
EXTERN integer main_s; 
EXTERN halfword bchar; 
EXTERN halfword false_bchar; 
EXTERN bool cancel_boundary; 
EXTERN bool ins_disc; 
EXTERN halfword cur_box; 
EXTERN halfword after_token; 
EXTERN bool long_help_seen; 
EXTERN str_number format_ident; 
EXTERN word_file fmt_file; 
EXTERN integer ready_already; 

EXTERN alpha_file write_file[16]; /* hard wired limit in TeX */
EXTERN bool write_open[18]; 

EXTERN halfword write_loc; 
EXTERN pool_pointer edit_name_start; 
/* EXTERN integer edit_name_length, edit_line, tfm_temp;  */
EXTERN integer edit_name_length, edit_line; 
/* EXTERN integer tfm_temp; */    /* only used in tex3.c */
EXTERN int tfm_temp;        /* only used in tex3.c 95/Jan/7 */

/* new stuff defined in local.c - bkph */

#ifdef MSDOS
  EXTERN bool is_initex;
  EXTERN bool verbose_flag;
  EXTERN bool trace_flag;
  EXTERN bool debug_flag;
  EXTERN bool heap_flag;
  EXTERN bool open_trace_flag;
  EXTERN bool cache_file_flag;
  EXTERN bool knuth_flag;
  EXTERN bool no_interrupts;
  EXTERN bool c_style_flag;
  EXTERN bool non_ascii;
  EXTERN bool key_replace;
  EXTERN bool deslash;
  EXTERN bool trimeof;
  EXTERN bool allow_patterns;
  EXTERN bool show_fonts_used;
  EXTERN bool reset_exceptions;
  EXTERN bool show_current;
  EXTERN bool current_flag;
  EXTERN bool current_tfm;
  EXTERN bool return_flag;
  EXTERN bool want_version;
  EXTERN bool civilize_flag;
  EXTERN bool show_numeric;
  EXTERN bool restrict_to_ascii;
  EXTERN bool show_missing;
  EXTERN bool full_file_name_flag;
  EXTERN bool save_strings_flag;
  EXTERN int mem_initex;
  EXTERN int mem_extra_high;
  EXTERN int mem_extra_low;
  EXTERN int new_hyphen_prime;
  EXTERN int missing_characters;
  EXTERN int show_in_hex;
  EXTERN int show_in_dos;
  EXTERN int test_dir_access;
  EXTERN int dir_method;
  EXTERN int file_method;
  /* EXTERN int waitflush; */
  EXTERN int show_fmt_flag;
  EXTERN int show_tfm_flag;
  EXTERN bool show_texinput_flag; /* 1998/Jan/28 */
  EXTERN bool truncate_long_lines;  /* 1998/Feb/2 */
  EXTERN bool show_cs_names;      /* 1998/Mar/31 */
  EXTERN int tab_step;
  EXTERN int pseudo_tilde;
  EXTERN int pseudo_space;
  EXTERN int allow_quoted_names;
  EXTERN int default_rule;
  EXTERN char *format_file;
  EXTERN char *source_direct;     /* 1998/Sep/29 */
  EXTERN char *string_file;
  EXTERN int share_flag;
  EXTERN char *format_name;
  EXTERN char *encoding_name;
  EXTERN bool format_specific;
  EXTERN bool encoding_specific;
  EXTERN bool show_line_break_stats;  /* 1996/Feb/9 */
  EXTERN int first_pass_count;      /* 1996/Feb/9 */
  EXTERN int second_pass_count;     /* 1996/Feb/9 */
  EXTERN int final_pass_count;      /* 1996/Feb/9 */
  EXTERN int underfull_hbox;      /* 1996/Feb/9 */
  EXTERN int overfull_hbox;     /* 1996/Feb/9 */
  EXTERN int underfull_vbox;      /* 1996/Feb/9 */
  EXTERN int overfull_vbox;     /* 1996/Feb/9 */
  EXTERN int paragraph_failed;      /* 1996/Feb/9 */
  EXTERN int singleline;        /* 1996/Feb/15 */
  EXTERN FILE *errout;
  EXTERN int font_dimen_zero;     /* 1998/Oct/5 */
  EXTERN int ignore_frozen;     /* 1998/Oct/5 */
  EXTERN bool suppress_f_ligs;    /* 1999/Jan/5 */
  EXTERN int abort_flag;      // not yet hooked up ???
  EXTERN int err_level;     // not yet hooked up ???
  EXTERN int jump_used;       /* 1999/Nov/28 */
  EXTERN jmp_buf jumpbuffer;      /* 1999/Nov/7 */
 #endif /* DOS */

#ifdef MSDOS
  extern int current_pool_size;       /* in local.c - bkph */
  extern int current_max_strings;     /* in local.c - bkph */
  extern int current_mem_size;        /* in local.c - bkph */
  extern int current_font_mem_size;     /* in local.c - bkph */
  extern int current_save_size;       /* in local.c - bkph */
  extern int current_stack_size;      /* in local.c - bkph */
  extern int current_nest_size;       /* in local.c - bkph */
  extern int current_param_size;      /* in local.c - bkph */
  extern int current_buf_size;        /* in local.c - bkph */
  extern char *tex_version;       /* in local.c - bkph */
  extern char *application;       /* in local.c - bkph */
  extern char *yandyversion;        /* in local.c - bkph */
  unsigned char wintodos[128];        /* in local.c - bkph */
  extern char last_filename[PATH_MAX];    /* in ourpaths.c */
  extern char log_line[MAXLINE];        /* in local.c */
  extern char *texpath;           /* in local.c */

  memory_word * allocate_main_memory (int);   /* in local.c - bkph */
  memory_word * realloc_main (int, int);      /* in local.c - bkph */
  packed_ASCII_code * realloc_str_pool (int);   /* in local.c - bkph */
  pool_pointer * realloc_str_start (int);     /* in local.c - bkph */
  memory_word * realloc_save_stack (int);     /* in local.c - bkph */
  list_state_record * realloc_nest_stack (int); /* in local.c - bkph */
  in_state_record * realloc_input_stack (int);    /* in local.c - bkph */
  halfword * realloc_param_stack (int);     /* in local.c - bkph */
  ASCII_code * realloc_buffer (int);        /* in local.c - bkph */
  fmemoryword * realloc_font_info (int);      /* in local.c - bkph */

  int realloc_hyphen (int);       /* in local.c - bkph */
  int allocate_tries (int);       /* in local.c - bkph */

void try_and_open (char *);       /* in local.c - bkph */
void check_eqtb (char *);       /* in local.c - bkph */
void probe_memory (void);       /* in local.c - bkph */
// void show_maximums (FILE *);     /* in local.c - bkph */
void print_cs_names (FILE *, int);    /* in local.c - bkph */
void perrormod(char *);         /* in local.c */

char *grabenv(char *);      /* in local.c - bkph */
// void showversion (FILE *);     /* in local.c - bkph */
void stamp_it (char *);         /* in local.c - bkph */
void stampcopy (char *);        /* in local.c - bkph */
bool prime (int);         /* in local.c - bkph */
int endit (int);            /* in local.c - bkph */

void uexit (int unix_code);     /* in lib/uexit.c - bkph */
void t_open_in (void);          /* in lib/texmf.c - bkph */

bool extensionirrelevantp (unsigned char *base, int nlen, char *suffix);

void call_edit (ASCII_code *filename, pool_pointer fnstart,
         integer fnlength, integer linenumber); /* from lib/texmf.c - bkph */

void add_variable_space(int);       /* in itex.c - bkph */

void get_date_and_time (integer *minutes, integer *day,
            integer *month, integer *year);   /* in lib/texmf.c - bkph */

bool maketextex (void);       /* in openinou.c */
bool maketextfm (void);       /* in openinou.c */

char *unixify (char *);       /* in pathsrch.c bkph */

#ifdef _WINDOWS
  void show_line (char *, int);     /* in local.c */
  void show_char (int);         /* in local.c */
  int main(int, char *[]);        /* in lib\texmf.c */
#endif

#ifdef CHECKPOOL
  int checkpool (char *);         /* in itex.c - debugging */
#endif

#endif /* ifdef MSDOS */

/****************************************************************************/

#include "coerce.h"

/****************************************************************************/
/* sec 0054 */
#define no_print     16
#define term_only    17
#define log_only     18
#define term_and_log 19
#define pseudo       20
#define new_string   21
#define max_selector 21
/* sec 0073 */
#define batch_mode      0
#define nonstop_mode    1
#define scroll_mode     2
#define error_stop_mode 3
/* sec 0076 */
#define spotless             0
#define warning_issued       1
#define error_message_issued 2
#define fatal_error_stop     3
/* sec 0105 */
#define nx_plux_y(...)   mult_and_add(..., 07777777777L)
#define mult_integers(a) mult_and_add(a,0,07777777777L)
/* sec 0108 */
#define inf_bad 10000L
/* sec 0109 */
#define set_glue_ratio_zero(a) (a) = 0.0
#define set_glue_ratio_one(a)  (a) = 1.0
#define tex_float(a)           (a)
#define unfloat(a)             (a)
#define float_constant(a)      (float) (a)
/* sec 0115*/
#define pointer halfword
#define null    min_halfword
/* sec 0118 */
#define link(p) mem[(p)].hh.v.RH
#define info(p) mem[(p)].hh.v.LH
/* sec 0124 */
#define empty_flag  max_halfword
#define is_empty(a) (link(a) = empty_flag)
#define node_size   info
#define llink(a)    info(a+1)
#define rlink(a)    link(a+1)
/* sec 0133 */
#define type(a)    mem[a].hh.b0
#define subtype(a) mem[a].hh.b1
/* sec 0134 */
#define is_char_node(a) (a >= hi_mem_min)
#define font            type
#define character       subtype
/* sec 0135 */
#define hlist_node      0
#define box_node_size   7
#define width_offset    1
#define depth_offset    2
#define height_offset   3
#define width(a)        mem[a + width_offset].sc
#define depth(a)        mem[a + depth_offset].sc
#define height(a)       mem[a + height_offset].sc
#define shift_amount(a) mem[a + 4].sc
#define list_offset     5
#define glue_order(a)   subtype(a + list_offset)
#define glue_sign(a)    type(a + list_offset)
#define normal          0
#define stretching      1
#define shrinking       2
#define glue_offset     6
#define glue_set(a)     mem[a + glue_offset].gr
/* sec 0137 */
#define vlist_node 1
/* sec 0138 */
#define rule_node      2
#define rule_node_size 4
#define null_flag      -010000000000L
#define is_running(a)  (a = null_flag)
/* sec 0140 */
#define ins_node         3
#define ins_node_size    5
#define float_cost(a)    mem[a + 1].cint
#define ins_ptr(a)       info(a + 4)
#define split_top_ptr(a) link(a + 4)
/* sec 0162 */
#define contrib_head  mem_top - 1
#define page_head     mem_top - 2
#define temp_head     mem_top - 3
#define hold_head     mem_top - 4
#define adjust_head   mem_top - 5
#define active        mem_top - 7
#define align_head    mem_top - 8
#define end_span      mem_top - 9
#define omit_template mem_top - 10
#define null_list     mem_top - 11
#define lig_trick     mem_top - 12
/* sec 0207 */
#define escape 0
#define relax 0
#define left_brace 1
#define right_brace 2
#define math_shift 3
#define tab_mark 4
#define car_ret 5
#define out_param 5
#define mac_param 6
#define sup_mark 7
#define sub_mark 8
#define ignore 9
#define endv 9
#define spacer 10
#define letter 11
#define other_char 12
#define active_char 13
#define par_end 13
#define match 13
#define comment 14
#define end_match 14
#define stop 14
#define invalid_char 15
#define delim_num 15
#define max_char_code 15
/* sec 0208 */
#define char_num 16
#define math_char_num 17
#define mark 18
#define xray 19
#define make_box 20
#define hmove 21
#define vmove 22
#define un_hbox 23
#define un_vbox 24
#define remove_item 25
#define hskip 26
#define vskip 27
#define mskip 28
#define kern 29
#define mkern 30
#define leader_ship 31
#define halign 32
#define valign 33
#define no_align 34
#define vrule 35
#define hrule 36
#define insert 37
#define vadjust 38
#define ignore_spaces 39
#define after_assignment 40
#define after_group 41
#define break_penalty 42
#define start_par 43
#define ital_corr 44
#define accent 45
#define math_accent 46
#define discretionary 47
#define eq_no 48
#define left_right 49
#define math_comp 50
#define limit_switch 51
#define above 52
#define math_style 53
#define math_choice 54
#define non_script 55
#define vcenter 56
#define case_shift 57
#define message 58
#define extension 59
#define in_stream 60
#define begin_group 61
#define end_group 62
#define omit 63
#define ex_space 64
#define no_boundary 65
#define radical 66
#define end_cs_name 67
#define min_internal 68
#define char_given 68
#define math_given 69
#define last_item 70
#define max_non_prefixed_command 70
/* sec 0209 */
#define toks_register 71
#define assign_toks 72
#define assign_int 73
#define assign_dimen 74
#define assign_glue 75
#define assign_mu_glue 76
#define assign_font_dimen 77
#define assign_font_int 78
#define set_aux 79
#define set_prev_graf 80
#define set_page_dimen 81
#define set_page_int 82
#define set_box_dimen 83
#define set_shape 84
#define def_code 85
#define def_family 86
#define set_font 87
#define def_font 88
#define tex_register 89
#define max_internal 89
#define advance 90
#define multiply 91
#define divide 92
#define prefix 93
#define let 94
#define shorthand_def 95
#define read_to_cs 96
#define def 97
#define set_box 98
#define hyph_data 99
#define set_interaction 100
#define max_command 100
/* sec 0210 */
#define undefined_cs max_command + 1
#define expand_after max_command + 2
#define no_expand max_command + 3
#define input max_command + 4
#define if_test max_command + 5
#define fi_or_else max_command + 6
#define cs_name max_command + 7
#define convert max_command + 8
#define the max_command + 9
#define top_bot_mark max_command + 10
#define call max_command + 11
#define long_call max_command + 12
#define outer_call max_command + 13
#define long_outer_call max_command + 14
#define end_template max_command + 15
#define dont_expand max_command + 16
#define glue_ref max_command + 17
#define shape_ref max_command + 18
#define box_ref max_command + 19 
#define data max_command + 20
/* sec 0212 */
#define ignore_depth -65536000L
/* sec 0213 */
#define mode            cur_list.mode_field
#define head            cur_list.head_field
#define tail            cur_list.tail_field
#define aux             cur_list.aux_field
#define prev_depth      aux.sc
#define space_factor    aux.hh.v.LH
#define clang           aux.hh.v.RH
#define incompleat_noad aux.cint
#define prev_graf       cur_list.pg_field
#define mode_line       cur_list.ml_field
/* sec 0221 */
#define level_zero min_quarterword
#define level_one  level_zero + 1
/* sec 0222 */
#define active_base                   1
#define single_base                   active_base + 256
#define null_cs                       single_base + 256
#define hash_base                     null_cs + 1
#define frozen_control_sequence       hash_base + hash_size
#define frozen_protection             frozen_control_sequence
#define frozen_cr                     frozen_control_sequence + 1
#define frozen_end_group              frozen_control_sequence + 2
#define frozen_right                  frozen_control_sequence + 3
#define frozen_fi                     frozen_control_sequence + 4
#define frozen_end_template           frozen_control_sequence + 5
#define frozen_endv                   frozen_control_sequence + 6
#define frozen_relax                  frozen_control_sequence + 7
#define end_write                     frozen_control_sequence + 8
#define frozen_dont_expand            frozen_control_sequence + 9
#define frozen_null_font              frozen_control_sequence + 10
#define font_id_base                  frozen_null_font - font_base
#define undefined_control_sequence    frozen_null_font + 257
#define glue_base                     undefined_control_sequence + 1
/* sec 0224 */
#define line_skip_code                0
#define baseline_skip_code            1
#define par_skip_code                 2
#define above_display_skip_code       3
#define below_display_skip_code       4
#define above_display_short_skip_code 5
#define below_display_short_skip_code 6
#define left_skip_code                7
#define right_skip_code               8
#define top_skip_code                 9
#define split_top_skip_code           10
#define tab_skip_code                 11
#define space_skip_code               12
#define xspace_skip_code              13
#define par_fill_skip_code            14
#define thin_mu_skip_code             15
#define med_mu_skip_code              16
#define thick_mu_skip_code            17
#define glue_pars                     18
#define skip_base                     glue_base + glue_pars
#define mu_skip_base                  skip_base + 256
#define local_base                    mu_skip_base + 256
// #
#define skip(a)                       equiv(skip_base + a)
#define mu_skip(a)                    equiv(mu_skip_base + a)
#define glue_par(a)                   equiv(glue_base + a)
#define line_skip                     glue_par(line_skip_code)
#define baseline_skip                 glue_par(baseline_skip_code)
#define par_skip                      glue_par(par_skip_code)
#define above_display_skip            glue_par(above_display_skip_code)
#define below_display_skip            glue_par(below_display_skip_code)
#define above_display_short_skip      glue_par(above_display_short_skip_code)
#define below_display_short_skip      glue_par(below_display_short_skip_code)
#define left_skip                     glue_par(left_skip_code)
#define right_skip                    glue_par(right_skip_code)
#define top_skip                      glue_par(top_skip_code)
#define split_top_skip                glue_par(split_top_skip_code)
#define tab_skip                      glue_par(tab_skip_code)
#define space_skip                    glue_par(space_skip_code)
#define xspace_skip                   glue_par(xspace_skip_code)
#define par_fill_skip                 glue_par(par_fill_skip_code)
#define thin_mu_skip                  glue_par(thin_mu_skip_code)
#define med_mu_skip                   glue_par(med_mu_skip_code)
#define thick_mu_skip                 glue_par(thick_mu_skip_code)
/* sec 0230 */
#define par_shape_loc                 local_base
#define output_routine_loc            local_base + 1
#define every_par_loc                 local_base + 2
#define every_math_loc                local_base + 3
#define every_display_loc             local_base + 4
#define every_hbox_loc                local_base + 5
#define every_vbox_loc                local_base + 6
#define every_job_loc                 local_base + 7
#define every_cr_loc                  local_base + 8
#define err_help_loc                  local_base + 9
#define toks_base                     local_base + 10
#define box_base                      toks_base + 256
#define cur_font_loc                  box_base + 256
#define math_font_base                cur_font_loc + 1
#define cat_code_base                 math_font_base + 48
#define lc_code_base                  cat_code_base + 256
#define uc_code_base                  lc_code_base + 256
#define sf_code_base                  uc_code_base + 256
#define math_code_base                sf_code_base + 256
#define int_base                      math_code_base + 256
// #
#define par_shape_ptr                 equiv(par_shape_loc)
#define output_routine                equiv(output_routine_loc)
#define every_par                     equiv(every_par_loc)
#define every_math                    equiv(every_math_loc)
#define every_display                 equiv(every_display_loc)
#define every_hbox                    equiv(every_hbox_loc)
#define every_vbox                    equiv(every_vbox_loc)
#define every_job                     equiv(every_job_loc)
#define every_cr                      equiv(every_cr_loc)
#define err_help                      equiv(err_help_loc)
#define toks(a)                       equiv(toks_base + a)
#define box(a)                        equiv(box_base + a)
#define cur_font                      equiv(cur_font_loc)
#define fam_fnt(a)                    equiv(math_font_base + a)
#define cat_code(a)                   equiv(cat_code_base + a)
#define lc_code(a)                    equiv(lc_code_base + a)
#define uc_code(a)                    equiv(uc_code_base +a)
#define sf_code(a)                    equiv(sf_code_bas + a)
#define math_code(a)                  equiv(math_code_base + a)
/* sec 0236 */
#define pretolerance_code             0  // 3163
#define tolerance_code                1  // 3164
#define line_penalty_code             2  // 3165
#define hyphen_penalty_code           3  // 3166
#define ex_hyphen_penalty_code        4  // 3167
#define club_penalty_code             5  // 3168
#define widow_penalty_code            6  // 3169
#define display_widow_penalty_code    7  // 3170
#define broken_penalty_code           8  // 3171
#define bin_op_penalty_code           9  // 3172
#define rel_penalty_code              10 // 3173
#define pre_display_penalty_code      11 // 3174
#define post_display_penalty_code     12 // 3175
#define inter_line_penalty_code       13 // 3176
#define double_hyphen_demerits_code   14 // 3177
#define final_hyphen_demerits_code    15 // 3178
#define adj_demerits_code             16 // 3179
#define mag_code                      17 // 3180
#define delimiter_factor_code         18 // 3181
#define looseness_code                19 // 3182
#define time_code                     20 // 3183
#define day_code                      21 // 3184
#define month_code                    22 // 3185
#define year_code                     23 // 3186
#define show_box_breadth_code         24 // 3187
#define show_box_depth_code           25 // 3188
#define hbadness_code                 26 // 3189
#define vbadness_code                 27 // 3190
#define pausing_code                  28 // 3191
#define tracing_online_code           29 // 3192
#define tracing_macros_code           30 // 3193
#define tracing_stats_code            31 // 3194
#define tracing_paragraphs_code       32 // 3195
#define tracing_pages_code            33 // 3196
#define tracing_output_code           34 // 3197
#define tracing_lost_chars_code       35 // 3198
#define tracing_commands_code         36 // 3199 
#define tracing_restores_code         37 // 3200
#define uc_hyph_code                  38 // 3201
#define output_penalty_code           39 // 3202 NOT USED.
#define max_dead_cycles_code          40 // 3203
#define hang_after_code               41 // 3204
#define floating_penalty_code         42 // 3205
#define global_defs_code              43 // 3206
#define cur_fam_code                  44 // 3207
#define escape_char_code              45 // 3208
#define default_hyphen_char_code      46 // 3209
#define default_skew_char_code        47 // 3210
#define end_line_char_code            48 // 3211
#define new_line_char_code            49 // 3212
#define language_code                 50 // 3213
#define left_hyphen_min_code          51 // 3214
#define right_hyphen_min_code         52 // 3215
#define holding_inserts_code          53 // 3216
#define error_context_lines_code      54 // 3217
#define int_pars                      55
#define count_base                    int_base + int_pars // 3218
#define del_code_base                 count_base + 256    // 
#define dimen_base                    del_code_base + 256 // 3730
// #
#define del_code(a)                   eqtb[del_code_base + a].cint
#define count(a)                      eqtb[count_base + a].cint
#define int_par(a)                    eqtb[int_base + a].cint
#define pretolerance                  int_par(pretolerance_code)
#define tolerance                     int_par(tolerance_code)
#define line_penalty                  int_par(line_penalty_code)
#define hyphen_penalty                int_par(hyphen_penalty_code)
#define ex_hyphen_penalty             int_par(ex_hyphen_penalty_code)
#define club_penalty                  int_par(club_penalty_code)
#define widow_penalty                 int_par(widow_penalty_code)
#define display_widow_penalty         int_par(display_widow_penalty_code)
#define broken_penalty                int_par(broken_penalty_code)
#define bin_op_penalty                int_par(bin_op_penalty_code)
#define rel_penalty                   int_par(rel_penalty_code)
#define pre_display_penalty           int_par(pre_display_penalty_code)
#define post_display_penalty          int_par(post_display_penalty_code)
#define inter_line_penalty            int_par(inter_line_penalty_code)
#define double_hyphen_demerits        int_par(double_hyphen_demerits_code)
#define final_hyphen_demerits         int_par(final_hyphen_demerits_code)
#define adj_demerits                  int_par(adj_demerits_code)
#define mag                           int_par(mag_code)
#define delimiter_factor              int_par(delimiter_factor_code)
#define looseness                     int_par(looseness_code)
#define tex_time                      int_par(time_code)
#define day                           int_par(day_code)
#define month                         int_par(month_code)
#define year                          int_par(year_code)
#define show_box_breadth              int_par(show_box_breadth_code)
#define show_box_depth                int_par(show_box_depth_code)
#define hbadness                      int_par(hbadness_code)
#define vbadness                      int_par(vbadness_code)
#define pausing                       int_par(pausing_code)
#define tracing_online                int_par(tracing_online_code)
#define tracing_macros                int_par(tracing_macros_code)
#define tracing_stats                 int_par(tracing_stats_code)
#define tracing_paragraphs            int_par(tracing_paragraphs_code)
#define tracing_pages                 int_par(tracing_pages_code)
#define tracing_output                int_par(tracing_output_code)
#define tracing_lost_chars            int_par(tracing_lost_chars_code)
#define tracing_commands              int_par(tracing_commands_code)
#define tracing_restores              int_par(tracing_restores_code)
#define uc_hyph                       int_par(uc_hyph_code)
#define output_penalty                int_par(output_penalty_code)
#define max_dead_cycles               int_par(max_dead_cycles_code)
#define hang_after                    int_par(hang_after_code)
#define floating_penalty              int_par(floating_penalty_code)
#define global_defs                   int_par(global_defs_code)
#define cur_fam                       int_par(cur_fam_code)
#define escape_char                   int_par(escape_char_code)
#define default_hyphen_char           int_par(default_hyphen_char_code)
#define default_skew_char             int_par(default_skew_char_code)
#define end_line_char                 int_par(end_line_char_code)
#define new_line_char                 int_par(new_line_char_code)
#define language                      int_par(language_code)
#define left_hyphen_min               int_par(left_hyphen_min_code)
#define right_hyphen_min              int_par(right_hyphen_min_code)
#define holding_inserts               int_par(holding_inserts_code)
#define error_context_lines           int_par(error_context_lines_code)
/* sec 0247 */
#define par_indent_code               0  // 3730
#define math_surround_code            1  // 3731
#define line_skip_limit_code          2  // 3732
#define hsize_code                    3  // 3733
#define vsize_code                    4  // 3734
#define max_depth_code                5  // 3735
#define split_max_depth_code          6  // 3736
#define box_max_depth_code            7  // 3737
#define hfuzz_code                    8  // 3738
#define vfuzz_code                    9  // 3739
#define delimiter_shortfall_code      10 // 3740
#define null_delimiter_space_code     11 // 3741
#define script_space_code             12 // 3742
#define pre_display_size_code         13 // 3743
#define display_width_code            14 // 3744
#define display_indent_code           15 // 3745
#define overfull_rule_code            16 // 3746
#define hang_indent_code              17 // 3747
#define h_offset_code                 18 // 3748
#define v_offset_code                 19 // 3749
#define emergency_stretch_code        20 // 3750
#define dimen_pars                    21
#define scaled_base                   dimen_base + dimen_pars
#define eqtb_size                     scaled_base + 255
// #
#define dimen(a)                      eqtb[scaled_base + a].cint
#define dimen_par(a)                  eqtb[dimen_base + a].cint
#define par_indent                    dimen_par(par_indent_code)
#define math_surround                 dimen_par(math_surround_code)
#define line_skip_limit               dimen_par(line_skip_limit_code)
#define hsize                         dimen_par(hsize_code)
#define vsize                         dimen_par(vsize_code)
#define max_depth                     dimen_par(max_depth_code)
#define split_max_depth               dimen_par(split_max_depth_code)
#define box_max_depth                 dimen_par(box_max_depth_code)
#define hfuzz                         dimen_par(hfuzz_code)
#define vfuzz                         dimen_par(vfuzz_code)
#define delimiter_shortfall           dimen_par(delimiter_shortfall_code)
#define null_delimiter_space          dimen_par(null_delimiter_space_code)
#define script_space                  dimen_par(script_space_code)
#define pre_display_size              dimen_par(pre_display_size_code)
#define display_width                 dimen_par(display_width_code)
#define display_indent                dimen_par(display_indent_code)
#define overfull_rule                 dimen_par(overfull_rule_code)
#define hang_indent                   dimen_par(hang_indent_code)
#define h_offset                      dimen_par(h_offset_code)
#define v_offset                      dimen_par(v_offset_code)
#define emergency_stretch             dimen_par(emergency_stretch_code)
/* sec 79 */

extern void tex_help (unsigned int n, ...);
extern void append_char(ASCII_code c);
#define help0()     tex_help(0)
#define help1(...)  tex_help(1, __VA_ARGS__)
#define help2(...)  tex_help(2, __VA_ARGS__)
#define help3(...)  tex_help(3, __VA_ARGS__)
#define help4(...)  tex_help(4, __VA_ARGS__)
#define help5(...)  tex_help(5, __VA_ARGS__)
#define help6(...)  tex_help(6, __VA_ARGS__)
/********BINDING WITH LIBHARU*********/
/*
EXTERN HPDF_Doc  yandy_pdf;
EXTERN HPDF_Page yandy_page;
EXTERN HPDF_Font yandy_font;
*/
/********BINDING WITH LIBHARU*********/