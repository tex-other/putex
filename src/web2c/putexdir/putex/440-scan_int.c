Static void scanint(void)
{
  boolean negative;
  long m;
  smallnumber d;
  boolean vacuous, OKsofar;

  radix = 0;
  OKsofar = true;   /*441:*/
  negative = false;
  do {   /*406:*/
    do {
      getxtoken();   /*:406*/
    } while (curcmd == spacer);
    if (curtok == othertoken + '-') {   /*:441*/
      negative = !negative;
      curtok = othertoken + '+';
    }
  } while (curtok == othertoken + '+');
  if (curtok == alphatoken) {   /*442:*/
    gettoken();
    if (curtok < cstokenflag) {
      curval = curchr;
      if (curcmd <= rightbrace) {
	if (curcmd == rightbrace)
	  alignstate++;
	else
	  alignstate--;
      }
    } else if (curtok < cstokenflag + singlebase)
      curval = curtok - cstokenflag - activebase;
    else
      curval = curtok - cstokenflag - singlebase;
    if (curval > 255) {
      printnl(S(292));
      print(S(605));
      help2(S(606),S(607));
      curval = '0';
      backerror();
    } else {   /*443:*/
      getxtoken();
      if (curcmd != spacer)
	backinput();
    }
  }  /*:442*/
  else if (curcmd >= mininternal && curcmd <= maxinternal)
    scansomethinginternal(intval, false);
  else {
    radix = 10;
    m = 214748364L;
    if (curtok == octaltoken) {
      radix = 8;
      m = 268435456L;
      getxtoken();
    } else if (curtok == hextoken) {
      radix = 16;
      m = 134217728L;
      getxtoken();
    }
    vacuous = true;
    curval = 0;   /*445:*/
    while (true) {
      if (curtok < zerotoken + radix && curtok >= zerotoken &&
	  curtok <= zerotoken + 9)
	d = curtok - zerotoken;
      else if (radix == 16) {
	if (curtok <= Atoken + 5 && curtok >= Atoken)
	  d = curtok - Atoken + 10;
	else if (curtok <= otherAtoken + 5 && curtok >= otherAtoken)
	  d = curtok - otherAtoken + 10;
	else
	  goto _Ldone;
      } else
	goto _Ldone;
      vacuous = false;
      if (curval >= m && (curval > m || d > 7 || radix != 10)) {
	if (OKsofar) {
	  printnl(S(292));
	  print(S(608));
	  help2(S(609),S(610));
	  error();
	  curval = infinity;
	  OKsofar = false;
	}
      } else
	curval = curval * radix + d;
      getxtoken();
    }
_Ldone:   /*:445*/
    if (vacuous) {   /*446:*/
      printnl(S(292));
      print(S(593));
      help3(S(594),S(595),S(596));
      backerror();
    }  /*:446*/
    else if (curcmd != spacer)
      backinput();
  }
  if (negative)
    curval = -curval;

  /*:443*/
  /*444:*/
  /*:444*/
}
