Static void newhyphexceptions(void)
{
  unsigned char n; /* INT */
  unsigned char j; /* INT */
  hyphpointer h;
  strnumber k, s, t;
  pointer p, q;

  scanleftbrace();
  setcurlang();   /*935:*/
  n = 0;
  p = 0;
  while (true) {   /*:935*/
    getxtoken();
_Lreswitch:
    switch (curcmd) {

    case letter:
    case otherchar:
    case chargiven:   /*937:*/
      if (curchr == '-') {   /*938:*/
	if (n < 63) {
	  q = getavail();
	  link(q) = p;
	  info(q) = n;
	  p = q;
	}
      }  /*:938*/
      else {   /*:937*/
	if (lccode(curchr) == 0) {
	  printnl(S(292));
	  print(S(783));
	  help2(S(784),S(785));
	  error();
	} else if (n < 63) {
	  n++;
	  hc[n] = lccode(curchr);
	}
      }
      break;

    case charnum:
      scancharnum();
      curchr = curval;
      curcmd = chargiven;
      goto _Lreswitch;
      break;

    case spacer:
    case rightbrace:   /*936:*/
      if (n > 1) {   /*939:*/
	n++;
	hc[n] = curlang;
	h = 0;
	for (j = 1; j <= n; j++) {
	  h = (h + h + hc[j]) % hyphsize;
	}
	s = str_ins(hc+1,n);
	if (hyphcount == hyphsize)
	  overflow(S(786), hyphsize);
	hyphcount++;
	while (hyphword[h] != 0) {  /*941:*/
	  k = hyphword[h];
	  if (flength(k) < flength(s)) {
	    goto _Lfound;
	  }
	  if (flength(k) > flength(s)) {
	    goto _Lnotfound;
	  }
	{ int ress = str_cmp(k,s);
	if(ress < 0) goto _Lfound;
	if(ress > 0) goto _Lnotfound;
	}	
_Lfound:
	  q = hyphlist[h];
	  hyphlist[h] = p;
	  p = q;
	  t = hyphword[h];
	  hyphword[h] = s;
	  s = t;
_Lnotfound:   /*:941*/
	  if (h > 0)
	    h--;
	  else
	    h = hyphsize;
	}
	hyphword[h] = s;
	hyphlist[h] = p;   /*:940*/
      }
      /*:939*/
      if (curcmd == rightbrace)
	goto _Lexit;
      n = 0;
      p = 0;
      break;

    default:
      printnl(S(292));
      print(S(597));
      printesc(S(787));
      print(S(788));
      help2(S(789),S(790));
      error();   /*:936*/
      break;
    }
  }
_Lexit: ;
}
