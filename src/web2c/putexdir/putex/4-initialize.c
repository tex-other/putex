Static void initialize(void)
{   /*:927*/
  long i;
  long k;
  hyphpointer z;
  for (i = 0; i <= 255; i++)
    xchr[i] = (Char)i;
  for (i = firsttextchar; i <= lasttextchar; i++)
    xord[(Char)i] = invalidcode;
  for (i = 128; i <= 255; i++)
    xord[xchr[i]] = i;
  for (i = 0; i <= 126; i++)   /*:24*/
    xord[xchr[i]] = i;
  interaction = errorstopmode;   /*:74*/
  deletionsallowed = true;
  setboxallowed = true;
  errorcount = 0;   /*:77*/
  helpptr = 0;
  useerrhelp = false;   /*:80*/
  interrupt = 0;
  OKtointerrupt = true;   /*:97*/
  wasmemend = memmin;
  waslomax = memmin;
  washimin = memmax;
  panicking = false;   /*_ENDDEBUG*/
  nestptr = 0;
  maxneststack = 0;
  mode = vmode;
  head = contribhead;
  tail = contribhead;
  prevdepth = ignoredepth;
  modeline = 0;
  prevgraf = 0;
  shownmode = 0;   /*991:*/
  pagecontents = empty;
  pagetail = pagehead;
  link(pagehead) = 0;
  lastglue = maxhalfword;
  lastpenalty = 0;
  lastkern = 0;
  pagedepth = 0;
  pagemaxdepth = 0;   /*:991*/
  for (k = intbase; k <= eqtbsize; k++)
    xeqlevel[k - intbase] = levelone;
  nonewcontrolsequence = true;
  next(hashbase) = 0;
  text(hashbase) = 0;
  for (k = hashbase + 1; k < undefinedcontrolsequence; k++)   /*:257*/
    hash[k - hashbase] = hash[0];
  /*272:*/
  saveptr = 0;
  curlevel = levelone;
  curgroup = bottomlevel;
  curboundary = 0;
  maxsavestack = 0;   /*:272*/
  /*287:*/
  magset = 0;   /*:287*/
  /*383:*/
  topmark = 0;
  firstmark = 0;
  botmark = 0;
  splitfirstmark = 0;
  splitbotmark = 0;   /*:383*/
  /*439:*/
  curval = 0;
  curvallevel = intval;
  radix = 0;
  curorder = 0;   /*:439*/
  /*481:*/
  for (k = 0; k <= 16; k++)   /*:481*/
    readopen[k] = closed;
  /*490:*/
  condptr = 0;
  iflimit = normal;
  curif = 0;
  ifline = 0;   /*:490*/
  /*521:*/
  memcpy(TEXformatdefault, "TeXformats:plain.fmt", formatdefaultlength);
      /*:521*/
  /*556:*/
  nullcharacter.b0 = minquarterword;
  nullcharacter.b1 = minquarterword;
  nullcharacter.b2 = minquarterword;
  nullcharacter.b3 = minquarterword;   /*:556*/
  /*593:*/
  totalpages = 0;
  maxv = 0;
  maxh = 0;
  maxpush = 0;
  doingleaders = false;
  deadcycles = 0;
  curs = -1;   /*:593*/
  dviout_init();
  /*648:*/
  adjusttail = 0;
  lastbadness = 0;   /*:648*/
  /*662:*/
  packbeginline = 0;   /*:662*/
  /*685:*/
  emptyfield.rh = empty;
  emptyfield.UU.lh = 0;
  nulldelimiter.b0 = 0;
  nulldelimiter.b1 = minquarterword;
  nulldelimiter.b2 = 0;
  nulldelimiter.b3 = minquarterword;   /*:685*/
  /*771:*/
  alignptr = 0;
  curalign = 0;
  curspan = 0;
  curloop = 0;
  curhead = 0;
  curtail = 0;   /*:771*/
  /*928:*/
  for (z = 0; z <= hyphsize; z++) {
    hyphword[z] = 0;
    hyphlist[z] = 0;
  }
  hyphcount = 0;   /*:928*/
  /*990:*/
  outputactive = false;
  insertpenalties = 0;   /*:990*/
  /*1033:*/
  ligaturepresent = false;
  cancelboundary = false;
  lfthit = false;
  rthit = false;
  insdisc = false;   /*:1033*/
  /*1267:*/
  aftertoken = 0;   /*:1267*/
  /*1282:*/
  longhelpseen = false;   /*:1282*/
  /*1300:*/
  formatident = 0;   /*:1300*/
  /*1343:*/
  for (k = 0; k <= 17; k++)   /*:1343*/
    writeopen[k] = false;
  /*164:*/
  for (k = membot + 1; k <= lomemstatmax; k++)
    mem[k - memmin].sc = 0;
  k = membot;
  while (k <= lomemstatmax) {
    gluerefcount(k) = 1;
    stretchorder(k) = normal;
    shrinkorder(k) = normal;
    k += gluespecsize;
  }
  stretch(filglue) = unity;
  stretchorder(filglue) = fil;
  stretch(fillglue) = unity;
  stretchorder(fillglue) = fill;
  stretch(ssglue) = unity;
  stretchorder(ssglue) = fil;
  shrink(ssglue) = unity;
  shrinkorder(ssglue) = fil;
  stretch(filnegglue) = -unity;
  stretchorder(filnegglue) = fil;
  rover = lomemstatmax + 1;
  link(rover) = emptyflag;
  nodesize(rover) = 1000;
  llink(rover) = rover;
  rlink(rover) = rover;
  lomemmax = rover + 1000;
  link(lomemmax) = 0;
  info(lomemmax) = 0;
  for (k = himemstatmin; k <= memtop; k++) {   /*790:*/
	mem[k - memmin].sc = 0;
	type(k) = charnodetype;
  }
/*    mem[k - memmin] = mem[lomemmax - memmin]; */
  info(omittemplate) = endtemplatetoken;   /*:790*/
  /*797:*/
  link(endspan) = maxquarterword + 1;
  info(endspan) = 0;   /*:797*/
  /*820:*/
  type(lastactive) = hyphenated;
  linenumber(lastactive) = maxhalfword;
  subtype(lastactive) = 0;
  /*:820*/
  /*981:*/
  subtype(pageinshead) = minquarterword + 255;
  type(pageinshead) = splitup;
  link(pageinshead) = pageinshead;   /*:981*/
  /*988:*/
  type(pagehead) = gluenode;
  subtype(pagehead) = normal;
  /*:988*/
  avail = 0;
  memend = memtop;
  himemmin = himemstatmin;
  varused = lomemstatmax - membot + 1;
  dynused = himemstatusage;   /*:164*/
  /*222:*/
  eqtype(undefinedcontrolsequence) = undefinedcs;
  equiv(undefinedcontrolsequence) = 0;
  eqlevel(undefinedcontrolsequence) = levelzero;
  for (k = activebase; k < undefinedcontrolsequence; k++)   /*:222*/
    eqtb[k - activebase] = eqtb[undefinedcontrolsequence - activebase];
  /*228:*/
  equiv(gluebase) = zeroglue;
  eqlevel(gluebase) = levelone;
  eqtype(gluebase) = glueref;
  for (k = gluebase + 1; k < localbase; k++)
    eqtb[k - activebase] = eqtb[gluebase - activebase];
  gluerefcount(zeroglue) += localbase - gluebase;   /*:228*/
  /*232:*/
  parshapeptr = 0;
  eqtype(parshapeloc) = shaperef;
  eqlevel(parshapeloc) = levelone;
  for (k = outputroutineloc; k <= toksbase + 255; k++)
    eqtb[k - activebase] = eqtb[undefinedcontrolsequence - activebase];
  box(0) = 0;
  eqtype(boxbase) = boxref;
  eqlevel(boxbase) = levelone;
  for (k = boxbase + 1; k <= boxbase + 255; k++)
    eqtb[k - activebase] = eqtb[boxbase - activebase];
  curfont = nullfont;
  eqtype(curfontloc) = data;
  eqlevel(curfontloc) = levelone;
  for (k = mathfontbase; k <= mathfontbase + 47; k++)
    eqtb[k - activebase] = eqtb[curfontloc - activebase];
  equiv(catcodebase) = 0;
  eqtype(catcodebase) = data;
  eqlevel(catcodebase) = levelone;
  for (k = catcodebase + 1; k < intbase; k++)
    eqtb[k - activebase] = eqtb[catcodebase - activebase];
  for (k = 0; k <= 255; k++) {
    catcode(k) = otherchar;
    mathcode(k) = k;
    sfcode(k) = 1000;
  }
  catcode(carriagereturn) = carret;
  catcode(' ') = spacer;
  catcode('\\') = escape;
  catcode('%') = comment;
  catcode(invalidcode) = invalidchar;
  catcode(nullcode) = ignore;
  for (k = '0'; k <= '9'; k++)
    mathcode(k) = k + varcode;
  for (k = 'A'; k <= 'Z'; k++) {   /*:232*/
    catcode(k) = letter;
    catcode(k + 'a' - 'A') = letter;
    mathcode(k) = k + varcode + 256;
    mathcode(k + 'a' - 'A') =
      k + 'a' - 'A' + varcode + 256;
    lccode(k) = k + 'a' - 'A';
    lccode(k + 'a' - 'A') = k + 'a' - 'A';
    uccode(k) = k;
    uccode(k + 'a' - 'A') = k;
    sfcode(k) = 999;
  }
  /*240:*/
  for (k = intbase; k < delcodebase; k++)
    eqtb[k - activebase].int_ = 0;
  mag = 1000;
  tolerance = 10000;
  hangafter = 1;
  maxdeadcycles = 25;
  escapechar = '\\';
  endlinechar = carriagereturn;
  for (k = 0; k <= 255; k++)
    delcode(k) = -1;
  delcode('.') = 0;   /*:240*/
  /*250:*/
  for (k = dimenbase; k <= eqtbsize; k++)   /*:250*/
    eqtb[k - activebase].sc = 0;
  /*258:*/
  hashused = frozencontrolsequence;
  cscount = 0;
  eqtype(frozendontexpand) = dontexpand;
  text(frozendontexpand) = S(257);   /*:258*/
  fonts_init();
  /*946:*/
  for (k = -trieopsize; k <= trieopsize; k++)
    trieophash[k + trieopsize] = 0;
  for (k = 0; k <= 255; k++)
    trieused[k] = minquarterword;
  trieopptr = 0;   /*:946*/
  /*951:*/
  trienotready = true;
  trieroot = 0;
  triec[0] = 0;
  trieptr = 0;   /*:951*/
  /*1216:*/
  text(frozenprotection) = S(258);   /*:1216*/
  /*1301:*/
  formatident = S(259);   /*:1301*/
  /*1369:*/
  text(endwrite) = S(260);
  eqlevel(endwrite) = levelone;
  eqtype(endwrite) = outercall;
  equiv(endwrite) = 0;   /*:1369*/
  /*:8*/
}
