/* Our next goal is to write the |scan_int| procedure, which scans anything that
\TeX\ treats as an integer. But first we might as well look at some simple
applications of |scan_int| that have already been made inside of
|scan_something_internal|. */
void scan_eight_bit_int(void)
{
  scan_int();
  if (cur_val<0 || cur_val>255) {
    print_err("Bad register code");
    help2("A register number must be between 0 and 255.",
      "I changed this one to zero."); int_error(cur_val); cur_val=0;
  }
}

