Static void boxend(long boxcontext)
{
  pointer p;

  if (boxcontext < boxflag) {   /*1076:*/
    if (curbox == 0)
      return;
    shiftamount(curbox) = boxcontext;
    if (labs(mode) == vmode) {
      appendtovlist(curbox);
      if (adjusttail != 0) {
	if (adjusthead != adjusttail) {
	  link(tail) = link(adjusthead);
	  tail = adjusttail;
	}
	adjusttail = 0;
      }
      if (mode > 0)
	buildpage();
      return;
    }
    if (labs(mode) == hmode)
      spacefactor = 1000;
    else {
      p = newnoad();
      mathtype(nucleus(p)) = subbox;
      info(nucleus(p)) = curbox;
      curbox = p;
    }
    link(tail) = curbox;
    tail = curbox;
    return;
  }  /*:1076*/
  if (boxcontext < shipoutflag) {   /*1077:*/
    if (boxcontext < boxflag + 256)
      eqdefine(boxbase - boxflag + boxcontext, boxref, curbox);
    else   /*:1077*/
      geqdefine(boxbase - boxflag + boxcontext - 256, boxref, curbox);
    return;
  }
  if (curbox == 0)
    return;
  if (boxcontext <= shipoutflag)   /*1078:*/
  {  /*404:*/
    shipout(curbox);
    return;
  }
  /*:1078*/
  do {
    getxtoken();   /*:404*/
  } while (curcmd == spacer || curcmd == relax);
  if ( (curcmd == hskip && labs(mode) != vmode) ||
      (curcmd == vskip && labs(mode) == vmode) ||
      (curcmd == mskip && labs(mode) == mmode) ) {
    appendglue();
    subtype(tail) = boxcontext - leaderflag + aleaders;
    leaderptr(tail) = curbox;
    return;
  }
  printnl(S(292));
  print(S(849));
  help3(S(850),S(851),S(852));
  backerror();
  flushnodelist(curbox);
}
