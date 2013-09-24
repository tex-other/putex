void fatal_error(str_number s) /* prints |s|, and that's it */
{
  normalize_selector();
  print_err("Emergency stop");
  help1(s);
  if (interaction==error_stop_mode)
    interaction=scroll_mode;
  if (log_opened)
    error();
  if (interaction>batch_mode)
    debughelp();
  history=fatal_error_stop;
  jump_out();
}
