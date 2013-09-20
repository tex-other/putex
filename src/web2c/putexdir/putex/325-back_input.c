Static void backinput(void)
{
  pointer p;

  while (state == tokenlist && loc == 0)
    endtokenlist();
  p = getavail();
  info(p) = curtok;
  if (curtok < rightbracelimit) {
    if (curtok < leftbracelimit)
      alignstate--;
    else
      alignstate++;
  }
  if (inputptr > maxinstack) {
    maxinstack = inputptr;
    if (inputptr == stacksize)
      overflow(S(508), stacksize);
  }
  inputstack[inputptr] = curinput;
  inputptr++;
  state = tokenlist;
  start = p;
  tokentype = backedup;
  loc = p;
}

