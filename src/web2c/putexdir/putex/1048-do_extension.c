Static void doextension(void)
{
  long k;
  pointer p;

  switch (curchr) {

  case opennode:   /*1351:*/
    newwritewhatsit(opennodesize);
    scanoptionalequals();
    scanfilename();
    openname(tail) = curname;
    openarea(tail) = curarea;
    openext(tail) = curext;
    break;
    /*:1351*/

  case writenode:   /*1352:*/
    k = curcs;
    newwritewhatsit(writenodesize);
    curcs = k;
    p = scantoks(false, false);
    writetokens(tail) = defref;
    break;
    /*:1352*/

  case closenode:   /*1353:*/
    newwritewhatsit(writenodesize);
    writetokens(tail) = 0;
    break;
    /*:1353*/

  case specialnode:   /*1354:*/
    newwhatsit(specialnode, writenodesize);
    writestream(tail) = 0;
    p = scantoks(false, true);
    writetokens(tail) = defref;
    break;
    /*:1354*/

  case immediatecode:   /*1375:*/
    getxtoken();
    if (curcmd == extension && curchr <= closenode) {
      p = tail;
      doextension();
      outwhat(tail);
      flushnodelist(tail);
      tail = p;
      link(p) = 0;
    } else
      backinput();
    break;
    /*:1375*/

  case setlanguagecode:   /*1377:*/
    if (labs(mode) != hmode)
      reportillegalcase();
    else {   /*:1377*/
      newwhatsit(languagenode, smallnodesize);
      scanint();
      if (curval <= 0)
	clang = 0;
      else if (curval > 255)
	clang = 0;
      else
	clang = curval;
      whatlang(tail) = clang;
      whatlhm(tail) = normmin(lefthyphenmin);
      whatrhm(tail) = normmin(righthyphenmin);
    }
    break;

  default:
    confusion(S(1001));
    break;
  }
}
