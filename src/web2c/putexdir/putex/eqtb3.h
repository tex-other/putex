#define line_skip_code 0 /* interline glue if |baseline_skip| is infeasible */
#define baseline_skip_code 1 /* desired glue between baselines */
#define par_skip_code 2 /* extra glue just above a paragraph */
#define above_display_skip_code 3 /* extra glue just above displayed math */
#define below_display_skip_code 4 /* extra glue just below displayed math */
#define above_display_short_skip_code 5  /* glue above displayed math following short lines */
#define below_display_short_skip_code 6  /* glue below displayed math following short lines */
#define left_skip_code 7 /* glue at left of justified lines */
#define right_skip_code 8 /* glue at right of justified lines */
#define top_skip_code 9 /* glue at top of main pages */
#define split_top_skip_code 10 /* glue at top of split pages */
#define tab_skip_code 11 /* glue between aligned entries */
#define space_skip_code 12 /* glue between words (if not |zero_glue|) */
#define xspace_skip_code 13 /* glue after sentences (if not |zero_glue|) */
#define par_fill_skip_code 14 /* glue on last line of paragraph */
#define thin_mu_skip_code 15 /* thin space in math formula */
#define med_mu_skip_code 16 /* medium space in math formula */
#define thick_mu_skip_code 17 /* thick space in math formula */
#define glue_pars 18 /* total number of glue parameters */
#define skip_base glue_base+glue_pars /* table of 256 ``skip'' registers */
#define mu_skip_base skip_base+256 /* table of 256 ``muskip'' registers */
#define local_base mu_skip_base+256 /* beginning of region 4 */
