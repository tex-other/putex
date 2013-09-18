Static void scancharnum(void)
{
  scanint();
  if ((unsigned long)curval <= 255)
    return;
  printnl(S(292));
  print(S(576));
  help2(S(577),S(575));
  interror(curval);
  curval = 0;
}
