Static void storefmtfile(void)
{  /*1304:*/
  long j, k, l, x;
  pointer p, q;
  memoryword pppfmtfile;
  if (saveptr != 0) {
    printnl(S(292));
    print(S(988));
    help1(S(989));
    if (interaction == errorstopmode)
      interaction = scrollmode;
    if (logopened)   /*_DEBUG*/
      error();
    if (interaction > batchmode)
      debughelp();
    /*_ENDDEBUG*/
    history = fatalerrorstop;
    jumpout();
  }
  /*:1304*/
  /*1328:*/
  selector = newstring;
  print(S(990));
  print(jobname);
  printchar(' ');
/* p2c: tex1.p: Note: Using % for possibly-negative arguments [317] */
  printint(year % 100);
  printchar('.');
  printint(month);
  printchar('.');
  printint(day);
  printchar(')');
  if (interaction == batchmode)
    selector = logonly;
  else
    selector = termandlog;
  str_room(1);
  formatident = makestring();
  packjobname(formatextension);
  while (!wopenout(&fmtfile))
    promptfilename(S(991), formatextension);
  printnl(S(992));
  slowprint(wmakenamestring(&fmtfile));
  flushstring();
  printnl(S(385));   /*:1328*/
  slowprint(formatident);   /*1307:*/
  pppfmtfile.int_ = 371982687L;
  pput(pppfmtfile);
  pppfmtfile.int_ = membot;
  pput(pppfmtfile);
  pppfmtfile.int_ = memtop;
  pput(pppfmtfile);
  pppfmtfile.int_ = eqtbsize;
  pput(pppfmtfile);
  pppfmtfile.int_ = hashprime;
  pput(pppfmtfile);
  pppfmtfile.int_ = hyphsize;
  pput(pppfmtfile);   /*:1307*/
  str_dump(fmtfile);
   /*1311:*/
  sortavail();
  varused = 0;
  pppfmtfile.int_ = lomemmax;
  pput(pppfmtfile);
  pppfmtfile.int_ = rover;
  pput(pppfmtfile);
  p = membot;
  q = rover;
  x = 0;
  do {
    for (k = p; k <= q + 1; k++) {
      pppfmtfile = mem[k - memmin];
      pput(pppfmtfile);
    }
    x += q - p + 2;
    varused += q - p;
    p = q + nodesize(q);
    q = rlink(q);
  } while (q != rover);
  varused += lomemmax - p;
  dynused = memend - himemmin + 1;
  for (k = p; k <= lomemmax ; k++) {
    pppfmtfile = mem[k - memmin];
    pput(pppfmtfile);
  }
  x += lomemmax - p + 1;
  pppfmtfile.int_ = himemmin;
  pput(pppfmtfile);
  pppfmtfile.int_ = avail;
  pput(pppfmtfile);
  for (k = himemmin; k <= memend; k++) {
    pppfmtfile = mem[k - memmin];
    pput(pppfmtfile);
  }
  x += memend - himemmin + 1;
  p = avail;
  while (p != 0) {
    dynused-=charnodesize;
    p = link(p);
  }
  pppfmtfile.int_ = varused;
  pput(pppfmtfile);
  pppfmtfile.int_ = dynused;
  pput(pppfmtfile);
  println();
  printint(x);
  print(S(993));
  printint(varused);
  printchar('&');   /*:1311*/
  printint(dynused);   /*1313:*/
  /*1315:*/
  k = activebase;
  do {   /*1316:*/
    j = k;
    while (j < intbase - 1) {
      if ((equiv(j) == equiv(j + 1)) & (eqtype(j) ==
	    eqtype(j + 1)) & (eqlevel(j) == eqlevel(j + 1)))
	goto _Lfound1;
      j++;
    }
    l = intbase;
    goto _Ldone1;
_Lfound1:
    j++;
    l = j;
    while (j < intbase - 1) {
      if ((equiv(j) != equiv(j + 1)) | (eqtype(j) !=
	    eqtype(j + 1)) | (eqlevel(j) != eqlevel(j + 1)))
	goto _Ldone1;
      j++;
    }
_Ldone1:
    pppfmtfile.int_ = l - k;
    pput(pppfmtfile);
    while (k < l) {
      pppfmtfile = eqtb[k - activebase];
      pput(pppfmtfile);
      k++;
    }
    k = j + 1;
    pppfmtfile.int_ = k - l;
    pput(pppfmtfile);   /*:1315*/
  } while (k != intbase);
  do {
    j = k;
    while (j < eqtbsize) {
      if (eqtb[j - activebase].int_ == eqtb[j - activebase + 1].int_)
	goto _Lfound2;
      j++;
    }
    l = eqtbsize + 1;
    goto _Ldone2;
_Lfound2:
    j++;
    l = j;
    while (j < eqtbsize) {
      if (eqtb[j - activebase].int_ != eqtb[j - activebase + 1].int_)
	goto _Ldone2;
      j++;
    }
_Ldone2:
    pppfmtfile.int_ = l - k;
    pput(pppfmtfile);
    while (k < l) {
      pppfmtfile = eqtb[k - activebase];
      pput(pppfmtfile);
      k++;
    }
    k = j + 1;
    pppfmtfile.int_ = k - l;
    pput(pppfmtfile);   /*:1316*/
  } while (k <= eqtbsize);
  pppfmtfile.int_ = parloc;
  pput(pppfmtfile);
  pppfmtfile.int_ = writeloc;
  pput(pppfmtfile);   /*1318:*/
  pppfmtfile.int_ = hashused;
  pput(pppfmtfile);
  cscount = frozencontrolsequence - hashused - 1;
  for (p = hashbase; p <= hashused; p++) {
    if (text(p) != 0) {
      pppfmtfile.int_ = p;
      pput(pppfmtfile);
      pppfmtfile.hh = hash[p - hashbase];
      pput(pppfmtfile);
      cscount++;
    }
  }
  for (p = hashused + 1; p < undefinedcontrolsequence; p++) {
    pppfmtfile.hh = hash[p - hashbase];
    pput(pppfmtfile);
  }
  pppfmtfile.int_ = cscount;
  pput(pppfmtfile);
  println();
  printint(cscount);   /*:1318*/
  /*:1313*/
  print(S(994));   
  fonts_dump(fmtfile);
  /*1324:*/
  pppfmtfile.int_ = hyphcount;
  pput(pppfmtfile);
  for (k = 0; k <= hyphsize; k++) {
    if (hyphword[k] != 0) {
      pppfmtfile.int_ = k;
      pput(pppfmtfile);
      pppfmtfile.int_ = hyphword[k];
      pput(pppfmtfile);
      pppfmtfile.int_ = hyphlist[k];
      pput(pppfmtfile);
    }
  }
  println();
  printint(hyphcount);
  print(S(995));
  if (hyphcount != 1)
    printchar('s');
  if (trienotready)
    inittrie();
  pppfmtfile.int_ = triemax;
  pput(pppfmtfile);
  for (k = 0; k <= triemax; k++) {
    pppfmtfile.hh = trie[k];
    pput(pppfmtfile);
  }
  pppfmtfile.int_ = trieopptr;
  pput(pppfmtfile);
  for (k = 0; k < trieopptr; k++) {
    pppfmtfile.int_ = hyfdistance[k];
    pput(pppfmtfile);
    pppfmtfile.int_ = hyfnum[k];
    pput(pppfmtfile);
    pppfmtfile.int_ = hyfnext[k];
    pput(pppfmtfile);
  }
  printnl(S(996));
  printint(triemax);
  print(S(997));
  printint(trieopptr);
  print(S(998));
  if (trieopptr != 1)
    printchar('s');
  print(S(999));
  printint(trieopsize);
  for (k = 255; k >= 0; k--) {   /*1326:*/
    if (trieused[k] > minquarterword) {   /*:1324*/
      printnl(S(675));
      printint(trieused[k] - minquarterword);
      print(S(1000));
      printint(k);
      pppfmtfile.int_ = k;
      pput(pppfmtfile);
      pppfmtfile.int_ = trieused[k] - minquarterword;
      pput(pppfmtfile);
    }
  }
  pppfmtfile.int_ = interaction;
  pput(pppfmtfile);
  pppfmtfile.int_ = formatident;
  pput(pppfmtfile);
  pppfmtfile.int_ = 69069L;
  pput(pppfmtfile);
  tracingstats = 0;   /*:1326*/
  /*1329:*/
  /*:1329*/
  wclose(&fmtfile);
}

