Static void linebreak(long finalwidowpenalty)
{
  /*862:*/
  boolean autobreaking;
  pointer prevp, q, r, s, prevs;
  internalfontnumber f;
  /*:862*/
  /*893:*/
  /* smallnumber */ int j; /* INT */
  uchar c;   /*:893*/

  packbeginline = modeline;   /*816:*/
  link(temphead) = link(head);
  if (ischarnode(tail)) {
    tailappend(newpenalty(infpenalty));
  } else if (type(tail) != gluenode) {
    tailappend(newpenalty(infpenalty));
  } else {
    type(tail) = penaltynode;
    deleteglueref(glueptr(tail));
    flushnodelist(leaderptr(tail));
    penalty(tail) = infpenalty;
  }
  link(tail) = newparamglue(parfillskipcode);
  initcurlang = prevgraf % 65536L;
/* p2c: tex1.p: Note: Using % for possibly-negative arguments [317] */
  initlhyf = prevgraf / 4194304L;
  initrhyf = (prevgraf / 65536L) & 63;
  popnest();   /*:816*/
  /*827:*/
  noshrinkerroryet = true;
  checkshrinkage(leftskip);
  checkshrinkage(rightskip);
  q = leftskip;
  r = rightskip;
  background[0] = width(q) + width(r);
  background[1] = 0;
  background[2] = 0;
  background[3] = 0;
  background[4] = 0;
  background[stretchorder(q) + 1] = stretch(q);
  background[stretchorder(r) + 1] += stretch(r);
  background[5] = shrink(q) + shrink(r);   /*:827*/
  /*834:*/
  minimumdemerits = awfulbad;
  minimaldemerits[tightfit - veryloosefit] = awfulbad;
  minimaldemerits[decentfit - veryloosefit] = awfulbad;
  minimaldemerits[loosefit - veryloosefit] = awfulbad;
  minimaldemerits[0] = awfulbad;   /*:834*/
  /*848:*/
  if (parshapeptr == 0) {
    if (hangindent == 0) {
      lastspecialline = 0;
      secondwidth = hsize;
      secondindent = 0;
    } else  /*849:*/
    {   /*:849*/
      lastspecialline = labs(hangafter);
      if (hangafter < 0) {
	firstwidth = hsize - labs(hangindent);
	if (hangindent >= 0)
	  firstindent = hangindent;
	else
	  firstindent = 0;
	secondwidth = hsize;
	secondindent = 0;
      } else {
	firstwidth = hsize;
	firstindent = 0;
	secondwidth = hsize - labs(hangindent);
	if (hangindent >= 0)
	  secondindent = hangindent;
	else
	  secondindent = 0;
      }
    }
  } else {
    lastspecialline = info(parshapeptr) - 1;
    secondwidth = mem[parshapeptr + (lastspecialline + 1) * 2 - memmin].sc;
    secondindent = mem[parshapeptr + lastspecialline * 2 - memmin + 1].sc;
  }
  if (looseness == 0)
    easyline = lastspecialline;
  else {
    easyline = maxhalfword;
    /*:848*/
  }
  /*863:*/
  threshold = pretolerance;
  if (threshold >= 0) {  /*_STAT*/
    if (tracingparagraphs > 0) {   /*_ENDSTAT*/
      begindiagnostic();
      printnl(S(777));
    }
    secondpass = false;
    finalpass = false;
  } else {
    threshold = tolerance;
    secondpass = true;
    finalpass = (emergencystretch <= 0);   /*_STAT*/
    if (tracingparagraphs > 0)   /*_ENDSTAT*/
      begindiagnostic();
  }
  while (true) {
    if (threshold > infbad)
      threshold = infbad;
    if (secondpass) {   /*891:*/
      if (trienotready)
	inittrie();
      curlang = initcurlang;
      lhyf = initlhyf;
      rhyf = initrhyf;
    }
    /*:891*/
    /*864:*/
    q = getnode(activenodesize);
    type(q) = unhyphenated;
    fitness(q) = decentfit;
    link(q) = lastactive;
    breaknode(q) = 0;
    linenumber(q) = prevgraf + 1;
    totaldemerits(q) = 0;
    link(active) = q;
    storebackground(1);
    storebackground(2);
    storebackground(3);
    storebackground(4);
    storebackground(5);
    storebackground(6);
    passive = 0;
    printednode = temphead;
    passnumber = 0;
    fontinshortdisplay = nullfont;   /*:864*/
    curp = link(temphead);
    autobreaking = true;
    prevp = curp;
    while ((curp != 0) & (link(active) != lastactive)) {   /*866:*/
      if (ischarnode(curp)) {   /*867:*/
	prevp = curp;
	do {
	  f = font(curp);
	  actwidth += charwidth(f, charinfo(f, character(curp)));
	  curp = link(curp);
	} while (ischarnode(curp));
      }
      /*:867*/
      switch (type(curp)) {

      case hlistnode:
      case vlistnode:
      case rulenode:
	actwidth += width(curp);
	break;

      case whatsitnode:   /*1362:*/
	advpast(curp);   /*:1362*/
	break;

      case gluenode:  /*868:*/
	if (autobreaking) {
	  if (ischarnode(prevp))
	    trybreak(0, unhyphenated);
	  else if (precedesbreak(prevp)) {
	    trybreak(0, unhyphenated);
	  } else if ((type(prevp) == kernnode) &
		     (subtype(prevp) != explicit))
	    trybreak(0, unhyphenated);
	}
	checkshrinkage(glueptr(curp));
	q = glueptr(curp);
	actwidth += width(q);
	activewidth[stretchorder(q) + 1] += stretch(q);
	activewidth[5] += shrink(q);   /*:868*/
	if (secondpass && autobreaking) {   /*894:*/
	  prevs = curp;
	  s = link(prevs);
	  if (s != 0) {  /*896:*/
	    while (true) {
	      if (ischarnode(s)) {
		c = character(s) - minquarterword;
		hf = font(s);
	      } else if (type(s) == ligaturenode) {
		if (ligptr(s) == 0)
		  goto _Llabcontinue;
		q = ligptr(s);
		c = character(q) - minquarterword;
		hf = font(q);
	      } else if ((type(s) == kernnode) & (subtype(s) == normal))
		goto _Llabcontinue;
	      else if (type(s) == whatsitnode) {
		advpast(s);   /*:1363*/
		goto _Llabcontinue;
	      } else
		goto _Ldone1;
	      if (lccode(c) != 0) {
		if (lccode(c) == c || uchyph > 0)
		  goto _Ldone2;
		else
		  goto _Ldone1;
	      }
_Llabcontinue:
	      prevs = s;
	      s = link(prevs);
	    }
_Ldone2:
	    hyfchar = get_hyphenchar(hf);
	    if (hyfchar < 0)
	      goto _Ldone1;
	    if (hyfchar > 255)
	      goto _Ldone1;
	    ha = prevs;   /*:896*/
	    if (lhyf + rhyf > 63)   /*897:*/
	      goto _Ldone1;
	    hn = 0;
	    while (true) {
	      if (ischarnode(s)) {
		if (font(s) != hf)
		  goto _Ldone3;
		hyfbchar = character(s);
		c = hyfbchar - minquarterword;
		if (lccode(c) == 0)
		  goto _Ldone3;
		if (hn == 63)
		  goto _Ldone3;
		hb = s;
		hn++;
		hu[hn] = c;
		hc[hn] = lccode(c);
		hyfbchar = nonchar;
	      } else if (type(s) == ligaturenode) {
		if (font_ligchar(s) != hf) {
		  goto _Ldone3;
		}
		j = hn;
		q = ligptr(s);
		if (q > 0)
		  hyfbchar = character(q);
		while (q > 0) {
		  c = character(q) - minquarterword;
		  if (lccode(c) == 0)
		    goto _Ldone3;
		  if (j == 63)
		    goto _Ldone3;
		  j++;
		  hu[j] = c;
		  hc[j] = lccode(c);
		  q = link(q);
		}
		hb = s;
		hn = j;
		if ((subtype(s)) & 1)
		  hyfbchar = fontbchar[hf ];
		else
		  hyfbchar = nonchar;
	      } else if ((type(s) == kernnode) & (subtype(s) == normal)) {
		hb = s;
		hyfbchar = fontbchar[hf ];
	      } else
		goto _Ldone3;
	      s = link(s);
	    }
_Ldone3:   /*:897*/
	    /*899:*/
	    if (hn < lhyf + rhyf)
	      goto _Ldone1;
	    while (true) {
	      if (!ischarnode(s)) {
		switch (type(s)) {

		case ligaturenode:
		  /* blank case */
		  break;

		case kernnode:
		  if (subtype(s) != normal)
		    goto _Ldone4;
		  break;

		case whatsitnode:
		case gluenode:
		case penaltynode:
		case insnode:
		case adjustnode:
		case marknode:
		  goto _Ldone4;
		  break;

		default:
		  goto _Ldone1;
		  break;
		}
	      }
	      s = link(s);
	    }
_Ldone4:   /*:899*/
	    hyphenate();
	  }
_Ldone1: ;
	}
	/*:894*/
	break;

      case kernnode:
	if (subtype(curp) == explicit) {
	  kernbreak();
	} else
	  actwidth += width(curp);
	break;

      case ligaturenode:
	f = font_ligchar(curp);
	actwidth += charwidth(f,
	    charinfo(f, character_ligchar(curp)));
	break;

      case discnode:   /*869:*/
	s = prebreak(curp);
	discwidth = 0;
	if (s == 0)
	  trybreak(exhyphenpenalty, hyphenated);
	else {
	  do {   /*870:*/
	    if (ischarnode(s)) {
	      f = font(s);
	      discwidth += charwidth(f, charinfo(f, character(s)));
	    } else {
	      switch (type(s)) {   /*:870*/

	      case ligaturenode:
		f = font_ligchar(s);
		discwidth += charwidth(f,
		    charinfo(f, character_ligchar(s)));
		break;

	      case hlistnode:
	      case vlistnode:
	      case rulenode:
	      case kernnode:
		discwidth += width(s);
		break;

	      default:
		confusion(S(778));
		break;
	      }
	    }
	    s = link(s);
	  } while (s != 0);
	  actwidth += discwidth;
	  trybreak(hyphenpenalty, hyphenated);
	  actwidth -= discwidth;
	}
	r = replacecount(curp);
	s = link(curp);
	while (r > 0) {  /*871:*/
	  if (ischarnode(s)) {
	    f = font(s);
	    actwidth += charwidth(f, charinfo(f, character(s)));
	  } else {
	    switch (type(s)) {   /*:871*/

	    case ligaturenode:
	      f = font_ligchar(s);
	      actwidth += charwidth(f,
		  charinfo(f, character_ligchar(s)));
	      break;

	    case hlistnode:
	    case vlistnode:
	    case rulenode:
	    case kernnode:
	      actwidth += width(s);
	      break;

	    default:
	      confusion(S(779));
	      break;
	    }
	  }
	  r--;
	  s = link(s);
	}
	prevp = curp;
	curp = s;
	goto _Ldone5;
	break;
	/*:869*/

      case mathnode:
	autobreaking = (subtype(curp) == after);
	kernbreak();
	break;

      case penaltynode:
	trybreak(penalty(curp), unhyphenated);
	break;

      case marknode:
      case insnode:
      case adjustnode:
	/* blank case */
	break;

      default:
	confusion(S(780));
	break;
      }
      prevp = curp;
      curp = link(curp);
_Ldone5: ;
    }
    /*:866*/
    if (curp == 0) {   /*873:*/
      trybreak(ejectpenalty, hyphenated);
      if (link(active) != lastactive) {   /*874:*/
	r = link(active);
	fewestdemerits = awfulbad;
	do {
	  if (type(r) != deltanode) {
	    if (totaldemerits(r) < fewestdemerits) {
	      fewestdemerits = totaldemerits(r);
	      bestbet = r;
	    }
	  }
	  r = link(r);
	} while (r != lastactive);
	bestline = linenumber(bestbet);   /*:874*/
	if (looseness == 0)
	  goto _Ldone;
	/*875:*/
	r = link(active);
	actuallooseness = 0;
	do {
	  if (type(r) != deltanode) {
	    linediff = linenumber(r) - bestline;
	    if ( (linediff < actuallooseness && looseness <= linediff) ||
		(linediff > actuallooseness && looseness >= linediff) ) {
	      bestbet = r;
	      actuallooseness = linediff;
	      fewestdemerits = totaldemerits(r);
	    } else if ((linediff == actuallooseness) &
		       (totaldemerits(r) < fewestdemerits)) {
	      bestbet = r;
	      fewestdemerits = totaldemerits(r);
	    }
	  }
	  r = link(r);
	} while (r != lastactive);
	bestline = linenumber(bestbet);   /*:875*/
	if (actuallooseness == looseness || finalpass)
	  goto _Ldone;
      }
    }
    /*:873*/
    /*865:*/
    q = link(active);
    while (q != lastactive) {
      curp = link(q);
      if (type(q) == deltanode)
	freenode(q, deltanodesize);
      else
	freenode(q, activenodesize);
      q = curp;
    }
    q = passive;
    while (q != 0) {   /*:865*/
      curp = link(q);
      freenode(q, passivenodesize);
      q = curp;
    }
    if (secondpass) {  /*_STAT*/
      if (tracingparagraphs > 0)   /*_ENDSTAT*/
	printnl(S(781));
      background[1] += emergencystretch;
      finalpass = true;
      continue;
    }
    if (tracingparagraphs > 0)   /*_ENDSTAT*/
      printnl(S(782));
    threshold = tolerance;
    secondpass = true;
    finalpass = (emergencystretch <= 0);
  }
_Ldone:   /*_STAT*/
  if (tracingparagraphs > 0) {   /*_ENDSTAT*/
    enddiagnostic(true);
    normalizeselector();
  }
  /*:863*/
  /*876:*/
  /*:876*/
  postlinebreak(finalwidowpenalty);   /*865:*/
  q = link(active);
  while (q != lastactive) {
    curp = link(q);
    if (type(q) == deltanode)
      freenode(q, deltanodesize);
    else
      freenode(q, activenodesize);
    q = curp;
  }
  q = passive;
  while (q != 0) {   /*:865*/
    curp = link(q);
    freenode(q, passivenodesize);
    q = curp;
  }
  packbeginline = 0;

  /*1363:*/
  /*898:*/
  /*:898*/
  /*_STAT*/
}
