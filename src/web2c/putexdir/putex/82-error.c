void error(void)
{
  ASCIIcode c;
  long s1, s2, s3, s4;

  if (history < errormessageissued)
    history = errormessageissued;
  printchar('.');
  showcontext();
  if (interaction == errorstopmode) {   /*83:*/
    while (true) {   /*:83*/
_Llabcontinue:
      clearforerrorprompt();
      print(S(269));
      terminput();
      if (last == first)
	goto _Lexit;
      c = buffer[first];
      if (c >= 'a')   /*84:*/
	c += 'A' - 'a';
      switch (c) {

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':   /*_DEBUG*/
	if (deletionsallowed) {   /*88:*/
	  s1 = curtok;
	  s2 = curcmd;
	  s3 = curchr;
	  s4 = alignstate;
	  alignstate = 1000000L;
	  OKtointerrupt = false;
	  if ((last > first + 1) & (buffer[first + 1] >= '0') &
	      (buffer[first + 1] <= '9'))
	    c = c * 10 + buffer[first + 1] - '0' * 11;
	  else
	    c -= '0';
	  while (c > 0) {
	    gettoken();
	    c--;
	  }
	  curtok = s1;
	  curcmd = s2;
	  curchr = s3;
	  alignstate = s4;
	  OKtointerrupt = true;
	  help2(S(270),S(271));
	  showcontext();
	  goto _Llabcontinue;
	}
	/*:88*/
	break;

      case 'D':
	debughelp();
	goto _Llabcontinue;
	break;
	/*_ENDDEBUG*/

      case 'E':
	if (baseptr > 0) {
	  printnl(S(272));
	  slowprint(inputstack[baseptr].namefield);
	  print(S(273));
	  printint(line);
	  interaction = scrollmode;
	  jumpout();
	}
	break;

      case 'H':   /*89:*/
	if (useerrhelp) {
	  giveerrhelp();
	  useerrhelp = false;
	} else {
	  if (helpptr == 0) {
	    help2(S(274),S(275));
	  }
	  do {
	    helpptr--;
	    print(helpline[helpptr]);
	    println();
	  } while (helpptr != 0);
	}
	help4(S(276),S(275),S(277),S(278));
	goto _Llabcontinue;
	break;
	/*:89*/

      case 'I':   /*87:*/
	beginfilereading();
	if (last > first + 1) {
	  loc = first + 1;
	  buffer[first] = ' ';
	} else {
	  print(S(279));
	  terminput();
	  loc = first;
	}
	first = last;
	curinput.limitfield = last - 1;
	goto _Lexit;
	break;
	/*:87*/

      case 'Q':
      case 'R':
      case 'S':   /*86:*/
	errorcount = 0;
	interaction = batchmode + c - 'Q';
	print(S(280));
	switch (c) {

	case 'Q':
	  printesc(S(281));
	  selector--;
	  break;

	case 'R':
	  printesc(S(282));
	  break;

	case 'S':
	  printesc(S(283));
	  break;
	}
	print(S(284));
	println();
	fflush(termout);
	goto _Lexit;
	break;
	/*:86*/

      case 'X':
	interaction = scrollmode;
	jumpout();
	break;
      }/*85:*/
      print(S(285));
      printnl(S(286));
      printnl(S(287));
      if (baseptr > 0)
	print(S(288));
      if (deletionsallowed)
	printnl(S(289));
      printnl(S(290));   /*:85*/
      /*:84*/
    }
  }
  errorcount++;
  if (errorcount == 100) {
    printnl(S(291));
    history = fatalerrorstop;
    jumpout();
  }  /*90:*/
  if (interaction > batchmode)
    selector--;
  if (useerrhelp) {
    println();
    giveerrhelp();
  } else {
    while (helpptr > 0) {
      helpptr--;
      printnl(helpline[helpptr]);
    }
  }
  println();
  if (interaction > batchmode)   /*:90*/
    selector++;
  println();
_Lexit: ;
}
