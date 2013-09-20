Static void scanfifteenbitint(void)
{
  scanint();
  if ((unsigned long)curval <= 32767)
    return;
  printnl(S(292));
  print(S(580));
  help2(S(581),S(575));
  interror(curval);
  curval = 0;
}
