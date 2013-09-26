/* The |space_factor| or |prev_depth| settings are changed when a |set_aux|
command is sensed. Similarly, |prev_graf| is changed in the presence of
|set_prev_graf|, and |dead_cycles| or |insert_penalties| in the presence of
|set_page_int|. These definitions are always global.

When some dimension of a box register is changed, the change isn't exactly
global; but \TeX\ does not look at the \.{\\global} switch. */
void alter_aux(void)
{
  halfword c; /* |hmode| or |vmode| */
  if (cur_chr!=labs(mode)) {
    report_illegal_case();
  } else {
    c=cur_chr;
    scan_optionale_quals();
    if (c==vmode) {
      scan_normal_dimen(); prev_depth=cur_val;
    } else {
      scan_int();
      if (cur_val<0 || cur_val>32767) {
        print_err("Bad space factor");
        help1("I allow only values in the range 1..32767 here.");
        int_error(cur_val);
      } else space_factor = cur_val;
   }
  }
}
