#define par_indent_code 0 /* indentation of paragraphs */
#define math_surround_code 1 /* space around math in text */
#define line_skip_limit_code 2 /* threshold for |line_skip| instead of |baseline_skip| */
#define hsize_code 3 /* line width in horizontal mode */
#define vsize_code 4 /* page height in vertical mode */
#define max_depth_code 5 /* maximum depth of boxes on main pages */
#define split_max_depth_code 6 /* maximum depth of boxes on split pages */
#define box_max_depth_code 7 /* maximum depth of explicit vboxes */
#define hfuzz_code 8 /* tolerance for overfull hbox messages */
#define vfuzz_code 9 /* tolerance for overfull vbox messages */
#define delimiter_shortfall_code 10 /* maximum amount uncovered by variable delimiters */
#define null_delimiter_space_code 11 /* blank space in null delimiters */
#define script_space_code 12 /* extra space after subscript or superscript */
#define pre_display_size_code 13 /* length of text preceding a display */
#define display_width_code 14 /* length of line for displayed equation */
#define display_indent_code 15 /* indentation of line for displayed equation */
#define overfull_rule_code 16 /* width of rule that identifies overfull hboxes */
#define hang_indent_code 17 /* amount of hanging indentation */
#define h_offset_code 18 /* amount of horizontal offset when shipping pages out */
#define v_offset_code 19 /* amount of vertical offset when shipping pages out */
#define emergency_stretch_code 20 /* reduces badnesses on final pass of line-breaking */
#define dimen_pars 21 /* total number of dimension parameters */

#define scaled_base dimen_base+dimen_pars /* table of 256 user-defined \.{\\dimen} registers */
#define eqtb_size scaled_base+255 /* largest subscript of |eqtb| */
