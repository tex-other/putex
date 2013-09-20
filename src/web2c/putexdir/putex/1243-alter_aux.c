Static void alteraux(void)
{
  halfword c;

  if (curchr != labs(mode)) {
    reportillegalcase();
    return;
  }
  c = curchr;
  scanoptionalequals();
  if (c == vmode) {
    scannormaldimen();
    prevdepth = curval;
    return;
  }
  scanint();
  if (curval > 0 && curval <= 32767) {
    spacefactor = curval;
    return;
  }
  printnl(S(292));
  print(S(946));
  help1(S(947));
  interror(curval);
}
