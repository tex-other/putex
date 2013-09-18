Static void insertdollarsign(void)
{
  backinput();
  curtok = mathshifttoken + '$';
  printnl(S(292));
  print(S(827));
  help2(S(828),S(829));
  inserror();
}

