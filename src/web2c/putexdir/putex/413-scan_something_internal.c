Static void scansomethinginternal(smallnumber level, boolean negative)
{
  halfword m;
  /* char */ int p; /* INT */

  m = curchr;
  switch (curcmd) {

  case defcode:   /*414:*/
    scancharnum();
    if (m == mathcodebase) {
      curval = mathcode(curval);
      curvallevel = intval;
    } else if (m < mathcodebase) {
      curval = equiv(m + curval);
      curvallevel = intval;
    } else {
      curval = eqtb[m + curval - activebase].int_;
      curvallevel = intval;
    }
    break;
    /*:414*/

  case toksregister:
  case assigntoks:
  case deffamily:
  case setfont:
  case deffont:   /*415:*/
    if (level != tokval) {
      printnl(S(292));
      print(S(593));
      help3(S(594),S(595),S(596));
      backerror();
      curval = 0;
      curvallevel = dimenval;
    } else if (curcmd <= assigntoks) {
      if (curcmd < assigntoks) {
	scaneightbitint();
	m = toksbase + curval;
      }
      curval = equiv(m);
      curvallevel = tokval;
    } else {
      backinput();
      scanfontident();
      curval += fontidbase;
      curvallevel = identval;
    }
    break;

  case assignint:
    curval = eqtb[m - activebase].int_;
    curvallevel = intval;
    break;

  case assigndimen:
    curval = eqtb[m - activebase].sc;
    curvallevel = dimenval;
    break;

  case assignglue:
    curval = equiv(m);
    curvallevel = glueval;
    break;

  case assignmuglue:
    curval = equiv(m);
    curvallevel = muval;
    break;

  case setaux:   /*418:*/
    if (labs(mode) != m) {
      printnl(S(292));
      print(S(597));
      printcmdchr(setaux, m);
      help4(S(598),S(599),S(600),S(601));
      error();
      if (level != tokval) {
	curval = 0;
	curvallevel = dimenval;
      } else {
	curval = 0;
	curvallevel = intval;
      }
    } else if (m == vmode) {
      curval = prevdepth;
      curvallevel = dimenval;
    } else {
      curval = spacefactor;
      curvallevel = intval;
    }
    break;

  case setprevgraf:   /*422:*/
    if (mode == 0) {
      curval = 0;
      curvallevel = intval;
    } else {   /*:422*/
      nest[nestptr] = curlist;
      p = nestptr;
      while (abs(nest[p].modefield) != vmode)
	p--;
      curval = nest[p].pgfield;
      curvallevel = intval;
    }
    break;

  case setpageint:   /*419:*/
    if (m == 0)
      curval = deadcycles;
    else
      curval = insertpenalties;
    curvallevel = intval;
    break;
    /*:419*/

  case setpagedimen:   /*421:*/
    if (pagecontents == empty && !outputactive) {
      if (m == 0)
	curval = maxdimen;
      else
	curval = 0;
    } else
      curval = pagesofar[m];
    curvallevel = dimenval;
    break;
    /*:421*/

  case setshape:   /*423:*/
    if (parshapeptr == 0)
      curval = 0;
    else
      curval = info(parshapeptr);
    curvallevel = intval;
    break;
    /*:423*/

  case setboxdimen:   /*420:*/
    scaneightbitint();
    if (box(curval) == 0)
      curval = 0;
    else
      curval = mem[box(curval) + m - memmin].sc;
    curvallevel = dimenval;
    break;
    /*:420*/

  case chargiven:
  case mathgiven:
    curval = curchr;
    curvallevel = intval;
    break;

  case assignfontdimen:   /*425:*/
    findfontdimen(false);
    fontinfo[fmemptr].sc = 0;
    curval = fontinfo[curval].sc;
    curvallevel = dimenval;
    break;
    /*:425*/

  case assignfontint:   /*426:*/
    scanfontident();
    if (m == 0) {
      curval = get_hyphenchar(curval);
      curvallevel = intval;
    } else {
      curval = get_skewchar(curval);
      curvallevel = intval;
    }
    break;
    /*:426*/

  case register_:   /*427:*/
    scaneightbitint();
    switch (m) {

    case intval:
      curval = count(curval);
      break;

    case dimenval:
      curval = dimen(curval);
      break;

    case glueval:
      curval = skip(curval);
      break;

    case muval:
      curval = muskip(curval);
      break;
    }
    curvallevel = m;
    break;
    /*:427*/

  case lastitem:   /*424:*/
    if (curchr > glueval) {
      if (curchr == inputlinenocode)
	curval = line;
      else
	curval = lastbadness;
      curvallevel = intval;
    } else {   /*:424*/
      if (curchr == glueval)
	curval = zeroglue;
      else
	curval = 0;
      curvallevel = curchr;
      if (!ischarnode(tail) && mode != 0) {
	switch (curchr) {

	case intval:
	  if (type(tail) == penaltynode)
	    curval = penalty(tail);
	  break;

	case dimenval:
	  if (type(tail) == kernnode)
	    curval = width(tail);
	  break;

	case glueval:
	  if (type(tail) == gluenode) {
	    curval = glueptr(tail);
	    if (subtype(tail) == muglue)
	      curvallevel = muval;
	  }
	  break;
	}
      } else if (mode == vmode && tail == head) {
	switch (curchr) {

	case intval:
	  curval = lastpenalty;
	  break;

	case dimenval:
	  curval = lastkern;
	  break;

	case glueval:
	  if (lastglue != maxhalfword)
	    curval = lastglue;
	  break;
	}
      }
    }
    break;
    /*428:*/

  default:
    printnl(S(292));
    print(S(602));
    printcmdchr(curcmd, curchr);
    print(S(603));
    printesc(S(604));
    help1(S(601));
    error();
    if (level != tokval) {   /*:428*/
      curval = 0;
      curvallevel = dimenval;
    } else {
      curval = 0;
      curvallevel = intval;
    }
    break;
  }
  while (curvallevel > level) {   /*429:*/
    if (curvallevel == glueval)
      curval = width(curval);
    else if (curvallevel == muval)
      muerror();
    curvallevel--;
  }
  /*:429*/
  /*430:*/
  if (!negative) {
    if (curvallevel >= glueval && curvallevel <= muval) {
      addglueref(curval);   /*:430*/
    }
    return;
  }
  if (curvallevel < glueval) {
    curval = -curval;
    return;
  }
  curval = newspec(curval);   /*431:*/
  width(curval) = -width(curval);
  stretch(curval) = -stretch(curval);
  shrink(curval) = -shrink(curval);   /*:431*/

  /*:415*/
  /*:418*/
}
