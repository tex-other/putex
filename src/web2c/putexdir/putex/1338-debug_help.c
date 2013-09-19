Static void debughelp(void)
{
  long k, l, m, n;

  while (true) {
    printnl(S(1253));
    fflush(termout);
    fscanf(termin," %ld",&m);
    if (m < 0) {
      goto _Lexit;
      continue;
    }
    if (m == 0) {
      goto _Lbreakpoint_;
_Lbreakpoint_:
      m = 0;   /*'BREAKPOINT'*/
      continue;
    }
    fscanf(termin," %ld",&n);
    switch (m) {   /*1339:*/

    case 1:
      printword(mem[n - memmin]);
      break;

    case 2:
      printint(info(n));
      break;

    case 3:
      printint(link(n));
      break;

    case 4:
      printword(eqtb[n - activebase]);
      break;

    case 5:
      printword(fontinfo[n]);
      break;

    case 6:
      printword(savestack[n]);
      break;

    case 7:
      showbox(n);
      break;

    case 8:
      breadthmax = 10000;
	depththreshold = str_adjust_to_room(poolsize) - 10;
      shownodelist(n);
      break;

    case 9:
      showtokenlist(n, 0, 1000);
      break;

    case 10:
      slowprint(n);
      break;

    case 11:
      checkmem(n > 0);
      break;

    case 12:
      searchmem(n);
      break;

    case 13:
      fscanf(termin, " %ld", &l);
      printcmdchr(n, l);
      break;

    case 14:
      for (k = 0; k <= n; k++)
	print(buffer[k]);
      break;

    case 15:
      fontinshortdisplay = nullfont;
      shortdisplay(n);
      break;

    case 16:   /*:1339*/
      panicking = !panicking;
      break;

    default:
      print('?');
      break;
    }
  }
_Lexit: ;
}
