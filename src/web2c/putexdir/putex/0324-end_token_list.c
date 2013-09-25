/* When a token list has been fully scanned, the following computations
should be done as we leave that level of input. The |token_type| tends
to be equal to either |backed_up| or |inserted| about 2/3 of the time. */
void endtokenlist(void) /* leave a token-list input level */
{
  if (token_type>=backed_up) { /* token list to be deleted */
      if (token_type<=inserted) flush_list(start);
      else {
        delete_token_ref(start); /* update reference count */
        if (token_type==macro) { /* parameters must be flushed */
          while (param_ptr>param_start) {
            decr(param_ptr);
            flush_list(param_stack[param_ptr]);
          }
        }
      }
  } else if (token_type==utemplate) {
      if (align_state>500000L) align_state=0;
      else fatal_error("(interwoven alignment preambles are not allowed)");
  }
  pop_input();
  check_interrupt();
}

