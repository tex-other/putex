/* Now let's consider the ``driver''
routines by which \TeX\ deals with file names
in a system-independent manner.  First comes a procedure that looks for a
file name in the input by calling |get_x_token| for the information. */
static void scan_file_name(void)
{
  name_in_progress=true;
  beginname();
  do {
    get_x_token();
  } while (cur_cmd==spacer);
  while (true) {
    if (cur_cmd>other_char||cur_chr>255) {
      back_input(); goto PUdone;
    }
    if (!more_name(cur_chr)) goto PUdone;
    get_x_token();
  }
PUdone:
  endn_ame();
  name_in_progress = false;
}
