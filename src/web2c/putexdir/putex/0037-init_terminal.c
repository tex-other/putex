/* The following program does the required initialization
without retrieving a possible command line.
It should be clear how to modify this routine to deal with command lines,
if the system permits them. */
boolean init_terminal(void) /* gets the terminal input started */
{
  t_open_in();
  if (initinc(1)) {
    loc=first;
    return true;
  }
  while (true) {
    fprintf(term_out, "**");
    fflush(term_out);
    if (!input_ln(&term+in, true)) {
      putc('\n', term_out);
      fprintf(term_out,"! End of file on the terminal... why?");
      return false;
    }
    loc=first;
    while ((loc<last) & (buffer[loc]==' '))
      loc++;
    if (loc<last) {
      return true;
    }
    fprintf(termout, "Please type the name of your input file.\n");
  }
}
