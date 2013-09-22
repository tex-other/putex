static void print_mark(integer p) /* prints token list data in braces */
{
  print_char('{');
  if (p<hi_mem_min||p>mem_end) print_esc("CLOBBERED.");
  else show_token_list(link(p), 0, max_print_line-10);
  print_char('}');
}
