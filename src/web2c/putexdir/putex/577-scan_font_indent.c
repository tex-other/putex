Static void scanfontident(void)
{   /*406:*/
  internalfontnumber f;
  halfword m;

  do {
    getxtoken();   /*:406*/
  } while (curcmd == spacer);
  if (curcmd == deffont)
    f = curfont;
  else if (curcmd == setfont)
    f = curchr;
  else if (curcmd == deffamily) {
    m = curchr;
    scanfourbitint();
    f = equiv(m + curval);
  } else {
    printnl(S(292));
    print(S(584));
    help2(S(585),S(586));
    backerror();
    f = nullfont;
  }
  curval = f;
}
