Static void begintokenlist(halfword p, quarterword t)
{
  if (inputptr > maxinstack) {
    maxinstack = inputptr;
    if (inputptr == stacksize)
      overflow(S(508), stacksize);
  }
  inputstack[inputptr] = curinput;
  inputptr++;
  state = tokenlist;
  start = p;
  tokentype = t;
  if (t < macro) {
    loc = p;
    return;
  }
  addtokenref(p);
  if (t == macro) {
    paramstart = paramptr;
    return;
  }
  loc = link(p);
  if (tracingmacros <= 1)
    return;
  begindiagnostic();
  printnl(S(385));
  switch (t) {

  case marktext:
    printesc(S(402));
    break;

  case writetext:
    printesc(S(379));
    break;

  default:
    printcmdchr(assigntoks, t - outputtext + outputroutineloc);
    break;
  }
  print(S(310));
  tokenshow(p);
  enddiagnostic(false);
}
