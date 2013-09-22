/* Here are some simple routines used in the display of noads. */
static void print_fam_and_char(pointer p) /* prints family and character */
{
  print_esc("fam"); print_int(fam(p)); print_char(' ');
  print_ASCII(character(p)-min_quarterword);
}
