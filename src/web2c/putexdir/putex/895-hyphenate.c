Static void hyphenate(void)   /*:929*/
{  /*923:*/
  /*901:*/
  /* char */ int i, j, l; /* INT */
  pointer q, r, s;
  halfword bchar;
  /*:901*/
  /*912:*/
  pointer majortail, minortail, hyfnode;
  ASCIIcode c=0 /* XXXX */;
  /* char */ int cloc; /* INT */
  long rcount;
  /*:912*/
  /*922:*/
  triepointer z;
  long v;
  /*:922*/
  /*929:*/
  hyphpointer h;
  strnumber k;
  char FORLIM;

  for (j = 0; j <= hn; j++)   /*930:*/
    hyf[j] = 0;
  h = hc[1];
  hn++;
  hc[hn] = curlang;
  for (j = 2; j <= hn ; j++)
    h = (h + h + hc[j]) % hyphsize;
  while (true) {  /*931:*/
    k = hyphword[h];
    if (k == 0)
      goto _Lnotfound;
    if (flength(k) < hn) {
      goto _Lnotfound;
    }
    if (flength(k) == hn) {
	{
        int ress=str_scmp(k,hc+1);
	if(ress<0) goto _Lnotfound;
	if(ress>0) goto _Ldone;	
	}
      s = hyphlist[h];
      while (s != 0) {   /*:932*/
	hyf[info(s)] = 1;
	s = link(s);
      }
      hn--;
      goto _Lfound;
    }
_Ldone:   /*:931*/
    if (h > 0)
      h--;
    else
      h = hyphsize;
  }
_Lnotfound:
  hn--;   /*:930*/
  if (triechar(curlang + 1) != curlang)
    goto _Lexit;
  hc[0] = 0;
  hc[hn + 1] = 0;
  hc[hn + 2] = 256;
  FORLIM = hn - rhyf + 1;
  for (j = 0; j <= FORLIM; j++) {
    z = trielink(curlang + 1) + hc[j];
    l = j;
    while (hc[l] == triechar(z) - minquarterword) {
      if (trieop(z) != minquarterword) {   /*924:*/
	v = trieop(z);
	do {
	  v += opstart[curlang];
	  i = l - hyfdistance[v - 1];
	  if (hyfnum[v - 1] > hyf[i])
	    hyf[i] = hyfnum[v - 1];
	  v = hyfnext[v - 1];
	} while (v != minquarterword);
      }
      /*:924*/
      l++;
      z = trielink(z) + hc[l];
    }
  }
_Lfound:
  for (j = 0; j < lhyf; j++)
    hyf[j] = 0;
  for (j = 0; j < rhyf; j++)   /*902:*/
    hyf[hn - j] = 0;   /*:923*/
  for (j = lhyf; j <= hn - rhyf; j++) {
    if (hyf[j] & 1)
      goto _Lfound1;
  }
  goto _Lexit;
_Lfound1:   /*:902*/
  /*903:*/
  q = link(hb);
  link(hb) = 0;
  r = link(ha);
  link(ha) = 0;
  bchar = hyfbchar;
  if (ischarnode(ha)) {
    if (font(ha) != hf)
      goto _Lfound2;
    initlist = ha;
    initlig = false;
    hu[0] = character(ha) - minquarterword;
  } else if (type(ha) == ligaturenode) {
    if (font_ligchar(ha) != hf) {
      goto _Lfound2;
    }
    initlist = ligptr(ha);
    initlig = true;
    initlft = (subtype(ha) > 1);
    hu[0] = character_ligchar(ha) - minquarterword;
    if (initlist == 0) {
      if (initlft) {
	hu[0] = 256;
	initlig = false;
      }
    }
    freenode(ha, smallnodesize);
  } else {
    if (!ischarnode(r)) {
      if (type(r) == ligaturenode) {
	if (subtype(r) > 1)
	  goto _Lfound2;
      }
    }
    j = 1;
    s = ha;
    initlist = 0;
    goto _Lcommonending;
  }
  s = curp;
  while (link(s) != ha)
    s = link(s);
  j = 0;
  goto _Lcommonending;
_Lfound2:
  s = ha;
  j = 0;
  hu[0] = 256;
  initlig = false;
  initlist = 0;
_Lcommonending:
  flushnodelist(r);   /*913:*/
  do {
    l = j;
    j = reconstitute(j, hn, bchar, hyfchar) + 1;
    if (hyphenpassed == 0) {
      link(s) = link(holdhead);
      while (link(s) > 0)
	s = link(s);
      if (hyf[j - 1] & 1) {
	l = j;
	hyphenpassed = j - 1;
	link(holdhead) = 0;
      }
    }
    if (hyphenpassed > 0) {   /*914:*/
      do {
	r = getnode(smallnodesize);
	link(r) = link(holdhead);
	type(r) = discnode;
	majortail = r;
	rcount = 0;
	while (link(majortail) > 0) {
	  advancemajortail();
	}
	i = hyphenpassed;
	hyf[i] = 0;   /*915:*/
	minortail = 0;
	prebreak(r) = 0;
	hyfnode = newcharacter(hf, hyfchar);
	if (hyfnode != 0) {
	  i++;
	  c = hu[i];
	  hu[i] = hyfchar;
	  freeavail(hyfnode);
	}
	while (l <= i) {
	  l = reconstitute(l, i, fontbchar[hf ], nonchar) + 1;
	  if (link(holdhead) <= 0)
	    continue;
	  if (minortail == 0)
	    prebreak(r) = link(holdhead);
	  else
	    link(minortail) = link(holdhead);
	  minortail = link(holdhead);
	  while (link(minortail) > 0)
	    minortail = link(minortail);
	}
	if (hyfnode != 0) {   /*:915*/
	  hu[i] = c;
	  l = i;
	  i--;
	}
	/*916:*/
	minortail = 0;
	postbreak(r) = 0;
	cloc = 0;
	if (bcharlabel[hf ] != nonaddress) {
	  l--;
	  c = hu[l];
	  cloc = l;
	  hu[l] = 256;
	}
	while (l < j) {   /*:916*/
	  do {
	    l = reconstitute(l, hn, bchar, nonchar) + 1;
	    if (cloc > 0) {
	      hu[cloc] = c;
	      cloc = 0;
	    }
	    if (link(holdhead) > 0) {
	      if (minortail == 0)
		postbreak(r) = link(holdhead);
	      else
		link(minortail) = link(holdhead);
	      minortail = link(holdhead);
	      while (link(minortail) > 0)
		minortail = link(minortail);
	    }
	  } while (l < j);
	  while (l > j) {   /*917:*/
	    j = reconstitute(j, hn, bchar, nonchar) + 1;
	    link(majortail) = link(holdhead);
	    while (link(majortail) > 0) {
	      advancemajortail();
	    }
	  }
	  /*:917*/
	}
	/*918:*/
	if (rcount > 127) {
	  link(s) = link(r);
	  link(r) = 0;
	  flushnodelist(r);
	} else {
	  link(s) = r;
	  replacecount(r) = rcount;
	}
	s = majortail;   /*:918*/
	hyphenpassed = j - 1;
	link(holdhead) = 0;   /*:914*/
      } while (hyf[j - 1] & 1);
    }
  } while (j <= hn);
  link(s) = q;   /*:913*/
  /*:903*/
  flushlist(initlist);
_Lexit: ;
}
