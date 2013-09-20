Static void macrocall(void)
{
  pointer r, p=0 /* XXXX */, s, t, u, v, rbraceptr=0 /* XXXX */, refcount, savewarningindex;
  /* smallnumber */ int n; /* INT */
  halfword unbalance, m=0 /* XXXX */;
  smallnumber savescannerstatus;
  ASCIIcode matchchr=0 /* XXXX */;
  pointer pstack[9];

  savescannerstatus = scannerstatus;
  savewarningindex = warningindex;
  warningindex = curcs;
  refcount = curchr;
  r = link(refcount);
  n = 0;
  if (tracingmacros > 0) {   /*401:*/
    begindiagnostic();
    println();
    printcs(warningindex);
    tokenshow(refcount);
    enddiagnostic(false);
  }
  /*:401*/
  if (info(r) != endmatchtoken) {   /*391:*/
    scannerstatus = matching;
    unbalance = 0;
    longstate = eqtype(curcs);
    if (longstate >= outercall)
      longstate -= 2;
    do {
      link(temphead) = 0;
      if ((info(r) > matchtoken + (dwa_do_8-1)) | (info(r) < matchtoken))
	    /*392:*/
	      s = 0;
      else {
	matchchr = info(r) - matchtoken;
	s = link(r);
	r = s;
	p = temphead;
	m = 0;
      }
_Llabcontinue:
      gettoken();
      if (curtok == info(r)) {   /*394:*/
	r = link(r);
	if ((info(r) >= matchtoken) & (info(r) <= endmatchtoken)) {
	  if (curtok < leftbracelimit)
	    alignstate--;
	  goto _Lfound;
	} else
	  goto _Llabcontinue;
      }
      /*:394*/
      /*397:*/
      if (s != r) {
	if (s == 0) {   /*398:*/
	  printnl(S(292));
	  print(S(533));
	  sprintcs(warningindex);
	  print(S(534));
	  help4(S(535),S(536),S(537),S(538));
	  error();
	  goto _Lexit;
	}
	/*:398*/
	t = s;
	do {
	  storenewtoken(p,info(t));
	  m++;
	  u = link(t);
	  v = s;
	  while (true) {
	    if (u == r) {
	      if (curtok != info(v))
		goto _Ldone;
	      else {
		r = link(v);
		goto _Llabcontinue;
	      }
	    }
	    if (info(u) != info(v))
	      goto _Ldone;
	    u = link(u);
	    v = link(v);
	  }
_Ldone:
	  t = link(t);
	} while (t != r);
	r = s;
      }
      if (curtok == partoken) {
	if (longstate != longcall) {   /*396:*/
	  if (longstate == call) {
	    runaway();
	    printnl(S(292));
	    print(S(539));
	    sprintcs(warningindex);
	    print(S(540));
	    help3(S(541),S(542),S(543));
	    backerror();
	  }
	  pstack[n] = link(temphead);
	  alignstate -= unbalance;
	  for (m = 0; m <= n; m++)
	    flushlist(pstack[m]);
	  goto _Lexit;
	}
	/*:396*/
      }
      if (curtok < rightbracelimit) {
	if (curtok < leftbracelimit) {   /*399:*/
	  unbalance = 1;
	  while (true) {
	    faststorenewtoken(p,curtok);
	    gettoken();
	    if (curtok == partoken) {
	      if (longstate != longcall) {   /*396:*/
		if (longstate == call) {
		  runaway();
		  printnl(S(292));
		  print(S(539));
		  sprintcs(warningindex);
		  print(S(540));
		  help3(S(541),S(542),S(543));
		  backerror();
		}
		pstack[n] = link(temphead);
		alignstate -= unbalance;
		for (m = 0; m <= n; m++)
		  flushlist(pstack[m]);
		goto _Lexit;
	      }
	      /*:396*/
	    }
	    if (curtok >= rightbracelimit)
	      continue;
	    if (curtok < leftbracelimit)
	      unbalance++;
	    else {
	      unbalance--;
	      if (unbalance == 0)
		goto _Ldone1;
	    }
	  }
_Ldone1:
	  rbraceptr = p;
	  storenewtoken(p,curtok);
	} else {   /*395:*/
	  backinput();
	  printnl(S(292));
	  print(S(544));
	  sprintcs(warningindex);
	  print(S(545));
	  help6(S(546),S(547),S(548),S(549),S(550),S(551));
	  alignstate++;
	  longstate = call;
	  curtok = partoken;
	  inserror();
	}
	/*:399*/
      } else  /*393:*/
      {   /*:393*/
	if (curtok == spacetoken) {
	  if (info(r) <= endmatchtoken) {
	    if (info(r) >= matchtoken)
	      goto _Llabcontinue;
	  }
	}
	storenewtoken(p,curtok);
      }
      m++;
      if (info(r) > endmatchtoken)
	goto _Llabcontinue;
      if (info(r) < matchtoken)
	goto _Llabcontinue;
_Lfound:
      if (s != 0) {   /*400:*/
	if (((m == 1) & (info(p) < rightbracelimit)) && p != temphead) {
	  link(rbraceptr) = 0;
	  freeavail(p);
	  p = link(temphead);
	  pstack[n] = link(p);
	  freeavail(p);
	} else
	  pstack[n] = link(temphead);
	n++;
	if (tracingmacros > 0) {
	  begindiagnostic();
	  printnl(matchchr);
	  printint(n);
	  print(S(552));
	  showtokenlist(pstack[n - 1], 0, 1000);
	  enddiagnostic(false);
	}
      }
      /*:400*/
      /*:392*/
    } while (info(r) != endmatchtoken);
  }
  /*:391*/
  /*390:*/
  while (state == tokenlist && loc == 0)
    endtokenlist();
  begintokenlist(refcount, macro);
  name = warningindex;
  loc = link(r);
  if (n > 0) {   /*:390*/
    if (paramptr + n > maxparamstack) {
      maxparamstack = paramptr + n;
      if (maxparamstack > paramsize)
	overflow(S(553), paramsize);
    }
    for (m = 0; m < n; m++)
      paramstack[paramptr + m] = pstack[m];
    paramptr += n;
  }
_Lexit:
  scannerstatus = savescannerstatus;
  warningindex = savewarningindex;

  /*:397*/
  /*:395*/
}
