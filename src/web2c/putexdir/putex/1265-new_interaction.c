void new_interaction(void)
{
  print_ln();
  interaction=cur_chr;
  if (interaction==batch_mode) selector=no_print;
  else selector=term_only;
  if (log_opened) selector+=2;
}
