#define active_base 1 /* beginning of region 1, for active character equivalents */
#define single_base active_base+256 /* equivalents of one-character control sequences */
#define null_cs single_base+256 /* equivalent of \.{\\csname\\endcsname} */
#define hash_base null_cs+1 /* beginning of region 2, for the hash table */
#define frozen_control_sequence hash_base+hash_size /* for error recovery */
#define frozen_protection frozen_control_sequence /* inaccessible but definable */
#define frozen_cr frozen_control_sequence+1 /* permanent `\.{\\cr}' */
#define frozen_end_group frozen_control_sequence+2 /* permanent `\.{\\endgroup}' */
#define frozen_right frozen_control_sequence+3 /* permanent `\.{\\right}' */
#define frozen_fi frozen_control_sequence+4 /* permanent `\.{\\fi}' */
#define frozen_end_template frozen_control_sequence+5 /* permanent `\.{\\endtemplate}' */
#define frozen_endv frozen_control_sequence+6 /* second permanent `\.{\\endtemplate}' */
#define frozen_relax frozen_control_sequence+7 /* permanent `\.{\\relax}' */
#define end_write frozen_control_sequence+8 /* permanent `\.{\\endwrite}' */
#define frozen_dont_expand frozen_control_sequence+9 /* permanent `\.{\\notexpanded:}' */
#define frozen_null_font frozen_control_sequence+10 /* permanent `\.{\\nullfont}' */
#define font_id_base frozen_null_font-font_base /* begins table of 257 permanent font identifiers */
#define undefined_control_sequence frozen_null_font+257 /* dummy location */
#define glue_base undefined_control_sequence+1 /* beginning of region 3 */
