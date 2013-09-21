/* To end a line of text output, we call |print_ln|. */
void print_ln(void) /* prints an end-of-line */
{
  switch (selector) {
    case term_and_log: putc('\n', termout); putc('\n', logfile);
      term_offset=0; file_offset=0;
      break;
    case log_only: putc('\n', logfile); file_offset=0;
      break;
    case term_only: putc('\n', termout); term_offset=0;
      break;
    case no_print: case pseudo: case new_string: break;
    default: putc('\n', write_file[selector]); break;
    }
} /* |tally| is not affected */
