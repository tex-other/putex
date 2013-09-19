Static void newpatterns(void)
{
  unsigned char k, l; /* INT */
  boolean digitsensed;
  quarterword v;
  triepointer p, q;
  boolean firstchild;
  ASCIIcode c;

  if (trienotready) {
    setcurlang();
    scanleftbrace();
    /*961:*/
    k = 0;
    hyf[0] = 0;
    digitsensed = false;
    while (true) {
      getxtoken();
      switch (curcmd) {

      case letter:
      case otherchar:   /*962:*/
	if (digitsensed | (curchr < '0') | (curchr > '9')) {
	  if (curchr == '.')
	    curchr = 0;
	  else {
	    curchr = lccode(curchr);
	    if (curchr == 0) {
	      printnl(S(292));
	      print(S(770));
	      help1(S(771));
	      error();
	    }
	  }
	  if (k < 63) {
	    k++;
	    hc[k] = curchr;
	    hyf[k] = 0;
	    digitsensed = false;
	  }
	} else if (k < 63) {
	  hyf[k] = curchr - '0';
	  digitsensed = true;
	}
	break;

      case spacer:
      case rightbrace:
	if (k > 0) {   /*963:*/
	  if (hc[1] == 0)
	    hyf[0] = 0;
	  if (hc[k] == 0)
	    hyf[k] = 0;
	  l = k;
	  v = minquarterword;
	  while (true) {
	    if (hyf[l] != 0)
	      v = newtrieop(k - l, hyf[l], v);
	    if (l <= 0)
	      goto _Ldone1;
	    l--;
	  }
_Ldone1:   /*:965*/
	  q = 0;
	  hc[0] = curlang;
	  while (l <= k) {
	    c = hc[l];
	    l++;
	    p = triel[q];
	    firstchild = true;
	    while (p > 0 && c > triec[p]) {
	      q = p;
	      p = trier[q];
	      firstchild = false;
	    }
	    if (p == 0 || c < triec[p]) {   /*964:*/
	      if (trieptr == triesize)
		overflow(S(769), triesize);
	      trieptr++;
	      trier[trieptr] = p;
	      p = trieptr;
	      triel[p] = 0;
	      if (firstchild)
		triel[q] = p;
	      else
		trier[q] = p;
	      triec[p] = c;
	      trieo[p] = minquarterword;
	    }
	    /*:964*/
	    q = p;
	  }
	  if (trieo[q] != minquarterword) {
	    printnl(S(292));
	    print(S(772));
	    help1(S(771));
	    error();
	  }
	  trieo[q] = v;
	}
	/*965:*/
	/*:963*/
	if (curcmd == rightbrace)
	  goto _Ldone;
	k = 0;
	hyf[0] = 0;
	digitsensed = false;
	break;

      default:
	printnl(S(292));
	print(S(773));
	printesc(S(774));
	help1(S(771));
	error();
	break;
      }
    }
_Ldone:   /*:961*/
    return;
  }
  printnl(S(292));
  print(S(775));
  printesc(S(774));
  help1(S(776));
  error();
  link(garbage) = scantoks(false, false);
  flushlist(defref);

  /*:962*/
}
