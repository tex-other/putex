Static void beginbox(long boxcontext)
{
  pointer p, q;
  quarterword m;
  halfword k;
  eightbits n;
  quarterword FORLIM;

  switch (curchr) {

  case boxcode:
    scaneightbitint();
    curbox = box(curval);
    box(curval) = 0;
    break;

  case copycode:
    scaneightbitint();
    curbox = copynodelist(box(curval));
    break;

  case lastboxcode:   /*1080:*/
    curbox = 0;
    if (labs(mode) == mmode) {
      youcant();
      help1(S(853));
      error();
    } else if (mode == vmode && head == tail) {
      youcant();
      help2(S(854),S(855));
      error();
    } else {
      if (!ischarnode(tail)) {
	if ((type(tail) == hlistnode) | (type(tail) == vlistnode))
	{   /*1081:*/
	  q = head;
	  do {
	    p = q;
	    if (!ischarnode(q)) {
	      if (type(q) == discnode) {
		FORLIM = replacecount(q);
		for (m = 1; m <= FORLIM; m++)
		  p = link(p);
		if (p == tail)
		  goto _Ldone;
	      }
	    }
	    q = link(p);
	  } while (q != tail);
	  curbox = tail;
	  shiftamount(curbox) = 0;
	  tail = p;
	  link(p) = 0;
_Ldone: ;
	}
	/*:1081*/
      }
    }
    break;
    /*:1080*/

  case vsplitcode:   /*1082:*/
    scaneightbitint();
    n = curval;
    if (!scankeyword(S(697))) {
      printnl(S(292));
      print(S(856));
      help2(S(857),S(858));
      error();
    }
    scannormaldimen();
    curbox = vsplit(n, curval);
    break;
    /*:1082*/
    /*1083:*/

  default:
    k = curchr - vtopcode;
    saved(0) = boxcontext;
    if (k == hmode) {
      if (boxcontext < boxflag && labs(mode) == vmode)
	scanspec(adjustedhboxgroup, true);
      else
	scanspec(hboxgroup, true);
    } else {
      if (k == vmode)
	scanspec(vboxgroup, true);
      else {
	scanspec(vtopgroup, true);
	k = vmode;
      }
      normalparagraph();
    }
    pushnest();
    mode = -k;
    if (k == vmode) {
      prevdepth = ignoredepth;
      if (everyvbox != 0)
	begintokenlist(everyvbox, everyvboxtext);
    } else {
      spacefactor = 1000;
      if (everyhbox != 0)
	begintokenlist(everyhbox, everyhboxtext);
    }
    goto _Lexit;   /*:1083*/
    break;
  }
  boxend(boxcontext);
_Lexit: ;
}
