Static void showtokenlist(long p, long q, long l)
{
  long m, c;
  ASCIIcode matchchr, n;

  matchchr = '#';
  n = '0';
  tally = 0;
  while (p != 0 && tally < l) {
    if (p == q) {   /*320:*/
      settrickcount();
    }
    /*:320*/
    /*293:*/
    if (p < himemmin || p > memend) {
      printesc(S(308));
      goto _Lexit;
    }
    if (info(p) >= cstokenflag)
      printcs(info(p) - cstokenflag);
    else {   /*:293*/
      m = info(p) / dwa_do_8;
      c = (info(p)) & (dwa_do_8-1);
      if (info(p) < 0)
	printesc(S(309));
      else {  /*294:*/
	switch (m) {   /*:294*/

	case leftbrace:
	case rightbrace:
	case mathshift:
	case tabmark:
	case supmark:
	case submark:
	case spacer:
	case letter:
	case otherchar:
	  print(c);
	  break;

	case macparam:
	  print(c);
	  print(c);
	  break;

	case outparam:
	  print(matchchr);
	  if (c > 9) {
	    printchar('!');
	    goto _Lexit;
	  }
	  printchar(c + '0');
	  break;

	case match:
	  matchchr = c;
	  print(c);
	  n++;
	  printchar(n);
	  if (n > '9')
	    goto _Lexit;
	  break;

	case endmatch:
	  print(S(310));
	  break;

	default:
	  printesc(S(309));
	  break;
	}
      }
    }
    p = link(p);
  }
  if (p != 0)
    printesc(S(311));
_Lexit: ;
}
