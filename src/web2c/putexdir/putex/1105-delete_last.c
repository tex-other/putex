Static void deletelast(void)
{
  pointer p, q;
  quarterword m, FORLIM;

  if (mode == vmode && tail == head) {   /*1106:*/
    if (curchr != gluenode || lastglue != maxhalfword) {
      youcant();
      help2(S(854),S(869));
      if (curchr == kernnode)
	helpline[0] = S(870);
      else if (curchr != gluenode)
	helpline[0] = S(871);
      error();
    }
  }  /*:1106*/
  else {
    if (!ischarnode(tail)) {
      if (type(tail) == curchr) {
	q = head;
	do {
	  p = q;
	  if (!ischarnode(q)) {
	    if (type(q) == discnode) {
	      FORLIM = replacecount(q);
	      for (m = 1; m <= FORLIM; m++)
		p = link(p);
	      if (p == tail)
		goto _Lexit;
	    }
	  }
	  q = link(p);
	} while (q != tail);
	link(p) = 0;
	flushnodelist(tail);
	tail = p;
      }
    }
  }
_Lexit: ;
}
