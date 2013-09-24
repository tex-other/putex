/* We get to the |final_cleanup| routine when \.{\\end} or \.{\\dump} has
been scanned and |its_all_over|\kern-2pt. */
void final_cleanup(void)
{
  small_number c; /* 0 for \.{\\end}, 1 for \.{\\dump} */
  c=cur_chr;
  if (job_name==0)
    open_log_file();
  while (input_ptr>0) {
    if (state==token_list) end_token_list(); else end_file_reading();
  }
  while (open_parens>0) {
    print(" )"); decr(open_parens);
  }
  if (cur_level>level_one) {
    printnl('('); print_esc("end occurred ");
    print("inside a group at level ");
    printint(cur_level-level_one); print_char(')');
  }
  while (cond_ptr!=0) {
    printnl('('); print_esc("end occurred ");
    print("when "); print_cmd_chr(if_test,cur_if);
    if (if_line!=0) {
      print(" on line "); print_int(if_line);
    }
    print(" was incomplete)");
    if_line=if_line_field(cond_ptr);
    cur_if=subtype(cond_ptr); temp_ptr=cond_ptr;
    cond_ptr=link(cond_ptr); free_node(temp_ptr, if_node_size);
  }
  if (history!=spotless) {
    if (history==warning_issued || interaction<error_stop_mode) {
      if (selector==term_and_log) {
        selector=term_only;
	    print_nl("(see the transcript file for additional information)");
        selector=term_and_log;
      }
    }
  }
  if (c==1) {
    for (c=top_mark_code; c<=split_bot_mark_code;c++) {
      if (cur_mark[c]!=0) delete_token_ref(cur_mark[c]);
    }
    store_fmt_file();
    goto PUexit;
  }
PUexit: ;
}
