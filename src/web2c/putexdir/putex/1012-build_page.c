Static void buildpage(void)
{
  pointer p, q, r;
  long b, c, pi=0 /* XXXX */;
  uchar n;
  scaled delta, h, w;

  if (link(contribhead) == 0 || outputactive)
    goto _Lexit;
  do {
_Llabcontinue:
    p = link(contribhead);   /*996:*/
    if (lastglue != maxhalfword)
      deleteglueref(lastglue);
    lastpenalty = 0;
    lastkern = 0;
    if (type(p) == gluenode) {   /*997:*/
      lastglue = glueptr(p);
      addglueref(lastglue);
    } else {   /*:996*/
      lastglue = maxhalfword;
      if (type(p) == penaltynode)
	lastpenalty = penalty(p);
      else if (type(p) == kernnode)
	lastkern = width(p);
    }
    /*1000:*/
    switch (type(p)) {   /*:1000*/

    case hlistnode:
    case vlistnode:
    case rulenode:
      if (pagecontents < boxthere) {   /*1001:*/
	if (pagecontents == empty)
	  freezepagespecs(boxthere);
	else
	  pagecontents = boxthere;
	q = newskipparam(topskipcode);
	if (width(tempptr) > height(p))
	  width(tempptr) -= height(p);
	else
	  width(tempptr) = 0;
	link(q) = p;
	link(contribhead) = q;
	goto _Llabcontinue;
      } else  /*1002:*/
      {   /*:1002*/
	pagetotal += pagedepth + height(p);
	pagedepth = depth(p);
	goto _Lcontribute_;
      }
      /*:1001*/
      break;

    case whatsitnode:   /*1364:*/
      goto _Lcontribute_;   /*:1364*/
      break;

    case gluenode:
      if (pagecontents < boxthere)
	goto _Ldone1;
      if (!precedesbreak(pagetail))
	goto _Lupdateheights_;
      pi = 0;
      break;

    case kernnode:
      if (pagecontents < boxthere)
	goto _Ldone1;
      if (link(p) == 0)
	goto _Lexit;
      if (type(link(p)) != gluenode)
	goto _Lupdateheights_;
      pi = 0;
      break;

    case penaltynode:
      if (pagecontents < boxthere)
	goto _Ldone1;
      pi = penalty(p);
      break;

    case marknode:
      goto _Lcontribute_;
      break;

    case insnode:   /*1008:*/
      if (pagecontents == empty)
	freezepagespecs(insertsonly);
      n = subtype(p);
      r = pageinshead;
      while (n >= subtype(link(r)))
	r = link(r);
      n -= minquarterword;
      if (subtype(r) != n) {   /*1009:*/
	q = getnode(pageinsnodesize);
	link(q) = link(r);
	link(r) = q;
	r = q;
	subtype(r) = n;
	type(r) = inserting;
	ensurevbox(n);
	if (box(n) == 0)
	  height(r) = 0;
	else
	  height(r) = height(box(n)) + depth(box(n));
	bestinsptr(r) = 0;
	q = skip(n);
	if (count(n) == 1000)
	  h = height(r);
	else
	  h = xovern(height(r), 1000) * count(n);
	pagegoal += -h - width(q);
	pagesofar[stretchorder(q) + 2] += stretch(q);
	pageshrink += shrink(q);
	if ((shrinkorder(q) != normal) & (shrink(q) != 0)) {
	  printnl(S(292));
	  print(S(817));
	  printesc(S(460));
	  printint(n);
	  help3(S(818),S(819),S(753));
	  error();
	}
      }
      /*:1009*/
      if (type(r) == splitup)
	insertpenalties += floatcost(p);
      else {
	lastinsptr(r) = p;
	delta = pagegoal - pagetotal - pagedepth + pageshrink;
	if (count(n) == 1000)
	  h = height(p);
	else
	  h = xovern(height(p), 1000) * count(n);
	if ((h <= 0 || h <= delta) & (height(p) + height(r) <=
				      dimen(n))) {
	  pagegoal -= h;
	  height(r) += height(p);
	} else  /*1010:*/
	{   /*:1010*/
	  if (count(n) <= 0)
	    w = maxdimen;
	  else {
	    w = pagegoal - pagetotal - pagedepth;
	    if (count(n) != 1000)
	      w = xovern(w, count(n)) * 1000;
	  }
	  if (w > dimen(n) - height(r))
	    w = dimen(n) - height(r);
	  q = vertbreak(insptr(p), w, depth(p));
	  height(r) += bestheightplusdepth;   /*_STAT*/
	  if (tracingpages > 0) {   /*1011:*/
	    begindiagnostic();
	    printnl(S(820));
	    printint(n);
	    print(S(821));
	    printscaled(w);
	    printchar(',');
	    printscaled(bestheightplusdepth);
	    print(S(764));
	    if (q == 0)
	      printint(ejectpenalty);
	    else if (type(q) == penaltynode)
	      printint(penalty(q));
	    else
	      printchar('0');
	    enddiagnostic(false);
	  }
	  /*:1011*/
	  /*_ENDSTAT*/
	  if (count(n) != 1000)
	    bestheightplusdepth = xovern(bestheightplusdepth, 1000) * count(n);
	  pagegoal -= bestheightplusdepth;
	  type(r) = splitup;
	  brokenptr(r) = q;
	  brokenins(r) = p;
	  if (q == 0)
	    insertpenalties += ejectpenalty;
	  else if (type(q) == penaltynode)
	    insertpenalties += penalty(q);
	}
      }
      goto _Lcontribute_;
      break;
      /*:1008*/

    default:
      confusion(S(822));
      break;
    }
    /*1005:*/
    if (pi < infpenalty)   /*:1005*/
    {  /*1007:*/
      if (pagetotal < pagegoal) {
	if (pagesofar[3] != 0 || pagesofar[4] != 0 || pagesofar[5] != 0)
	  b = 0;
	else
	  b = badness(pagegoal - pagetotal, pagesofar[2]);
      } else if (pagetotal - pagegoal > pageshrink)
	b = awfulbad;
      else
	b = badness(pagetotal - pagegoal, pageshrink);   /*:1007*/
      if (b < awfulbad) {
	if (pi <= ejectpenalty)
	  c = pi;
	else if (b < infbad)
	  c = b + pi + insertpenalties;
	else
	  c = deplorable;
      } else
	c = b;
      if (insertpenalties >= 10000)   /*_STAT*/
	c = awfulbad;
      if (tracingpages > 0) {   /*1006:*/
	begindiagnostic();
	printnl('%');
	print(S(758));
	printtotals();
	print(S(823));
	printscaled(pagegoal);
	print(S(763));
	if (b == awfulbad)
	  printchar('*');
	else
	  printint(b);
	print(S(764));
	printint(pi);
	print(S(824));
	if (c == awfulbad)
	  printchar('*');
	else
	  printint(c);
	if (c <= leastpagecost)
	  printchar('#');
	enddiagnostic(false);
      }
      /*:1006*/
      /*_ENDSTAT*/
      if (c <= leastpagecost) {
	bestpagebreak = p;
	bestsize = pagegoal;
	leastpagecost = c;
	r = link(pageinshead);
	while (r != pageinshead) {
	  bestinsptr(r) = lastinsptr(r);
	  r = link(r);
	}
      }
      if (c == awfulbad || pi <= ejectpenalty) {
	fireup(p);
	if (outputactive)
	  goto _Lexit;
	goto _Ldone;
      }
    }
    if ((type(p) < gluenode) | (type(p) > kernnode))
      goto _Lcontribute_;
_Lupdateheights_:   /*1004:*/
    if (type(p) == kernnode)
      q = p;
    else {
      q = glueptr(p);
      pagesofar[stretchorder(q) + 2] += stretch(q);
      pageshrink += shrink(q);
      if ((shrinkorder(q) != normal) & (shrink(q) != 0)) {
	printnl(S(292));
	print(S(825));
	help4(S(826),S(795),S(796),S(753));
	error();
	r = newspec(q);
	shrinkorder(r) = normal;
	deleteglueref(q);
	glueptr(p) = r;
	q = r;
      }
    }
    pagetotal += pagedepth + width(q);
    pagedepth = 0;   /*:1004*/
_Lcontribute_:   /*1003:*/
    if (pagedepth > pagemaxdepth) {   /*:1003*/
      pagetotal += pagedepth - pagemaxdepth;
      pagedepth = pagemaxdepth;
    }
    /*998:*/
    link(pagetail) = p;
    pagetail = p;
    link(contribhead) = link(p);
    link(p) = 0;
    goto _Ldone;   /*:998*/
_Ldone1:   /*999:*/
    link(contribhead) = link(p);
    link(p) = 0;   /*:999*/
    flushnodelist(p);
_Ldone: ;   /*:997*/
  } while (link(contribhead) != 0);   /*995:*/
  if (nestptr == 0)
    tail = contribhead;
  else
    contribtail = contribhead;   /*:995*/
_Lexit: ;
}
