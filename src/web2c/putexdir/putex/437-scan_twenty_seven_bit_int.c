Static void scantwentysevenbitint(void)
{
  scanint();
  if ((unsigned long)curval <= 134217727L)
    return;
  printnl(S(292));
  print(S(582));
  help2(S(583),S(575));
  interror(curval);
  curval = 0;
}
