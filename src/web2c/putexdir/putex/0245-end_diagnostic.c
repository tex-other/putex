void end_diagnostic(boolean blank_line)
/* restore proper conditions after tracing */
{
  print_nl("");
  if (blank_line) print_ln();
  selector=old_setting;
}
