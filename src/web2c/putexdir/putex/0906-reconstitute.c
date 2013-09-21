Static smallnumber reconstitute(/* smallnumber */ int j, smallnumber n, halfword bchar,
				halfword hchar)
{
  pointer p, t;
  fourquarters q;
  halfword currh, testchar;
  scaled w;
  fontindex k;

  hyphenpassed = 0;
  t = holdhead;
  w = 0;
  link(holdhead) = 0;   /*908:*/
  curl = hu[j];
  curq = t;
  if (j == 0) {
    ligaturepresent = initlig;
    p = initlist;
    if (ligaturepresent)
      lfthit = initlft;
    while (p > 0) {
      appendcharnodetot(character(p));
      p = link(p);
    }
  } else if (curl < nonchar) {
    appendcharnodetot(curl);
  }
  ligstack = 0;   /*:908*/
  setcurr();
_Llabcontinue:   /*909:*/
  if (curl == nonchar) {
    k = bcharlabel[hf ];
    if (k == nonaddress)
      goto _Ldone;
    q = fontinfo[k].qqqq;
  } else {
    q = charinfo(hf, curl);
    if (chartag(q) != ligtag) {
      goto _Ldone;
    }
    k = ligkernstart(hf,q);
    q = fontinfo[k].qqqq;
    if (skipbyte(q) > stopflag) {
      k = ligkernrestart(hf,q);
      q = fontinfo[k].qqqq;
    }
  }
  if (currh < nonchar)
    testchar = currh;
  else
    testchar = curr;
  while (true) {
    if (nextchar(q) == testchar) {
      if (skipbyte(q) <= stopflag) {
	if (currh < nonchar) {
	  hyphenpassed = j;
	  hchar = nonchar;
	  currh = nonchar;
	  goto _Llabcontinue;
	} else {
	  if (hchar < nonchar) {
	    if (hyf[j] & 1) {
	      hyphenpassed = j;
	      hchar = nonchar;
	    }
	  }
	  if (opbyte(q) < kernflag) {   /*911:*/
	    if (curl == nonchar)
	      lfthit = true;
	    if (j == n) {
	      if (ligstack == 0)
		rthit = true;
	    }
	    checkinterrupt();
	    switch (opbyte(q)) {

	    case minquarterword + 1:
	    case minquarterword + 5:
	      curl = rembyte(q);
	      ligaturepresent = true;
	      break;

	    case minquarterword + 2:
	    case minquarterword + 6:
	      curr = rembyte(q);
	      if (ligstack > 0)
		character(ligstack) = curr;
	      else {
		ligstack = newligitem(curr);
		if (j == n)
		  bchar = nonchar;
		else {
		  p = getavail();
		  ligptr(ligstack) = p;
		  character(p) = hu[j + 1];
		  font(p) = hf;
		}
	      }
	      break;

	    case minquarterword + 3:
	      curr = rembyte(q);
	      p = ligstack;
	      ligstack = newligitem(curr);
	      link(ligstack) = p;
	      break;

	    case minquarterword + 7:
	    case minquarterword + 11:
	      wraplig(false);
	      curq = t;
	      curl = rembyte(q);
	      ligaturepresent = true;
	      break;

	    default:
	      curl = rembyte(q);
	      ligaturepresent = true;
	      if (ligstack > 0) {
		popligstack();
	      } else if (j == n)
		goto _Ldone;
	      else {
		appendcharnodetot(curr);
		j++;
		setcurr();
	      }
	      break;
	    }
	    if (opbyte(q) > minquarterword + 4) {
	      if (opbyte(q) != minquarterword + 7)
		goto _Ldone;
	    }
	    goto _Llabcontinue;
	  }
	  /*:911*/
	  w = charkern(hf, q);
	  goto _Ldone;
	}
      }
    }
    if (skipbyte(q) >= stopflag) {
      if (currh == nonchar)
	goto _Ldone;
      else {
	currh = nonchar;
	goto _Llabcontinue;
      }
    }
    k += skipbyte(q) - minquarterword + 1;
    q = fontinfo[k].qqqq;
  }
_Ldone:   /*:909*/
  /*910:*/
  wraplig(rthit);
  if (w != 0) {
    link(t) = newkern(w);
    t = link(t);
    w = 0;
  }
  if (ligstack <= 0)   /*:910*/
    return j;
  curq = t;
  curl = character(ligstack);
  ligaturepresent = true;
  popligstack();
  goto _Llabcontinue;
/* p2c: tex1.p: Note: Deleting unreachable code [255] */
}

