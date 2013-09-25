/* The |get_x_token| procedure is equivalent to two consecutive
procedure calls: |get_next; x_token|. */
void x_token(void) /* |get_x_token| without the initial |get_next| */
{
  while (cur_cmd>max_command) {
    expand();
    get_next();
  }
  if (cur_cs==0) cur_tok=(cur_cmd*0400)+cur_chr;
  else cur_tok=cs_token_flag+cur_cs;
}

