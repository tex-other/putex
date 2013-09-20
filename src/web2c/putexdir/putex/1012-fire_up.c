Static void fireup(halfword c)
{  /*1013:*/
  pointer p, q, r, s, prevp, savesplittopskip;
  uchar n;
  boolean wait;
  long savevbadness;
  scaled savevfuzz;

  if (type(bestpagebreak) == penaltynode) {
    geqworddefine(intbase + outputpenaltycode, penalty(bestpagebreak));
    penalty(bestpagebreak) = infpenalty;
  } else   /*:1013*/
    geqworddefine(intbase + outputpenaltycode, infpenalty);
  if (botmark != 0) {   /*1014:*/
    if (topmark != 0)
      deletetokenref(topmark);
    topmark = botmark;
    addtokenref(topmark);
    deletetokenref(firstmark);
    firstmark = 0;
  }
  if (c == bestpagebreak)   /*1015:*/
    bestpagebreak = 0;
  if (box(255) != 0) {   /*:1015*/
    printnl(S(292));
    print(S(385));
    printesc(S(464));
    print(S(810));
    help2(S(811),S(809));
    boxerror(255);
  }
  insertpenalties = 0;
  savesplittopskip = splittopskip;
  if (holdinginserts <= 0) {   /*1018:*/
    r = link(pageinshead);
    while (r != pageinshead) {
      if (bestinsptr(r) != 0) {
	n = subtype(r) - minquarterword;
	ensurevbox(n);
	if (box(n) == 0)
	  box(n) = newnullbox();
	p = box(n) + listoffset;
	while (link(p) != 0)
	  p = link(p);
	lastinsptr(r) = p;
      }
      r = link(r);
    }
  }
  /*:1018*/
  q = holdhead;
  link(q) = 0;
  prevp = pagehead;
  p = link(prevp);
  while (p != bestpagebreak) {
    if (type(p) == insnode) {
      if (holdinginserts <= 0) {   /*1020:*/
	r = link(pageinshead);
	while (subtype(r) != subtype(p))
	  r = link(r);
	if (bestinsptr(r) == 0)   /*1022:*/
	  wait = true;
	else {
	  wait = false;
	  s = lastinsptr(r);
	  link(s) = insptr(p);
	  if (bestinsptr(r) == p) {   /*1021:*/
	    if (type(r) == splitup) {
	      if ((brokenins(r) == p) & (brokenptr(r) != 0)) {
		while (link(s) != brokenptr(r))
		  s = link(s);
		link(s) = 0;
		splittopskip = splittopptr(p);
		insptr(p) = prunepagetop(brokenptr(r));
		if (insptr(p) != 0) {
		  tempptr = vpack(insptr(p), 0, additional);
		  height(p) = height(tempptr) + depth(tempptr);
		  freenode(tempptr, boxnodesize);
		  wait = true;
		}
	      }
	    }
	    bestinsptr(r) = 0;
	    n = subtype(r) - minquarterword;
	    tempptr = listptr(box(n));
	    freenode(box(n), boxnodesize);
	    box(n) = vpack(tempptr, 0, additional);
	  }  /*:1021*/
	  else {
	    while (link(s) != 0)
	      s = link(s);
	    lastinsptr(r) = s;
	  }
	}
	link(prevp) = link(p);
	link(p) = 0;
	if (wait) {
	  link(q) = p;
	  q = p;
	  insertpenalties++;
	} else {
	  deleteglueref(splittopptr(p));
	  freenode(p, insnodesize);
	}
	p = prevp;   /*:1022*/
      }
      /*:1020*/
    } else if (type(p) == marknode) {
      if (firstmark == 0) {
	firstmark = markptr(p);
	addtokenref(firstmark);
      }
      if (botmark != 0)
	deletetokenref(botmark);
      botmark = markptr(p);
      addtokenref(botmark);
    }
    prevp = p;
    p = link(prevp);
  }
  splittopskip = savesplittopskip;   /*1017:*/
  if (p != 0) {
    if (link(contribhead) == 0) {
      if (nestptr == 0)
	tail = pagetail;
      else
	contribtail = pagetail;
    }
    link(pagetail) = link(contribhead);
    link(contribhead) = p;
    link(prevp) = 0;
  }
  savevbadness = vbadness;
  vbadness = infbad;
  savevfuzz = vfuzz;
  vfuzz = maxdimen;
  box(255) = vpackage(link(pagehead), bestsize, exactly, pagemaxdepth);
  vbadness = savevbadness;
  vfuzz = savevfuzz;
  if (lastglue != maxhalfword)   /*991:*/
    deleteglueref(lastglue);
  pagecontents = empty;
  pagetail = pagehead;
  link(pagehead) = 0;
  lastglue = maxhalfword;
  lastpenalty = 0;
  lastkern = 0;
  pagedepth = 0;
  pagemaxdepth = 0;   /*:991*/
  if (q != holdhead) {   /*:1017*/
    link(pagehead) = link(holdhead);
    pagetail = q;
  }
  /*1019:*/
  r = link(pageinshead);
  while (r != pageinshead) {
    q = link(r);
    freenode(r, pageinsnodesize);
    r = q;
  }
  link(pageinshead) = pageinshead;   /*:1019*/
  /*:1014*/
  if (topmark != 0 && firstmark == 0) {
    firstmark = topmark;
    addtokenref(topmark);
  }
  if (outputroutine != 0) {   /*1023:*/
    if (deadcycles < maxdeadcycles) {   /*1024:*/
      outputactive = true;
      deadcycles++;
      pushnest();
      mode = -vmode;
      prevdepth = ignoredepth;
      modeline = -line;
      begintokenlist(outputroutine, outputtext);
      newsavelevel(outputgroup);
      normalparagraph();
      scanleftbrace();
      goto _Lexit;
    }
    /*:1024*/
    printnl(S(292));
    print(S(812));
    printint(deadcycles);
    print(S(813));
    help3(S(814),S(815),S(816));
    error();
  }
  if (link(pagehead) != 0) {
    if (link(contribhead) == 0) {
      if (nestptr == 0)
	tail = pagetail;
      else
	contribtail = pagetail;
    } else
      link(pagetail) = link(contribhead);
    link(contribhead) = link(pagehead);
    link(pagehead) = 0;
    pagetail = pagehead;
  }
  shipout(box(255));
  box(255) = 0;   /*:1023*/
_Lexit: ;

  /*1016:*/
  /*:1016*/
  /*1025:*/
  /*:1025*/
}
