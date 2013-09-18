Static void print_mark(long p)
{
  print_char('{');
  if (p < himemmin || p > memend)
    print_esc(S(308));
  else
    show_token_list(link(p), 0, max_print_line - 10);
  print_char('}');
}
