Static void showactivities(void)
{
  pointer q, r;
  long t;
  short TEMP;

  nest[nestptr] = curlist;
  printnl(S(385));
  println();
  for (TEMP = nestptr; TEMP >= 0; TEMP--) {
    int p = TEMP;
    short m = nest[p].modefield;
    memoryword a = nest[p].auxfield;
    printnl(S(439));
    printmode(m);
    print(S(440));
    printint(labs(nest[p].mlfield));
    if (m == hmode) {
      if (nest[p].pgfield != 8585216L) {
	print(S(441));
/* p2c: tex1.p: Note: Using % for possibly-negative arguments [317] */
	printint(nest[p].pgfield % 65536L);
	print(S(442));
	printint(nest[p].pgfield / 4194304L);
	printchar(',');
	printint((nest[p].pgfield / 65536L) & 63);
	printchar(')');
      }
    }
    if (nest[p].mlfield < 0)
      print(S(443));
    if (p == 0) {  /*986:*/
      if (pagehead != pagetail) {   /*:986*/
	printnl(S(444));
	if (outputactive)
	  print(S(445));
	showbox(link(pagehead));
	if (pagecontents > empty) {
	  printnl(S(446));
	  printtotals();
	  printnl(S(447));
	  printscaled(pagegoal);
	  r = link(pageinshead);
	  while (r != pageinshead) {
	    println();
	    printesc(S(374));
	    t = subtype(r) - minquarterword;
	    printint(t);
	    print(S(448));
	    t = xovern(height(r), 1000) * count(t);
	    printscaled(t);
	    if (type(r) == splitup) {
	      q = pagehead;
	      t = 0;
	      do {
		q = link(q);
		if ((type(q) == insnode) & (subtype(q) == subtype(r)))
		  t++;
	      } while (q != brokenins(r));
	      print(S(449));
	      printint(t);
	      print(S(450));
	    }
	    r = link(r);
	  }
	}
      }
      if (link(contribhead) != 0)
	printnl(S(451));
    }
    showbox(link(nest[p].headfield));   /*219:*/
    switch (abs(m) / (maxcommand + 1)) {   /*:219*/

    case 0:
      printnl(S(452));
      if (a.sc <= ignoredepth)
	print(S(453));
      else
	printscaled(a.sc);
      if (nest[p].pgfield != 0) {
	print(S(454));
	printint(nest[p].pgfield);
	print(S(455));
	if (nest[p].pgfield != 1)
	  printchar('s');
      }
      break;

    case 1:
      printnl(S(456));
      printint(a.hh.UU.lh);
      if (m > 0) {
	if (a.hh.rh > 0) {
	  print(S(457));
	  printint(a.hh.rh);
	}
      }
      break;

    case 2:
      if (a.int_ != 0) {
	print(S(458));
	showbox(a.int_);
      }
      break;
    }
  }
}
