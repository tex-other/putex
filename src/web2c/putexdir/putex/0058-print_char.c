/* The |print_char| procedure sends one character to the desired destination,
using the |xchr| array to map it into an external character compatible with
|input_ln|. All printing comes through |print_ln| or |print_char|. */
void print_char(ASCII_code s) /* prints a single character */
{
  if (s==newlinechar) {
    if (selector<pseudo) {
      print_ln(); goto PUexit;
    }
  }
  switch (selector) {
    case term_and_log: fwrite(&xchr[s], 1, 1, termout); fwrite(&xchr[s], 1, 1, logfile);
      term_offset++; file_offset++;
      if (term_offset==max_print_line) {
        putc('\n', termout); term_offset=0;
      }
      if (file_offset==max_print_line) {
        putc('\n', logfile); file_offset=0;
      }
      break;
    case log_only: fwrite(&xchr[s], 1, 1, logfile); file_offset++;
      if (fileoffset==max_print_line) print_ln();
      break;
    case term_only: fwrite(&xchr[s], 1, 1, termout); term_offset++;
      if (term_offset==max_print_line) print_ln();
      break;
    case no_print:
      break;
    case pseudo:
      if (tally<trickcount) trick_buf[tally%errorline]=s;
      break;
    case new_string:
#if 0
      if (poolptr<poolsize) append_char(s);
#else
      string_print_proc(s);
/*
      str_appendchar(s);
*/
#endif
      break;
    default: fwrite(&xchr[s], 1, 1, writefile[selector]);
      break;
  }
  tally++;
PUexit: ;
}
