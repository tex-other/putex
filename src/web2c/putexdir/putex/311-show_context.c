Static void showcontext(void)
{   /*:315*/
  char oldsetting;
  long nn;
  boolean bottomline;   /*315:*/
  short i;
  short j;
  char l;
  long m;
  char n;
  long p, q;

  baseptr = inputptr;
  inputstack[baseptr] = curinput;
  nn = -1;
  bottomline = false;
  while (true) {
    curinput = inputstack[baseptr];
    if (state != tokenlist) {
      if (name > 17 || baseptr == 0)
	bottomline = true;
    }
    if (baseptr == inputptr || bottomline || nn < errorcontextlines) {
	  /*312:*/
	    if (baseptr == inputptr || state != tokenlist ||
		tokentype != backedup || loc != 0) {
	tally = 0;
	oldsetting = selector;
	if (state != tokenlist) {  /*313:*/
	  if (name <= 17) {   /*:313*/
	    if (terminalinput) {
	      if (baseptr == 0)
		printnl(S(489));
	      else
		printnl(S(490));
	    } else {
	      printnl(S(491));
	      if (name == 17)
		printchar('*');
	      else
		printint(name - 1);
	      printchar('>');
	    }
	  } else {
	    printnl(S(492));
	    printint(line);
	  }
	  printchar(' ');   /*318:*/
	  beginpseudoprint();
	  if (buffer[limit] == endlinechar)
	    j = limit;
	  else
	    j = limit + 1;
	  if (j > 0) {
	    for (i = start; i < j; i++) {   /*:318*/
	      if (i == loc) {
		settrickcount();
	      }
	      print(buffer[i]);
	    }
	  }
	} else {  /*314:*/
	  switch (tokentype) {   /*:314*/

	  case parameter:
	    printnl(S(493));
	    break;

	  case utemplate:
	  case vtemplate:
	    printnl(S(494));
	    break;

	  case backedup:
	    if (loc == 0)
	      printnl(S(495));
	    else
	      printnl(S(496));
	    break;

	  case inserted:
	    printnl(S(497));
	    break;

	  case macro:
	    println();
	    printcs(name);
	    break;

	  case outputtext:
	    printnl(S(498));
	    break;

	  case everypartext:
	    printnl(S(499));
	    break;

	  case everymathtext:
	    printnl(S(500));
	    break;

	  case everydisplaytext:
	    printnl(S(501));
	    break;

	  case everyhboxtext:
	    printnl(S(502));
	    break;

	  case everyvboxtext:
	    printnl(S(503));
	    break;

	  case everyjobtext:
	    printnl(S(504));
	    break;

	  case everycrtext:
	    printnl(S(505));
	    break;

	  case marktext:
	    printnl(S(506));
	    break;

	  case writetext:
	    printnl(S(507));
	    break;

	  default:
	    printnl('?');
	    break;
	  }
	  /*319:*/
	  beginpseudoprint();
	  if (tokentype < macro)
	    showtokenlist(start, loc, 100000L);
	  else   /*:319*/
	    showtokenlist(link(start), loc, 100000L);
	}
	selector = oldsetting;   /*317:*/
	if (trickcount == 1000000L) {
	  settrickcount();
	}
	if (tally < trickcount)
	  m = tally - firstcount;
	else
	  m = trickcount - firstcount;
	if (l + firstcount <= halferrorline) {
	  p = 0;
	  n = l + firstcount;
	} else {
	  print(S(284));
	  p = l + firstcount - halferrorline + 3;
	  n = halferrorline;
	}
	for (q = p; q < firstcount; q++) {
	  printchar(trickbuf[q % errorline]);
	}
	println();
	for (q = 1; q <= n; q++)
	  printchar(' ');
	if (m + n <= errorline)
	  p = firstcount + m;
	else
	  p = firstcount + errorline - n - 3;
	for (q = firstcount; q < p; q++) {
/* p2c: tex1.p: Note: Using % for possibly-negative arguments [317] */
	  printchar(trickbuf[q % errorline]);
	}
	if (m + n > errorline)   /*:317*/
	  print(S(284));
	nn++;
      }
    }  /*:312*/
    else if (nn == errorcontextlines) {
      printnl(S(284));
      nn++;
    }
    if (bottomline)
      goto _Ldone;
    baseptr--;
  }
_Ldone:
  curinput = inputstack[inputptr];
}
