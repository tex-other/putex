Static void convtoks(void)
{
  char oldsetting;
  char c;
  smallnumber savescannerstatus;
  str_poolpointer b;

  c = curchr;   /*471:*/
  switch (c) {   /*:471*/

  case numbercode:
  case romannumeralcode:
    scanint();
    break;

  case stringcode:
  case meaningcode:
    savescannerstatus = scannerstatus;
    scannerstatus = normal;
    gettoken();
    scannerstatus = savescannerstatus;
    break;

  case fontnamecode:
    scanfontident();
    break;

  case jobnamecode:
    if (jobname == 0)
      openlogfile();
    break;
  }
  oldsetting = selector;
  selector = newstring;
  b = str_mark();
  /*472:*/
  switch (c) {   /*:472*/

  case numbercode:
    printint(curval);
    break;

  case romannumeralcode:
    printromanint(curval);
    break;

  case stringcode:
    if (curcs != 0)
      sprintcs(curcs);
    else
      printchar(curchr);
    break;

  case meaningcode:
    printmeaning(curchr, curcmd);
    break;

  case fontnamecode:
    print(get_fontname(curval));
    if (get_fontsize(curval) != get_fontdsize(curval)) {
      print(S(642));
      printscaled(get_fontsize(curval));
      print(S(459));
    }
    break;

  case jobnamecode:
    print(jobname);
    break;
  }
  selector = oldsetting;
  link(garbage) = strtoks(b);
  inslist(link(temphead));
}  /*:470*/
