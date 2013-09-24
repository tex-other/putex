void write_out(pointer p)
{
  unsigned char old_setting; /* holds print |selector| */
  integer old_mode; /* saved |mode| */
  small_number j; /* write stream number */
  pointer q, r; /* temporary variables for list manipulation */

  q=get_avail(); info(q)=right_brace_token+'}';
  r=get_avail(); link(q)=r; info(r)=end_write_token; ins_list(q);
  begin_token_list(write_tokens(p),write_text);
  q=get_avail(); info(q)=left_brace_token+'{'; ins_list(q);
  /* now we're ready to scan `\.\{$\langle\,$token list$\,\rangle$\.{\} \\endwrite}' */
  old_mode=mode; mode=0;
  /* disable \.{\\prevdepth}, \.{\\spacefactor}, \.{\\lastskip}, \.{\\prevgraf} */
  cur_cs=write_loc; q=scan_toks(false,true); /* expand macros, etc. */
  get_token(); if (cur_tok!=endwrite_token) {
    print_err("Unbalanced write command");
    print(S(680));
    help2("On this page there's a \write with fewer real {'s than }'s.",
            "I can't handle that very well; good luck.");
    error();
    do {
      get_token();
    } while (cur_tok!=end_write_token);
  }
  mode=old_mode;
  end_token_list(); /* conserve stack space */
  old_setting=selector; j=write_stream(p);
  if (write_open[j]) selector=j;
  else {
    if (j==17 && selector==term_and_log) selector=log_only;
    printnl("");
  }
  token_show(def_ref); print_ln();
  flush_list(def_ref); selector=old_setting;
}
