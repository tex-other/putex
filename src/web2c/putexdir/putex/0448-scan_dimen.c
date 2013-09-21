Static void scandimen(boolean mu, boolean inf, boolean shortcut)
{
  boolean negative;
  long f;
  /*450:*/
  long num, denom;
  /* smallnumber */ int k, kk; /* INT */
  pointer p, q;
  scaled v;
  long savecurval;   /*:450*/
  char digs[23];

  f = 0;
  aritherror = false;
  curorder = normal;
  negative = false;
  if (!shortcut) {   /*441:*/
    negative = false;
    do {   /*406:*/
      do {
	getxtoken();   /*:406*/
      } while (curcmd == spacer);
      if (curtok == othertoken + '-') {   /*:441*/
	negative = !negative;
	curtok = othertoken + '+';
      }
    } while (curtok == othertoken + '+');
    if (curcmd >= mininternal && curcmd <= maxinternal) {   /*449:*/
      if (mu) {
	scansomethinginternal(muval, false);   /*451:*/
	if (curvallevel >= glueval) {   /*:451*/
	  v = width(curval);
	  deleteglueref(curval);
	  curval = v;
	}
	if (curvallevel == muval)
	  goto _Lattachsign_;
	if (curvallevel != intval)
	  muerror();
      } else {
	scansomethinginternal(dimenval, false);
	if (curvallevel == dimenval)
	  goto _Lattachsign_;
      }  /*:449*/
    } else {
      backinput();
      if (curtok == continentalpointtoken)
	curtok = pointtoken;
      if (curtok != pointtoken)
	scanint();
      else {
	radix = 10;
	curval = 0;
      }
      if (curtok == continentalpointtoken)
	curtok = pointtoken;
      if (radix == 10 && curtok == pointtoken) {   /*452:*/
	k = 0;
	p = 0;
	gettoken();
	while (true) {
	  getxtoken();
	  if (curtok > zerotoken + 9 || curtok < zerotoken)
	    goto _Ldone1;
	  if (k >= 17)
	    continue;
	  q = getavail();
	  link(q) = p;
	  info(q) = curtok - zerotoken;
	  p = q;
	  k++;
	}
_Ldone1:
	for (kk = k - 1; kk >= 0; kk--) {
	  digs[kk] = info(p);
	  q = p;
	  p = link(p);
	  freeavail(q);
	}
	f = rounddecimals(k,digs);
	if (curcmd != spacer)
	  backinput();
      }
      /*:452*/
    }
  }
  if (curval < 0) {
    negative = !negative;
    curval = -curval;
  }  /*453:*/
  if (inf) {   /*454:*/
    if (scankeyword(S(330))) {   /*:454*/
      curorder = fil;
      while (scankeyword('l')) {
	if (curorder != filll) {
	  curorder++;
	  continue;
	}
	printnl(S(292));
	print(S(611));
	print(S(612));
	help1(S(613));
	error();
      }
      goto _Lattachfraction_;
    }
  }
  /*455:*/
  savecurval = curval;   /*406:*/
  do {
    getxtoken();   /*:406*/
  } while (curcmd == spacer);
  if (curcmd >= mininternal && curcmd <= maxinternal) {
    if (mu) {
      scansomethinginternal(muval, false);   /*451:*/
      if (curvallevel >= glueval) {   /*:451*/
	v = width(curval);
	deleteglueref(curval);
	curval = v;
      }
      if (curvallevel != muval)
	muerror();
    } else
      scansomethinginternal(dimenval, false);
    v = curval;
    goto _Lfound;
  }
  backinput();
  if (mu)
    goto _Lnotfound;
  if (scankeyword(S(614)))   /*443:*/
    v = quad(curfont);   /*558:*/
    /*:558*/
  else if (scankeyword(S(615)))
    v = xheight(curfont);   /*559:*/
    /*:559*/
  else
    goto _Lnotfound;
  getxtoken();
  if (curcmd != spacer)   /*:443*/
    backinput();
_Lfound:
  curval = multandadd(savecurval, v, xnoverd(v, f, 65536L), 1073741823L);
  goto _Lattachsign_;
_Lnotfound:   /*:455*/
  if (mu) {   /*456:*/
    if (scankeyword(S(390)))
      goto _Lattachfraction_;
    else {   /*:456*/
      printnl(S(292));
      print(S(611));
      print(S(616));
      help4(S(617),S(618),S(619),S(620));
      error();
      goto _Lattachfraction_;
    }
  }
  if (scankeyword(S(621))) {   /*457:*/
    preparemag();
    if (mag != 1000) {
      curval = xnoverd(curval, 1000, mag);
      f = (f * 1000 + texremainder * 65536L) / mag;
      curval += f / 65536L;
      f %= 65536L;
/* p2c: tex1.p: Note: Using % for possibly-negative arguments [317] */
    }
  }
  /*:457*/
  if (scankeyword(S(459)))   /*458:*/
    goto _Lattachfraction_;
  if (scankeyword(S(622))) {
    num = 7227;
    denom = 100;
  } else if (scankeyword(S(623))) {
    num = 12;
    denom = 1;
  } else if (scankeyword(S(624))) {
    num = 7227;
    denom = 254;
  } else if (scankeyword(S(625))) {
    num = 7227;
    denom = 2540;
  } else if (scankeyword(S(626))) {
    num = 7227;
    denom = 7200;
  } else if (scankeyword(S(627))) {
    num = 1238;
    denom = 1157;
  } else if (scankeyword(S(628))) {
    num = 14856;
    denom = 1157;
  } else if (scankeyword(S(629)))
    goto _Ldone;
  else {
    printnl(S(292));
    print(S(611));
    print(S(630));
    help6(S(631),S(632),S(633),S(618),S(619),S(620));
    error();
    goto _Ldone2;
  }
  curval = xnoverd(curval, num, denom);
  f = (num * f + texremainder * 65536L) / denom;
  curval += f / 65536L;
  f %= 65536L;
/* p2c: tex1.p: Note: Using % for possibly-negative arguments [317] */
_Ldone2:   /*:458*/
_Lattachfraction_:
  if (curval >= 16384)
    aritherror = true;
  else
    curval = curval * unity + f;
_Ldone:   /*:453*/
  /*443:*/
  getxtoken();
  if (curcmd != spacer)   /*:443*/
    backinput();
_Lattachsign_:
  if (aritherror || labs(curval) >= 1073741824L) {   /*460:*/
    printnl(S(292));
    print(S(634));
    help2(S(635),S(636));
    error();
    curval = maxdimen;
    aritherror = false;
  }
  /*:460*/
  if (negative)
    curval = -curval;

  /*459:*/
  /*:459*/
}
