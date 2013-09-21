Static void trybreak(long pi, smallnumber breaktype)
{  /*831:*/
  pointer r, prevr;
  halfword oldl;
  boolean nobreakyet;
  /*830:*/
  pointer prevprevr=0 /* XXXXX */, s, q, v, savelink;
  long t;
  internalfontnumber f;
  halfword l;
  boolean noderstaysactive;
  scaled linewidth=0 /* XXXX */, shortfall;   /*:830*/
  char fitclass;
  halfword b;
  long d;
  boolean artificialdemerits;

  if (labs(pi) >= infpenalty) {
    if (pi > 0)
      goto _Lexit;
    pi = ejectpenalty;
    /*:831*/
  }
  nobreakyet = true;
  prevr = active;
  oldl = 0;
  copytocuractive(1);
  copytocuractive(2);
  copytocuractive(3);
  copytocuractive(4);
  copytocuractive(5);
  copytocuractive(6);
  while (true) {
_Llabcontinue:
    r = link(prevr);   /*832:*/
    if (type(r) == deltanode) {   /*:832*/
      updatewidth(1);
      updatewidth(2);
      updatewidth(3);
      updatewidth(4);
      updatewidth(5);
      updatewidth(6);
      prevprevr = prevr;
      prevr = r;
      goto _Llabcontinue;
    }
    /*835:*/
    l = linenumber(r);
    if (l > oldl) {   /*:835*/
      if (minimumdemerits < awfulbad && (oldl != easyline || r == lastactive))
      {   /*836:*/
	if (nobreakyet) {   /*837:*/
	  nobreakyet = false;
	  setbreakwidthtobackground(1);
	  setbreakwidthtobackground(2);
	  setbreakwidthtobackground(3);
	  setbreakwidthtobackground(4);
	  setbreakwidthtobackground(5);
	  setbreakwidthtobackground(6);
	  s = curp;
	  if (breaktype > unhyphenated) {
	    if (curp != 0) {   /*840:*/
	      t = replacecount(curp);
	      v = curp;
	      s = postbreak(curp);
	      while (t > 0) {
		t--;
		v = link(v);   /*841:*/
		if (ischarnode(v)) {
		  f = font(v);
		  breakwidth[0] -= charwidth(f,
		      charinfo(f, character(v)));
		  continue;
		}
		switch (type(v)) {   /*:841*/

		case ligaturenode:
		  f = font_ligchar(v);
		  breakwidth[0] -= charwidth(f,
		      charinfo(f, character_ligchar(v)));
		  break;

		case hlistnode:
		case vlistnode:
		case rulenode:
		case kernnode:
		  breakwidth[0] -= width(v);
		  break;

		default:
		  confusion(S(754));
		  break;
		}
	      }
	      while (s != 0) {   /*842:*/
		if (ischarnode(s)) {
		  f = font(s);
		  breakwidth[0] += charwidth(f,
		      charinfo(f, character(s)));
		} else {
		  switch (type(s)) {   /*:842*/

		  case ligaturenode:
		    f = font_ligchar(s);
		    breakwidth[0] += charwidth(f,
			charinfo(f, character_ligchar(s)));
		    break;

		  case hlistnode:
		  case vlistnode:
		  case rulenode:
		  case kernnode:
		    breakwidth[0] += width(s);
		    break;

		  default:
		    confusion(S(755));
		    break;
		  }
		}
		s = link(s);
	      }
	      breakwidth[0] += discwidth;
	      if (postbreak(curp) == 0)
		s = link(v);
	    }
	    /*:840*/
	  }
	  while (s != 0) {
	    if (ischarnode(s))
	      goto _Ldone;
	    switch (type(s)) {

	    case gluenode:   /*838:*/
	      v = glueptr(s);
	      breakwidth[0] -= width(v);
	      breakwidth[stretchorder(v) + 1] -= stretch(v);
	      breakwidth[5] -= shrink(v);
	      break;
	      /*:838*/

	    case penaltynode:
	      /* blank case */
	      break;

	    case mathnode:
	      breakwidth[0] -= width(s);
	      break;

	    case kernnode:
	      if (subtype(s) != explicit)
		goto _Ldone;
	      breakwidth[0] -= width(s);
	      break;

	    default:
	      goto _Ldone;
	      break;
	    }
	    s = link(s);
	  }
_Ldone: ;
	}
	/*:837*/
	/*843:*/
	if (type(prevr) == deltanode) {
	  converttobreakwidth(1);
	  converttobreakwidth(2);
	  converttobreakwidth(3);
	  converttobreakwidth(4);
	  converttobreakwidth(5);
	  converttobreakwidth(6);
	} else if (prevr == active) {
	  storebreakwidth(1);
	  storebreakwidth(2);
	  storebreakwidth(3);
	  storebreakwidth(4);
	  storebreakwidth(5);
	  storebreakwidth(6);
	} else {
	  q = getnode(deltanodesize);
	  link(q) = r;
	  type(q) = deltanode;
	  subtype(q) = 0;
	  newdeltatobreakwidth(1);
	  newdeltatobreakwidth(2);
	  newdeltatobreakwidth(3);
	  newdeltatobreakwidth(4);
	  newdeltatobreakwidth(5);
	  newdeltatobreakwidth(6);
	  link(prevr) = q;
	  prevprevr = prevr;
	  prevr = q;
	}
	if (labs(adjdemerits) >= awfulbad - minimumdemerits)
	  minimumdemerits = awfulbad - 1;
	else
	  minimumdemerits += labs(adjdemerits);
	for (fitclass = veryloosefit; fitclass <= tightfit; fitclass++) {
	  if (minimaldemerits[fitclass - veryloosefit] <= minimumdemerits)
	  {   /*845:*/
	    q = getnode(passivenodesize);
	    link(q) = passive;
	    passive = q;
	    curbreak(q) = curp;   /*_STAT*/
	    passnumber++;
	    serial(q) = passnumber;   /*_ENDSTAT*/
	    prevbreak(q) = bestplace[fitclass - veryloosefit];
	    q = getnode(activenodesize);
	    breaknode(q) = passive;
	    linenumber(q) = bestplline[fitclass - veryloosefit] + 1;
	    fitness(q) = fitclass;
	    type(q) = breaktype;
	    totaldemerits(q) = minimaldemerits[fitclass - veryloosefit];
	    link(q) = r;
	    link(prevr) = q;
	    prevr = q;   /*_STAT*/
	    if (tracingparagraphs > 0) {   /*846:*/
	      printnl(S(756));
	      printint(serial(passive));
	      print(S(757));
	      printint(linenumber(q) - 1);
	      printchar('.');
	      printint(fitclass);
	      if (breaktype == hyphenated)
		printchar('-');
	      print(S(758));
	      printint(totaldemerits(q));
	      print(S(759));
	      if (prevbreak(passive) == 0)
		printchar('0');
	      else
		printint(serial(prevbreak(passive)));
	    }
	    /*:846*/
	    /*_ENDSTAT*/
	  }
	  /*:845*/
	  minimaldemerits[fitclass - veryloosefit] = awfulbad;
	}
	minimumdemerits = awfulbad;   /*844:*/
	if (r != lastactive) {   /*:844*/
	  q = getnode(deltanodesize);
	  link(q) = r;
	  type(q) = deltanode;
	  subtype(q) = 0;
	  newdeltafrombreakwidth(1);
	  newdeltafrombreakwidth(2);
	  newdeltafrombreakwidth(3);
	  newdeltafrombreakwidth(4);
	  newdeltafrombreakwidth(5);
	  newdeltafrombreakwidth(6);
	  link(prevr) = q;
	  prevprevr = prevr;
	  prevr = q;
	}
      }
      /*:836*/
      if (r == lastactive)   /*850:*/
	goto _Lexit;
      if (l > easyline) {
	linewidth = secondwidth;
	oldl = maxhalfword - 1;
      } else {   /*:850*/
	oldl = l;
	if (l > lastspecialline)
	  linewidth = secondwidth;
	else if (parshapeptr == 0)
	  linewidth = firstwidth;
	else
	  linewidth = mem[parshapeptr + l * 2 - memmin].sc;
      }
    }
    /*851:*/
    artificialdemerits = false;
    shortfall = linewidth - curactivewidth[0];
    if (shortfall > 0) {   /*852:*/
      if (curactivewidth[2] != 0 || curactivewidth[3] != 0 ||
	  curactivewidth[4] != 0) {
	b = 0;
	fitclass = decentfit;
      } else {   /*:852*/
	if (shortfall > 7230584L) {
	  if (curactivewidth[1] < 1663497L) {
	    b = infbad;
	    fitclass = veryloosefit;
	    goto _Ldone1;
	  }
	}
	b = badness(shortfall, curactivewidth[1]);
	if (b > 12) {
	  if (b > 99)
	    fitclass = veryloosefit;
	  else
	    fitclass = loosefit;
	} else
	  fitclass = decentfit;
_Ldone1: ;
      }
    } else  /*853:*/
    {   /*:853*/
      if (-shortfall > curactivewidth[5])
	b = infbad + 1;
      else
	b = badness(-shortfall, curactivewidth[5]);
      if (b > 12)
	fitclass = tightfit;
      else
	fitclass = decentfit;
    }
    if (b > infbad || pi == ejectpenalty) {   /*854:*/
      if (((finalpass && minimumdemerits == awfulbad) &
	   (link(r) == lastactive)) && prevr == active)
	artificialdemerits = true;
      else if (b > threshold)
	goto _Ldeactivate_;
      noderstaysactive = false;
    }  /*:854*/
    else {
      prevr = r;
      if (b > threshold)
	goto _Llabcontinue;
      noderstaysactive = true;
    }  /*855:*/
    if (artificialdemerits)
      d = 0;
    else {   /*859:*/
      d = linepenalty + b;
      if (labs(d) >= 10000)
	d = 100000000L;
      else
	d *= d;
      if (pi != 0) {
	if (pi > 0)
	  d += pi * pi;
	else if (pi > ejectpenalty)
	  d -= pi * pi;
      }
      if ((breaktype == hyphenated) & (type(r) == hyphenated)) {
	if (curp != 0)
	  d += doublehyphendemerits;
	else
	  d += finalhyphendemerits;
      }
      if (labs(fitclass - fitness(r)) > 1)
	d += adjdemerits;
    }
    /*_STAT*/
    if (tracingparagraphs > 0) {   /*:856*/
      /*856:*/
      if (printednode != curp) {   /*857:*/
	printnl(S(385));
	if (curp == 0)
	  shortdisplay(link(printednode));
	else {
	  savelink = link(curp);
	  link(curp) = 0;
	  printnl(S(385));
	  shortdisplay(link(printednode));
	  link(curp) = savelink;
	}
	printednode = curp;
      }
      /*:857*/
      printnl('@');
      if (curp == 0)
	printesc(S(760));
      else if (type(curp) != gluenode) {
	if (type(curp) == penaltynode)
	  printesc(S(761));
	else if (type(curp) == discnode)
	  printesc(S(400));
	else if (type(curp) == kernnode)
	  printesc(S(391));
	else
	  printesc(S(394));
      }
      print(S(762));
      if (breaknode(r) == 0)
	printchar('0');
      else
	printint(serial(breaknode(r)));
      print(S(763));
      if (b > infbad)
	printchar('*');
      else
	printint(b);
      print(S(764));
      printint(pi);
      print(S(765));
      if (artificialdemerits)
	printchar('*');
      else
	printint(d);
    }
    /*_ENDSTAT*/
    d += totaldemerits(r);
    if (d <= minimaldemerits[fitclass - veryloosefit]) {   /*:855*/
      minimaldemerits[fitclass - veryloosefit] = d;
      bestplace[fitclass - veryloosefit] = breaknode(r);
      bestplline[fitclass - veryloosefit] = l;
      if (d < minimumdemerits)
	minimumdemerits = d;
    }
    if (noderstaysactive)
      goto _Llabcontinue;
_Ldeactivate_:   /*860:*/
    link(prevr) = link(r);
    freenode(r, activenodesize);
    if (prevr == active) {   /*861:*/
      r = link(active);
      if (type(r) != deltanode)
	continue;
      updateactive(1);
      updateactive(2);
      updateactive(3);
      updateactive(4);
      updateactive(5);
      updateactive(6);
      copytocuractive(1);
      copytocuractive(2);
      copytocuractive(3);
      copytocuractive(4);
      copytocuractive(5);
      copytocuractive(6);
      link(active) = link(r);
      freenode(r, deltanodesize);
      continue;
    }  /*:861*/
    /*:851*/
    if (type(prevr) != deltanode)   /*:860*/
      continue;
    r = link(prevr);
    if (r == lastactive) {
      downdatewidth(1);
      downdatewidth(2);
      downdatewidth(3);
      downdatewidth(4);
      downdatewidth(5);
      downdatewidth(6);
      link(prevprevr) = lastactive;
      freenode(prevr, deltanodesize);
      prevr = prevprevr;
      continue;
    }
    if (type(r) != deltanode)
      continue;
    updatewidth(1);
    updatewidth(2);
    updatewidth(3);
    updatewidth(4);
    updatewidth(5);
    updatewidth(6);
    combinetwodeltas(1);
    combinetwodeltas(2);
    combinetwodeltas(3);
    combinetwodeltas(4);
    combinetwodeltas(5);
    combinetwodeltas(6);
    link(prevr) = link(r);
    freenode(r, deltanodesize);
  }
_Lexit:   /*_STAT*/
  /*858:*/
  if (curp != printednode)
    return;
  if (curp == 0)
    return;
  if (type(curp) != discnode)   /*:858*/
    return;
  /*_ENDSTAT*/
  t = replacecount(curp);
  while (t > 0) {
    t--;
    printednode = link(printednode);
  }

  /*:843*/
  /*:859*/
}
