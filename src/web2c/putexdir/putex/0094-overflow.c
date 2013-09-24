/* Here is the most dreaded error message. */
void overflow(str_number s, integer n)
{
  normalize_selector();
  print_err("TeX capacity exceeded, sorry [");
  print(s); print_char('='); print_int(n); print_char(']');
  help2("If you really absolutely need more capacity,",
          "you can ask a wizard to enlarge me.");
  if (interaction==error_stop_mode)
    interaction=scroll_mode;
  if (log_opened)
    error();
  if (interaction>batch_mode)
    debughelp();
  history=fatal_error_stop;
  jump_out();
}
