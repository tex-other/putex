Static void mathac(void)
{
  if (curcmd == accent) {   /*1166:*/
    printnl(S(292));
    print(S(912));
    printesc(S(913));
    print(S(914));
    help2(S(915),S(916));
    error();
  }
  /*:1166*/
  tailappend(getnode(accentnoadsize));
  type(tail) = accentnoad;
  subtype(tail) = normal;
  mem[nucleus(tail) - memmin].hh = emptyfield;
  mem[subscr(tail) - memmin].hh = emptyfield;
  mem[supscr(tail) - memmin].hh = emptyfield;
  mathtype(accentchr(tail)) = mathchar;
  scanfifteenbitint();
  character(accentchr(tail)) = curval & 255;
  if (curval >= varcode && faminrange) {
    fam(accentchr(tail)) = curfam;
  } else
    fam(accentchr(tail)) = (curval / 256) & 15;
  scanmath(nucleus(tail));
}

