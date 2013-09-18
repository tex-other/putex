Static void readtoks(long n, halfword r)
{
  pointer p;
  long s;
  /* smallnumber */ int m; /* INT */

  scannerstatus = defining;
  warningindex = r;
  defref = getavail();
  tokenrefcount(defref) = 0;
  p = defref;
  storenewtoken(p,endmatchtoken);
  if ((unsigned long)n > 15)
    m = 16;
  else
    m = n;
  s = alignstate;
  alignstate = 1000000L;
  do {   /*483:*/
    beginfilereading();
    name = m + 1;
    if (readopen[m] == closed) {   /*484:*/
      if (interaction > nonstopmode) {
	if (n < 0) {
	  print(S(385));
	  terminput();
	} else {
	  println();
	  sprintcs(r);
	  print('=');
	  terminput();
	  n = -1;
	}
      } else   /*:484*/
	fatalerror(S(654));
    } else if (readopen[m] == justopen) {
      if (inputln(&readfile[m], false))
	readopen[m] = normal;
      else {   /*:485*/
	aclose(&readfile[m]);
	readopen[m] = closed;
      }
    } else {
      if (!inputln(&readfile[m], true)) {
	aclose(&readfile[m]);
	readopen[m] = closed;
	if (alignstate != 1000000L) {
	  runaway();
	  printnl(S(292));
	  print(S(655));
	  printesc(S(656));
	  help1(S(657));
	  alignstate = 1000000L;
	  error();
	}
      }
    }
    limit = last;
    if (endlinecharinactive) {
      limit--;
    } else
      buffer[limit] = endlinechar;
    first = limit + 1;
    loc = start;
    state = newline;
    while (true) {
      gettoken();
      if (curtok == 0)
	goto _Ldone;
      if (alignstate < 1000000L) {
	do {
	  gettoken();
	} while (curtok != 0);
	alignstate = 1000000L;
	goto _Ldone;
      }
      storenewtoken(p,curtok);
    }
_Ldone:   /*:483*/
    endfilereading();
  } while (alignstate != 1000000L);
  curval = defref;
  scannerstatus = normal;
  alignstate = s;

  /*485:*/
  /*486:*/
  /*:486*/
}

