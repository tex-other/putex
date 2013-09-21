Static void searchmem(halfword p)
{
  long q;

  for (q = memmin; q <= lomemmax; q++) {
    if (link(q) == p) {
      printnl(S(323));
      printint(q);
      printchar(')');
    }
    if (info(q) == p) {
      printnl(S(324));
      printint(q);
      printchar(')');
    }
  }
  for (q = himemmin; q <= memend; q++) {
    if (link(q) == p) {
      printnl(S(323));
      printint(q);
      printchar(')');
    }
    if (info(q) == p) {
      printnl(S(324));
      printint(q);
      printchar(')');
    }
  }  /*255:*/
  for (q = activebase; q <= boxbase + 255; q++) {   /*:255*/
    if (equiv(q) == p) {
      printnl(S(325));
      printint(q);
      printchar(')');
    }
  }
  /*285:*/
  if (saveptr > 0) {   /*933:*/
    for (q = 0; q < saveptr; q++) {   /*:285*/
      if (equivfield(savestack[q]) == p) {
	printnl(S(326));
	printint(q);
	printchar(')');
      }
    }
  }
  for (q = 0; q <= hyphsize; q++) {   /*:933*/
    if (hyphlist[q] == p) {
      printnl(S(327));
      printint(q);
      printchar(')');
    }
  }
}
