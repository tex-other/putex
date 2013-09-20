Static void getrtoken(void)
{
_Lrestart:
  do {
    gettoken();
  } while (curtok == spacetoken);
  if (curcs != 0 && curcs <= frozencontrolsequence)
    return;
  printnl(S(292));
  print(S(935));
  help5(S(936),S(937),S(938),S(939),S(940));
  if (curcs == 0)
    backinput();
  curtok = cstokenflag + frozenprotection;
  inserror();
  goto _Lrestart;
}
