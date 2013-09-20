Static void headforvmode(void)
{
  if (mode < 0) {
    if (curcmd != hrule) {
      offsave();
      return;
    }
    printnl(S(292));
    print(S(602));
    printesc(S(863));
    print(S(864));
    help2(S(865),S(866));
    error();
    return;
  }
  backinput();
  curtok = partoken;
  backinput();
  tokentype = inserted;
}
