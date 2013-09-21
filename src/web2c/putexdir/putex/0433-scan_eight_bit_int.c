Static void scaneightbitint(void)
{
  scanint();
  if ((unsigned long)curval <= 255)
    return;
  printnl(S(292));
  print(S(573));
  help2(S(574),S(575));
  interror(curval);
  curval = 0;
}

