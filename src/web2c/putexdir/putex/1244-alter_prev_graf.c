Static void alterprevgraf(void)
{
  int p;

  nest[nestptr] = curlist;
  p = nestptr;
  while (abs(nest[p].modefield) != vmode)
    p--;
  scanoptionalequals();
  scanint();
  if (curval >= 0) {
    nest[p].pgfield = curval;
    curlist = nest[nestptr];
    return;
  }
  printnl(S(292));
  print(S(773));
  printesc(S(948));
  help1(S(949));
  interror(curval);
}
