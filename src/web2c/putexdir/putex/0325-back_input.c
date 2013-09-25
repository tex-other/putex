/* Sometimes \TeX\ has read too far and wants to ``unscan'' what it has
seen. The |back_input| procedure takes care of this by putting the token
just scanned back into the input stream, ready to be read again. This
procedure can be used only if |cur_tok| represents the token to be
replaced. Some applications of \TeX\ use this procedure a lot,
so it has been slightly optimized for speed. */
void back_input(void) /* undoes one token of input */
{
  pointer p; /* a token list of length one */
  while (state==token_list && loc==0 && token_type!=v_template)
      end_token_list(); /* conserve stack space */
  p=get_avail(); info(p)=cur_tok;
  if (cur_tok<right_brace_limit) {
      if (cur_tok<left_brace_limit) decr(alignstate);
      else incr(alignstate);
  }
  /* enter a new input level, save the old */
  if (input_ptr>max_in_stack) {
    max_in_stack=input_ptr;
    if (input_ptr==stack_size) overflow("input stack size",stack_size);
  }
  input_stack[input_ptr]=cur_input; /* stack the record */
  incr(input_ptr)
  state=token_list; start=p; token_type=backed_up;
  loc=p; /* that was |back_list(p)|, without procedure overhead */
}

