Static void doregistercommand(smallnumber a)
{
  pointer l=0 /* XXXX */, q, r, s;
  char p;

  q = curcmd;   /*1237:*/
  if (q != register_) {
    getxtoken();
    if (curcmd >= assignint && curcmd <= assignmuglue) {
      l = curchr;
      p = curcmd - assignint;
      goto _Lfound;
    }
    if (curcmd != register_) {
      printnl(S(292));
      print(S(602));
      printcmdchr(curcmd, curchr);
      print(S(603));
      printcmdchr(q, 0);
      help1(S(941));
      error();
      goto _Lexit;
    }
  }
  p = curchr;
  scaneightbitint();
  switch (p) {

  case intval:
    l = curval + countbase;
    break;

  case dimenval:
    l = curval + scaledbase;
    break;

  case glueval:
    l = curval + skipbase;
    break;

  case muval:
    l = curval + muskipbase;
    break;
  }
_Lfound:   /*:1237*/
  if (q == register_)
    scanoptionalequals();
  else
    scankeyword(S(942));
  aritherror = false;
  if (q < multiply) {   /*1238:*/
    if (p < glueval) {
      if (p == intval)
	scanint();
      else {
	scannormaldimen();
      }
      if (q == advance)
	curval += eqtb[l - activebase].int_;
    } else {   /*:1238*/
      scanglue(p);
      if (q == advance) {   /*1239:*/
	q = newspec(curval);
	r = equiv(l);
	deleteglueref(curval);
	width(q) += width(r);
	if (stretch(q) == 0)
	  stretchorder(q) = normal;
	if (stretchorder(q) == stretchorder(r))
	  stretch(q) += stretch(r);
	else if ((stretchorder(q) < stretchorder(r)) &
		 (stretch(r) != 0)) {
	  stretch(q) = stretch(r);
	  stretchorder(q) = stretchorder(r);
	}
	if (shrink(q) == 0)
	  shrinkorder(q) = normal;
	if (shrinkorder(q) == shrinkorder(r))
	  shrink(q) += shrink(r);
	else if ((shrinkorder(q) < shrinkorder(r)) & (shrink(r) != 0)) {
	  shrink(q) = shrink(r);
	  shrinkorder(q) = shrinkorder(r);
	}
	curval = q;
      }
      /*:1239*/
    }
  } else {   /*1240:*/
    scanint();
    if (p < glueval) {
      if (q == multiply) {
	if (p == intval)
	  curval = multandadd(eqtb[l - activebase].int_, curval, 0,
			      2147483647L);
	else
	  curval = multandadd(eqtb[l - activebase].int_, curval, 0,
			      1073741823L);
      } else
	curval = xovern(eqtb[l - activebase].int_, curval);
    } else {
      s = equiv(l);
      r = newspec(s);
      if (q == multiply) {
	width(r) = multandadd(width(s), curval, 0, 1073741823L);
	stretch(r) = multandadd(stretch(s), curval, 0, 1073741823L);
	shrink(r) = multandadd(shrink(s), curval, 0, 1073741823L);
      } else {
	width(r) = xovern(width(s), curval);
	stretch(r) = xovern(stretch(s), curval);
	shrink(r) = xovern(shrink(s), curval);
      }
      curval = r;
    }
  }  /*:1240*/
  if (aritherror) {
    printnl(S(292));
    print(S(943));
    help2(S(944),S(945));
    error();
    goto _Lexit;
  }
  if (p < glueval) {
    worddefine(l, curval);
  } else {
    trapzeroglue();
    define(l, glueref, curval);
  }
_Lexit: ;
}
