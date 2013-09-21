/* Here is a procedure that uses |scanner_status| to print a warning message
when a subfile has ended, and at certain other crucial times: */
static void runaway(void)
{
  pointer p=0; /* head of runaway list */
  if (scanner_status<=skipping)
    return;
  printnl("Runaway ");
  switch (scanner_status) {
    case defining: print("definition"); p = def_ref;
      break;
    case matching: print("argument"); p = temp_head;
      break;
    case aligning: print("preamble"); p = hold_head;
      break;
    case absorbing: print("text"); p = def_ref;
      break;
  }
  print_char('?'); print_ln(); show_token_list(link(p), 0, errorline-10);
}

