Static void preparemag(void)
{
  if (magset > 0 && mag != magset) {
    printnl(S(292));
    print(S(481));
    printint(mag);
    print(S(482));
    printnl(S(483));
    help2(S(484),S(485));
    interror(magset);
    geqworddefine(intbase + magcode, magset);
  }
  if (mag <= 0 || mag > 32768L) {
    printnl(S(292));
    print(S(486));
    help1(S(487));
    interror(mag);
    geqworddefine(intbase + magcode, 1000);
  }
  magset = mag;
}

