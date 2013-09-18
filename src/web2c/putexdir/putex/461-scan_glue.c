Static void scanglue(smallnumber level)
{
  boolean negative, mu;
  pointer q;

  mu = (level == muval);   /*441:*/
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
  if (curcmd >= mininternal && curcmd <= maxinternal) {   /*462:*/
    scansomethinginternal(level, negative);
    if (curvallevel >= glueval) {
      if (curvallevel != level)
	muerror();
      goto _Lexit;
    }
    if (curvallevel == intval)
      scandimen(mu, false, true);
    else if (level == muval)
      muerror();
  } else {
    backinput();
    scandimen(mu, false, false);
    if (negative)
      curval = -curval;
  }
  q = newspec(zeroglue);
  width(q) = curval;
  if (scankeyword(S(637))) {
    scandimen(mu, true, false);
    stretch(q) = curval;
    stretchorder(q) = curorder;
  }
  if (scankeyword(S(638))) {
    scandimen(mu, true, false);
    shrink(q) = curval;
    shrinkorder(q) = curorder;
  }
  curval = q;   /*:462*/
_Lexit: ;
}
