/* If the user has set the |pausing| parameter to some positive value,
and if nonstop mode has not been selected, each line of input is displayed
on the terminal and the transcript file, followed by `\.{=>}'.
\TeX\ waits for a response. If the response is simply |carriage_return|, the
line is accepted as it stands, otherwise the line typed is
used instead of the line in the file. */
void firm_up_the_line(void)
{
  unsigned short k; /* an index into |buffer| */
  limit=last;
  if (pausing>0) {
  if (interaction>nonstop_mode) {
    weak_up_terminal(); print_ln();
    if (start<limit) {
        for (k=start; k<limit;k++) print(buffer[k]);
    }
    first=limit; wake_up_terminal; print("=>") term_input();
    if (last>first) {
      for (k=first; k<last;k++)
        buffer[k+start-first]=buffer[k];
      limit=start+last-first;
    }
  }
}
