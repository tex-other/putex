static void print_rule_dimen(scaled d) /* prints dimension in rule node */
{
  if (is_running(d)) print_char('*'); else print_scaled(d);
}
