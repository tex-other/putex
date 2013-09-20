Static halfword thetoks(void)
{
  char oldsetting;
  pointer p, r;

  getxtoken();
  scansomethinginternal(tokval, false);
  if (curvallevel >= identval) {   /*466:*/
    p = temphead;
    link(p) = 0;
    if (curvallevel == identval) {
      storenewtoken(p,cstokenflag + curval);
      return p;
    }
    if (curval == 0)
      return p;
    r = link(curval);
    while (r != 0) {
      faststorenewtoken(p,info(r));
      r = link(r);
    }
    return p;
  } else {
    str_poolpointer b = str_mark();
    oldsetting = selector;
    selector = newstring;
    switch (curvallevel) {

    case intval:
      printint(curval);
      break;

    case dimenval:
      printscaled(curval);
      print(S(459));
      break;

    case glueval:
      printspec(curval, S(459));
      deleteglueref(curval);
      break;

    case muval:
      printspec(curval, S(390));
      deleteglueref(curval);
      break;
    }
    selector = oldsetting;
/*	fprintf(stderr,"(%d %d)",bb-b,poolptr-b); */
    return (strtoks(b));
  }
  /*:466*/
}
