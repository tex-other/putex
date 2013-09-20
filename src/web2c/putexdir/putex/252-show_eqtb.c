Static void showeqtb(halfword n)
{
  if (n < activebase) {
    printchar('?');
    return;
  }
  if (n < gluebase) {
    /*
223:*/
    sprintcs(n);
    printchar('=');
    printcmdchr(eqtype(n), equiv(n));
    if (eqtype(n) >= call) {
      printchar(':');
      showtokenlist(link(equiv(n)), 0, 32);
    }
    return;
  }  /*:223*/
  if (n < localbase) {   /*229:*/
    if (n < skipbase) {
      printskipparam(n - gluebase);
      printchar('=');
      if (n < gluebase + thinmuskipcode)
	printspec(equiv(n), S(459));
      else
	printspec(equiv(n), S(390));
      return;
    }
    if (n < muskipbase) {
      printesc(S(460));
      printint(n - skipbase);
      printchar('=');
      printspec(equiv(n), S(459));
      return;
    }
    printesc(S(461));
    printint(n - muskipbase);
    printchar('=');
    printspec(equiv(n), S(390));
    return;
  }
  if (n < intbase) {   /*233:*/
    if (n == parshapeloc) {
      printesc(S(462));
      printchar('=');
      if (parshapeptr == 0)
	printchar('0');
      else
	printint(info(parshapeptr));
      return;
    }
    if (n < toksbase) {
      printcmdchr(assigntoks, n);
      printchar('=');
      if (equiv(n) != 0)
	showtokenlist(link(equiv(n)), 0, 32);
      return;
    }
    if (n < boxbase) {
      printesc(S(463));
      printint(n - toksbase);
      printchar('=');
      if (equiv(n) != 0)
	showtokenlist(link(equiv(n)), 0, 32);
      return;
    }
    if (n < curfontloc) {
      printesc(S(464));
      printint(n - boxbase);
      printchar('=');
      if (equiv(n) == 0) {
	print(S(465));
	return;
      }
      depththreshold = 0;
      breadthmax = 1;
      shownodelist(equiv(n));
      return;
    }
    if (n < catcodebase) {   /*234:*/
      if (n == curfontloc)
	print(S(466));
      else if (n < mathfontbase + 16) {
	printesc(S(266));
	printint(n - mathfontbase);
      } else if (n < mathfontbase + 32) {
	printesc(S(267));
	printint(n - mathfontbase - 16);
      } else {
	printesc(S(268));
	printint(n - mathfontbase - 32);
      }
      printchar('=');
/*      printesc(hash[fontidbase + equiv(n) - hashbase].rh);*/
        printesc(fontidtext(equiv(n)));
      return;
    }
    /*:234*/
    if (n < mathcodebase) {
      if (n < lccodebase) {
	printesc(S(467));
	printint(n - catcodebase);
      } else if (n < uccodebase) {
	printesc(S(468));
	printint(n - lccodebase);
      } else if (n < sfcodebase) {
	printesc(S(469));
	printint(n - uccodebase);
      } else {
	printesc(S(470));
	printint(n - sfcodebase);
      }
      printchar('=');
      printint(equiv(n));
      return;
    }
    printesc(S(471));
    printint(n - mathcodebase);
    printchar('=');
    printint(equiv(n));
    return;
  }
  if (n < dimenbase) {   /*242:*/
    if (n < countbase)
      printparam(n - intbase);
    else if (n < delcodebase) {
      printesc(S(472));
      printint(n - countbase);
    } else {
      printesc(S(473));
      printint(n - delcodebase);
    }
    printchar('=');
    printint(eqtb[n - activebase].int_);
    return;
  }  /*:242*/
  if (n > eqtbsize) {   /*251:*/
    printchar('?');
    return;
  }
  /*:251*/
  if (n < scaledbase)
    printlengthparam(n - dimenbase);
  else {
    printesc(S(474));
    printint(n - scaledbase);
  }
  printchar('=');
  printscaled(eqtb[n - activebase].sc);
  print(S(459));

  /*:229*/
  /*235:*/
  /*:235*/
  /*:233*/
}
