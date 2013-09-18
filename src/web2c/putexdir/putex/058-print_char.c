void printchar(ASCIIcode s)
{
  if (s == newlinechar) {   /*:244*/
    if (selector < pseudo) {
      println();
      goto _Lexit;
    }
  }
  switch (selector) {

  case term_and_log:
    fwrite(&xchr[s], 1, 1, termout);
    fwrite(&xchr[s], 1, 1, logfile);
    termoffset++;
    fileoffset++;
    if (termoffset == maxprintline) {
      putc('\n', termout);
      termoffset = 0;
    }
    if (fileoffset == maxprintline) {
      putc('\n', logfile);
      fileoffset = 0;
    }
    break;

  case log_only:
    fwrite(&xchr[s], 1, 1, logfile);
    fileoffset++;
    if (fileoffset == max_print_line)
      println();
    break;

  case term_only:
    fwrite(&xchr[s], 1, 1, termout);
    termoffset++;
    if (termoffset == maxprintline)
      println();
    break;

  case noprint:
    /* blank case */
    break;

  case pseudo:
    if (tally < trickcount) {
      trickbuf[tally % errorline] = s;
    }
    break;

  case newstring:
#if 0
    if (poolptr < poolsize) {
      appendchar(s);
    }
#else
	string_print_proc(s);
/*
	str_appendchar(s);
*/
#endif
    break;

  default:
    fwrite(&xchr[s], 1, 1, writefile[selector]);
    break;
  }
  tally++;
_Lexit: ;
}
