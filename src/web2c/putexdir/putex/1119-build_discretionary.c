Static void builddiscretionary(void)
{
  pointer p, q;
  long n;

  unsave();   /*1121:*/
  q = head;
  p = link(q);
  n = 0;
  while (p != 0) {
    if (!ischarnode(p)) {
      if (type(p) > rulenode) {
	if (type(p) != kernnode) {
	  if (type(p) != ligaturenode) {
	    printnl(S(292));
	    print(S(876));
	    help1(S(877));
	    error();
	    begindiagnostic();
	    printnl(S(878));
	    showbox(p);
	    enddiagnostic(true);
	    flushnodelist(p);
	    link(q) = 0;
	    goto _Ldone;
	  }
	}
      }
    }
    q = p;
    p = link(q);
    n++;
  }
_Ldone:   /*:1121*/
  p = link(head);
  popnest();
  switch (saved(-1)) {

  case 0:
    prebreak(tail) = p;
    break;

  case 1:
    postbreak(tail) = p;
    break;

  case 2:   /*1120:*/
    if (n > 0 && labs(mode) == mmode) {
      printnl(S(292));
      print(S(879));
      printesc(S(400));
      help2(S(880),S(881));
      flushnodelist(p);
      n = 0;
      error();
    } else
      link(tail) = p;
    if (n <= maxquarterword)
      replacecount(tail) = n;
    else {
      printnl(S(292));
      print(S(882));
      help2(S(883),S(884));
      error();
    }
    if (n > 0)
      tail = q;
    saveptr--;
    goto _Lexit;
    break;
    /*:1120*/
  }
  (saved(-1))++;
  newsavelevel(discgroup);
  scanleftbrace();
  pushnest();
  mode = -hmode;
  spacefactor = 1000;
_Lexit: ;
}
