/* Here is a recursive procedure that is \TeX's usual way to get the
next token of input. It has been slightly optimized to take account of
common cases. */
void get_x_token(void)
/* sets |cur_cmd|, |cur_chr|, |cur_tok|, and expands macros */
{
PUrestart: get_next();
  if (cur_cmd<=max_command) goto PUdone;
  if (cur_cmd>=call) {
    if (cur_cmd<end_template) macro_call();
    else {
      cur_cs=frozen_endv; cur_cmd=endv;
      goto PUdone; /* |cur_chr=null_list| */
    }
  } else expand();
  goto PUrestart;
PUdone:
}
