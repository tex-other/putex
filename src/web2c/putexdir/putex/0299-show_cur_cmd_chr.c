/* Here is a procedure that displays the current command. */
void show_cur_cmd_chr(void)
{
  begin_diagnostic(); print_nl('{');
  if (mode!=shown_mode) {
    print_mode(mode); print(": "); shown_mode=mode;
  }
  print_cmd_chr(cur_cmd, cur_chr); print_char('}');
  end_diagnostic(false);
}

