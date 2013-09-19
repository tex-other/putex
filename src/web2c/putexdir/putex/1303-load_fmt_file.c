Static boolean loadfmtfile(void)
{  /*1308:*/
  boolean Result;
  long j, k, x;
  pointer p, q;
  /* fourquarters w; */
  memoryword pppfmtfile;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x != 371982687L)
    goto _Lbadfmt_;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x != membot)
    goto _Lbadfmt_;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x != memtop)
    goto _Lbadfmt_;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x != eqtbsize)
    goto _Lbadfmt_;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x != hashprime)
    goto _Lbadfmt_;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x != hyphsize)   /*1310:*/
    goto _Lbadfmt_;  
  if(!str_undump(fmtfile,termout)) goto _Lbadfmt_;
  /*1312:*/
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x < lomemstatmax + 1000 || x >= himemstatmin)
    goto _Lbadfmt_;
  lomemmax = x;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x <= lomemstatmax || x > lomemmax)
    goto _Lbadfmt_;
  rover = x;
  p = membot;
  q = rover;
  do {
    for (k = p; k <= q + 1; k++) {
      pget(pppfmtfile);
      mem[k - memmin] = pppfmtfile;
    }
    p = q + nodesize(q);
    if ((p > lomemmax) | ((q >= rlink(q)) & (rlink(q) != rover)))
      goto _Lbadfmt_;
    q = rlink(q);
  } while (q != rover);
  for (k = p; k <= lomemmax; k++) {
    pget(pppfmtfile);
    mem[k - memmin] = pppfmtfile;
  }
  if (memmin < membot - 2) {
    p = llink(rover);
    q = memmin + 1;
    link(memmin) = 0;
    info(memmin) = 0;
    rlink(p) = q;
    llink(rover) = q;
    rlink(q) = rover;
    llink(q) = p;
    link(q) = emptyflag;
    nodesize(q) = membot - q;
  }
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x <= lomemmax || x > himemstatmin)
    goto _Lbadfmt_;
  himemmin = x;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if ((unsigned long)x > memtop)
    goto _Lbadfmt_;
  avail = x;
  memend = memtop;
  for (k = himemmin; k <= memend ; k++) {
    pget(pppfmtfile);
    mem[k - memmin] = pppfmtfile;
  }
  pget(pppfmtfile);
  varused = pppfmtfile.int_;
  pget(pppfmtfile);
  dynused = pppfmtfile.int_;   /*:1312*/
  /*1314:*/
  /*1317:*/
  k = activebase;
  do {
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 1 || k + x > eqtbsize + 1)
      goto _Lbadfmt_;
    for (j = k; j < k + x; j++) {
      pget(pppfmtfile);
      eqtb[j - activebase] = pppfmtfile;
    }
    k += x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0 || k + x > eqtbsize + 1)
      goto _Lbadfmt_;
    for (j = k; j < k + x; j++)
      eqtb[j - activebase] = eqtb[k - activebase - 1];
    k += x;   /*:1317*/
  } while (k <= eqtbsize);
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x < hashbase || x > frozencontrolsequence)
    goto _Lbadfmt_;
  parloc = x;
  partoken = cstokenflag + parloc;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x < hashbase || x > frozencontrolsequence)   /*1319:*/
    goto _Lbadfmt_;
  writeloc = x;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x < hashbase || x > frozencontrolsequence)
    goto _Lbadfmt_;
  hashused = x;
  p = hashbase - 1;
  do {
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x <= p || x > hashused)
      goto _Lbadfmt_;
    p = x;
    pget(pppfmtfile);
    hash[p - hashbase] = pppfmtfile.hh;
  } while (p != hashused);
  for (p = hashused + 1; p < undefinedcontrolsequence; p++) {
    pget(pppfmtfile);
    hash[p - hashbase] = pppfmtfile.hh;
  }
  pget(pppfmtfile);
  cscount = pppfmtfile.int_;   /*:1319*/
  /*:1314*/
  if(!fonts_undump(fmtfile,termout))
	goto _Lbadfmt_;
  /*1325:*/
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if ((unsigned long)x > hyphsize)
    goto _Lbadfmt_;
  hyphcount = x;
  for (k = 1; k <= hyphcount; k++) {
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > hyphsize)
      goto _Lbadfmt_;
    j = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (!str_valid(x))
      goto _Lbadfmt_;
    hyphword[j] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > maxhalfword)
      goto _Lbadfmt_;
    hyphlist[j] = x;
  }
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x < 0)
    goto _Lbadfmt_;
  if (x > triesize) {
    fprintf(termout, "---! Must increase the trie size\n");
    goto _Lbadfmt_;
  }
  j = x;
  triemax = j;
  for (k = 0; k <= j; k++) {
    pget(pppfmtfile);
    trie[k] = pppfmtfile.hh;
  }
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x < 0)
    goto _Lbadfmt_;
  if (x > trieopsize) {
    fprintf(termout, "---! Must increase the trie op size\n");
    goto _Lbadfmt_;
  }
  j = x;
  trieopptr = j;
  for (k = 1; k <= j; k++) {
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > 63)
      goto _Lbadfmt_;
    hyfdistance[k - 1] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > 63)
      goto _Lbadfmt_;
    hyfnum[k - 1] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > maxquarterword)
      goto _Lbadfmt_;
    hyfnext[k - 1] = x;
  }
  for (k = 0; k <= 255; k++)
    trieused[k] = minquarterword;
  k = 256;
  while (j > 0) {
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ( x >= k)
      goto _Lbadfmt_;
    k = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 1 || x > j)
      goto _Lbadfmt_;
    trieused[k] = x;
    j -= x;
    opstart[k] = j - minquarterword;
  }
  trienotready = false;   /*:1325*/
  /*1327:*/
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if ((unsigned long)x > errorstopmode)
    goto _Lbadfmt_;
  interaction = x;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
    if (!str_valid(x))
      goto _Lbadfmt_;
  formatident = x;
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if ((x != 69069L) | feof(fmtfile))
    goto _Lbadfmt_;   /*:1327*/
  Result = true;
  goto _Lexit;
_Lbadfmt_:
  fprintf(termout, "(Fatal format file error; I'm stymied)\n");
  Result = false;
_Lexit:
  return Result;
}
