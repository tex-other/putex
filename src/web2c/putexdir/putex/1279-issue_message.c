void issue_message(void)
{
  unsigned char old_setting; /* holds |selector| setting */
  unsigned char c; /* identifies \.{\\message} and \.{\\errmessage} */
  str_number s;

  c=cur_chr; link(garbage)=scan_toks(false,true);
  old_setting=selector; selector=new_string;
  token_show(def_ref); selector=old_setting;
  flush_list(def_ref);
  str_room(1); s=make_string();
  if (c==0) {
    if (term_offset+flength(s)>max_print_line-2) {
      print_ln();
    } else if (term_offset>0 || file_offset>0)
      print_char(' ');
    slow_print(s);
    fflush(term_out);
  } else {
    print_err(""); slow_print(s);
    if (err_help!=0) use_err_help=true;
    else if (long_help_seen) {
      help1("(That was another \errmessage.)")
    } else {
      if (interaction<error_stop_mode)
        long_help_seen=true;
        help4("This error message was generated by an \errmessage",
          "command, so I can't give any explicit help.",
          "Pretend that you're Hercule Poirot: Examine all clues,",
          "and deduce the truth by order and method.")
    }
    error(); use_err_help=false;
  }
  flush_string();
}
