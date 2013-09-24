void terminput(void) /* gets a line from the terminal */
{
  unsigned short k; /* index into |buffer| */
  fflush(termout);
  if (!input_ln(&term_in, true))
    fatal_error(S(302));
  term_offset=0;
  decr(selector);
  if (last!=first) {
    for (k=first;k<last;k++) print(buffer[k]);
  }
  print_ln();
  incr(selector); /* restore previous status */
}
