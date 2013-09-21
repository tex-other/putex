Static boolean initterminal(void)
{
  topenin();
  if (initinc(1)) {
    loc = first;
    return true;
  }
  while (true) {
    fprintf(termout, "**");
    fflush(termout);
    if (!inputln(&termin, true)) {
      putc('\n', termout);
      fprintf(termout,"! End of file on the terminal... why?");
      return false;
    }
    loc = first;
    while ((loc < last) & (buffer[loc] == ' '))
      loc++;
    if (loc < last) {
      return true;
    }
    fprintf(termout, "Please type the name of your input file.\n");
  }
}
