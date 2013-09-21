Static void conditional(void)
{  /*495:*/
  boolean b=false /* XXXX */;
  long r;
  long m, n;
  pointer p, q, savecondptr;
  smallnumber savescannerstatus, thisif;

  p = getnode(ifnodesize);
  link(p) = condptr;
  type(p) = iflimit;
  subtype(p) = curif;
  iflinefield(p) = ifline;
  condptr = p;
  curif = curchr;
  iflimit = ifcode;
  ifline = line;   /*:495*/
  savecondptr = condptr;
  thisif = curchr;   /*501:*/
  switch (thisif) {   /*:501*/

  case ifcharcode:
  case ifcatcode:   /*506:*/
    getxtokenoractivechar();
    if (curcmd > activechar || curchr > 255) {
      m = relax;
      n = 256;
    } else {
      m = curcmd;
      n = curchr;
    }
    getxtokenoractivechar();
    if (curcmd > activechar || curchr > 255) {
      curcmd = relax;
      curchr = 256;
    }
    if (thisif == ifcharcode)
      b = (n == curchr);
    else
      b = (m == curcmd);
    break;
    /*:506*/

  case ifintcode:
  case ifdimcode:   /*503:*/
    if (thisif == ifintcode)
      scanint();
    else {
      scannormaldimen();
    }
    n = curval;
    /*406:*/
    do {
      getxtoken();
      /*:406*/
    } while (curcmd == spacer);
    if ((curtok >= othertoken + '<') & (curtok <= othertoken + '>'))
      r = curtok - othertoken;
    else {
      printnl(S(292));
      print(S(659));
      printcmdchr(iftest, thisif);
      help1(S(660));
      backerror();
      r = '=';
    }
    if (thisif == ifintcode)
      scanint();
    else {
      scannormaldimen();
    }
    switch (r) {

    case '<':
      b = (n < curval);
      break;

    case '=':
      b = (n == curval);
      break;

    case '>':
      b = (n > curval);
      break;
    }
    break;
    /*:503*/

  case ifoddcode:   /*504:*/
    scanint();
    b = curval & 1;
    break;
    /*:504*/

  case ifvmodecode:
    b = (labs(mode) == vmode);
    break;

  case ifhmodecode:
    b = (labs(mode) == hmode);
    break;

  case ifmmodecode:
    b = (labs(mode) == mmode);
    break;

  case ifinnercode:
    b = (mode < 0);
    break;

  case ifvoidcode:
  case ifhboxcode:
  case ifvboxcode:   /*505:*/
    scaneightbitint();
    p = box(curval);
    if (thisif == ifvoidcode)
      b = (p == 0);
    else if (p == 0)
      b = false;
    else if (thisif == ifhboxcode)
      b = (type(p) == hlistnode);
    else
      b = (type(p) == vlistnode);
    break;
    /*:505*/

  case ifxcode:   /*507:*/
    savescannerstatus = scannerstatus;
    scannerstatus = normal;
    getnext();
    n = curcs;
    p = curcmd;
    q = curchr;
    getnext();
    if (curcmd != p)
      b = false;
    else if (curcmd < call)
      b = (curchr == q);
    else {
      /*
508:*/
      p = link(curchr);
      q = link(equiv(n));
      if (p == q)
	b = true;
      else {
	while (p != 0 && q != 0) {
	  if (info(p) != info(q))
	    p = 0;
	  else {
	    p = link(p);
	    q = link(q);
	  }
	}
	b = (p == 0 && q == 0);
      }
    }
    scannerstatus = savescannerstatus;
    break;
    /*:507*/

  case ifeofcode:
    scanfourbitint();
    b = (readopen[curval] == closed);
    break;

  case iftruecode:
    b = true;
    break;

  case iffalsecode:
    b = false;
    break;

  case ifcasecode:   /*509:*/
    scanint();
    n = curval;
    if (tracingcommands > 1) {
      begindiagnostic();
      print(S(661));
      printint(n);
      printchar('}');
      enddiagnostic(false);
    }
    while (n != 0) {
      passtext();
      if (condptr == savecondptr) {
	if (curchr != orcode)
	  goto _Lcommonending;
	n--;
	continue;
      }
      if (curchr != ficode)   /*496:*/
	continue;
      /*:496*/
      p = condptr;
      ifline = iflinefield(p);
      curif = subtype(p);
      iflimit = type(p);
      condptr = link(p);
      freenode(p, ifnodesize);
    }
    changeiflimit(orcode, savecondptr);
    goto _Lexit;
    break;
    /*:509*/
  }
  if (tracingcommands > 1) {   /*502:*/
    begindiagnostic();
    if (b)
      print(S(662));
    else
      print(S(663));
    enddiagnostic(false);
  }
  /*:502*/
  if (b) {
    changeiflimit(elsecode, savecondptr);
    goto _Lexit;
  }  /*500:*/
  while (true) {   /*:500*/
    passtext();
    if (condptr == savecondptr) {
      if (curchr != orcode)
	goto _Lcommonending;
      printnl(S(292));
      print(S(558));
      printesc(S(664));
      help1(S(559));
      error();
      continue;
    }
    if (curchr != ficode)   /*496:*/
      continue;
    /*:496*/
    p = condptr;
    ifline = iflinefield(p);
    curif = subtype(p);
    iflimit = type(p);
    condptr = link(p);
    freenode(p, ifnodesize);
  }
_Lcommonending:
  if (curchr == ficode) {   /*496:*/
    p = condptr;
    ifline = iflinefield(p);
    curif = subtype(p);
    iflimit = type(p);
    condptr = link(p);
    freenode(p, ifnodesize);
  }  /*:496*/
  else
    iflimit = ficode;
_Lexit: ;

  /*:508*/
}
