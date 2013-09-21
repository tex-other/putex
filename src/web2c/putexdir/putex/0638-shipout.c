Static void shipout(halfword p)
{
  int j, k;
  char oldsetting;

  if (tracingoutput > 0) {
    printnl(S(385));
    println();
    print(S(686));
  }
  if (termoffset > maxprintline - 9)
    println();
  else if (termoffset > 0 || fileoffset > 0)
    printchar(' ');
  printchar('[');
  j = 9;
  while (count(j) == 0 && j > 0)
    j--;
  for (k = 0; k <= j; k++) {
    printint(count(k));
    if (k < j)
      printchar('.');
  }
  fflush(termout);
  if (tracingoutput > 0) {   /*640:*/
    printchar(']');
    begindiagnostic();
    showbox(p);
    enddiagnostic(true);
  }
  /*641:*/
  if ((height(p) > maxdimen) | (depth(p) > maxdimen) |
      (height(p) + depth(p) + voffset > maxdimen) |
      (width(p) + hoffset > maxdimen)) {
    printnl(S(292));
    print(S(687));
    help2(S(688),S(689));
    error();
    if (tracingoutput <= 0) {
      begindiagnostic();
      printnl(S(690));
      showbox(p);
      enddiagnostic(true);
    }
    goto _Ldone;
  }
  if (height(p) + depth(p) + voffset > maxv)
    maxv = height(p) + depth(p) + voffset;
  if (width(p) + hoffset > maxh)
    maxh = width(p) + hoffset;   /*:641*/
  /*617:*/
  dvih = 0;
  dviv = 0;
  curh = hoffset;
  dvif = nullfont;
  if (outputfilename == 0) {
    if (jobname == 0)
      openlogfile();
    packjobname(S(691));
    while (!dvi_openout())
      promptfilename(S(692), S(691));
    outputfilename = bmakenamestring();
  }
  if (totalpages == 0) {   /*:617*/
    preparemag();
    dvi_pre(25400000L, 473628672L, mag);
    oldsetting = selector;
    selector = newstring;
    print(S(693));
    printint(year);
    printchar('.');
    printtwo(month);
    printchar('.');
    printtwo(day);
    printchar(':');
    printtwo(tex_time / 60);
    printtwo(tex_time % 60);
    selector = oldsetting;
    dviout(get_cur_length()); /* XXXX */
    str_cur_map(dviout_helper);
  }
  {
    long cp[10];
    for (k = 0; k <= 9; k++) {
	cp[k]=count(k);
    }
    dvibop(cp);
  }
  curv = height(p) + voffset;
  tempptr = p;
  if (type(p) == vlistnode)
    vlistout();
  else
    hlistout();
  dvi_eop();
  totalpages++;
  curs = -1;
_Ldone:   /*:640*/
  if (tracingoutput <= 0)
    printchar(']');
  deadcycles = 0;
  fflush(termout);   /*639:*/
  /*_STAT*/
  if (tracingstats > 1) {   /*_ENDSTAT*/
    printnl(S(694));
    printint(varused);
    printchar('&');
    printint(dynused);
    printchar(';');
  }
  flushnodelist(p);   /*_STAT*/
  if (tracingstats <= 1)   /*_ENDSTAT*/
    return;
  /*:639*/
  print(S(695));
  printint(varused);
  printchar('&');
  printint(dynused);
  print(S(696));
  printint(himemmin - lomemmax - 1);
  println();
}
