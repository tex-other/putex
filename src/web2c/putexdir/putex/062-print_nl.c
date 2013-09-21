/* The procedure |print_nl| is like |print|, but it makes sure that the
string appears at the beginning of a new line. */
void print_nl(str_rnumber s) /* prints string |s| at beginning of line */
{
  if ((term_offset>0 && (selector&1)) ||
      (file_offset>0 && selector>=log_only)) print_ln();
  print(s);
}
