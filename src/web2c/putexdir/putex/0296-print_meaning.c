/* The |print_meaning| subroutine displays |cur_cmd| and |cur_chr| in
symbolic form, including the expansion of a macro or mark. */
void print_meaning(void)
{
  print_cmd_chr(cur_cmd,cur_chr);
  if (cur_cmd>=call) {
    print_char(':'); print_ln(); token_show(cur_chr);
    return;
  }
  if (cur_cmd!=top_bot_mark)
    return;
  print_char(':');  print_ln();
  token_show(cur_mark[cur_chr]);
}

