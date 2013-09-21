Static void beginfilereading(void)
{
  if (inopen == maxinopen)
    overflow(S(510), maxinopen);
  if (first == bufsize)
    overflow(S(511), bufsize);
  inopen++;
  if (inputptr > maxinstack) {
    maxinstack = inputptr;
    if (inputptr == stacksize)
      overflow(S(508), stacksize);
  }
  inputstack[inputptr] = curinput;
  inputptr++;
  iindex = inopen;
  linestack[iindex - 1] = line;
  start = first;
  state = midline;
  name = 0;
}
