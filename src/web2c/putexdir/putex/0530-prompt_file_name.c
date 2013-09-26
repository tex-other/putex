/* If some trouble arises when \TeX\ tries to open a file, the following
routine calls upon the user to supply another file name. Parameter~|s|
is used in the error message to identify the type of file; parameter~|e|
is the default extension if none is given. Upon exit from the routine,
variables |cur_name|, |cur_area|, |cur_ext|, and |name_of_file| are
ready for another attempt at file opening. */
void promptfilename(str_number s, str_number e)
{
  unsigned short k; /* index into |buffer| */

  if (interaction==scroll_mode) wake_up_terminal();
  if (s=="input file name") print_err("I can't find file `");
  else print_err("I can't write on file `");
  print_file_name(cur_name,cur_area,cur_ext); print("'.");
  if (e==".tex") show_context();
  print_nl("Please type another "); print(s);
  if (interaction<scroll_mode)
    fatal_error("*** (job aborted, file error in nonstop mode)");
  clear_terminal(); prompt_input(": ");
  begin_name();
  k=first;
  while (buffer[k]==' ' && k<last) incr(k);
  while (true) {
    if (k==last) goto PUdone;
    if (!morename(buffer[k])) goto PUdone;
    incr(k);
  }
PUdone:
  end_name();
  if (cur_ext=="") cur_ext=e;
  pack_file_name(cur_name,cur_area,cur_ext);
}

