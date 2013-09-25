/* Here is a procedure that starts a new level of token-list input, given
a token list |p| and its type |t|. If |t=macro|, the calling routine should
set |name| and |loc|. */
void begin_token_list(pointer p, quarterword t)
{
  /* enter a new input level, save the old */
  if (input_ptr>max_in_stack) {
    max_in_stack=input_ptr;
    if (input_ptr==stack_size) overflow("input stack size",stack_size);
  }
  input_stack[input_ptr]=cur_input; /* stack the record */
  incr(input_ptr);
  state=tokenlist; start=p; token_type=t;
  if (t<macro) { /* the token list starts with a reference count */
    add_token_ref(p);
    if (t==macro) {
      param_start=param_ptr;
    } else {
      loc = link(p);
      if (tracing_macros>1)
          begin_diagnostic(); print_nl('');
      switch (t) {
          case mark_text: print_esc("mark"); break;
          case write_text:  print_esc("write"); break;
          default: print_cmd_chr(assign_toks,t-output_text+output_routine_loc); break;
      }
      print("->"); token_show(p); end_diagnostic(false);
    }
  } else {
    loc=p;
  }
}
