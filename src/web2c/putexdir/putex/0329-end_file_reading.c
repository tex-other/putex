Static void endfilereading(void)
{
  first = start;
  line = linestack[iindex - 1];
  if (name > 17)
    aclose(&curfile);
  popinput();
  inopen--;
}

