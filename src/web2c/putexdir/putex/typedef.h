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
typedef char two_choices; /* used when there are two variants in a record */
typedef char four_choices; /* used when there are four variants in a record */
typedef unsigned char glue_ord; /* infinity to the 0, 1, 2, or 3 power */
typedef unsigned char group_code; /* |save_level| for a level boundary */
typedef unsigned int trie_pointer; /* an index into |trie| */
typedef unsigned int hyph_pointer; /* an index into the ordered hash table */
typedef short dvi_index; /* an index into the output buffer */
