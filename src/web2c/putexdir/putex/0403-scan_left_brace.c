/* The |scan_left_brace| routine is called when a left brace is supposed to be
the next non-blank token. (The term ``left brace'' means, more precisely,
a character whose catcode is |left_brace|.) \TeX\ allows \.{\\relax} to
appear before the |left_brace|. */
void scan_left_brace(void) /* reads a mandatory |left_brace| */
{
  do {
    get_x_token();
  } while (cur_cmd==spacer || cur_cmd==relax);
  if (cur_cmd!=left_brace) {
    print_err("Missing { inserted");
    help4("A left brace was mandatory here, so I've put one in.",
      "You might want to delete and/or insert some corrections",
      "so that I will find a matching right brace soon.",
      "(If you're confused by all this, try typing `I}' now.)");
  back_error(); cur_tok=left_brace_token+'{'; cur_cmd=left_brace;
  cur_chr='{'; incr(alignstate);
}

