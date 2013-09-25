/* The |scan_optional_equals| routine looks for an optional `\.=' sign preceded
by optional spaces; `\.{\\relax}' is not ignored here. */
void scan_optional_equals(void)
{
  do {
    get_x_token();
  } while (cur_cmd==spacer);
  if (cur_tok!=other_token+'=') backinput();
}
