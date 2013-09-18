Static void print_rule_dimen(long d)
{
  if (is_running(d)) {
    print_char('*');
  } else
    print_scaled(d);
}
