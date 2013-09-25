/* Here is a procedure that ignores text until coming to an \.{\\or},
\.{\\else}, or \.{\\fi} at level zero of $\.{\\if}\ldots\.{\\fi}$
nesting. After it has acted, |cur_chr| will indicate the token that
was found, but |cur_tok| will not be set (because this makes the
procedure run faster). */
void pass_text(void)
{
  integer l; /* level of $\.{\\if}\ldots\.{\\fi}$ nesting */
  small_number save_scanner_status; /* |scanner_status| upon entry */

  save_scanner_status=scanner_status; scanner_status=skipping; l=0;
  skip_line=line;
  while (true) {
    get_next();
    if (cur_cmd==fi_or_else) {
      if (l==0) goto PUdone;
      if (cur_chr==fi_code) decr(l);
    } else if (cur_cmd==if_test) incr(l);
  }
PUdone:
  scanner_status=save_scanner_status;
}
