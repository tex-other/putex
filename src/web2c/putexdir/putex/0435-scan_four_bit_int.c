Static void scanfourbitint(void)
{
  scanint();
  if ((unsigned long)curval <= 15)
    return;
  printnl(S(292));
  print(S(578));
  help2(S(579),S(575));
  interror(curval);
  curval = 0;
}

