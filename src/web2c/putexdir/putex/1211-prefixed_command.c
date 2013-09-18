Static void prefixedcommand(void)
{
  smallnumber a;
  internalfontnumber f;
  halfword j;
  fontindex k;
  pointer p, q;
  long n;
  boolean e;

  a = 0;
  while (curcmd == prefix) {
    if (!((a / curchr) & 1))
      a += curchr;
    /*404:*/
    do {
      getxtoken();
      /*:404*/
    } while (curcmd == spacer || curcmd == relax);
    if (curcmd > maxnonprefixedcommand)   /*1212:*/
      continue;
    /*:1212*/
    printnl(S(292));
    print(S(957));
    printcmdchr(curcmd, curchr);
    printchar('\'');
    help1(S(958));
    backerror();
    goto _Lexit;
  }  /*1213:*/
  if (curcmd != def && (a & 3) != 0) {   /*:1213*/
    printnl(S(292));
    print(S(602));
    printesc(S(959));
    print(S(960));
    printesc(S(961));
    print(S(962));
    printcmdchr(curcmd, curchr);
    printchar('\'');
    help1(S(963));
    error();
  }
  /*1214:*/
  if (globaldefs != 0) {
    if (globaldefs < 0) {
      if (global) {
	a -= 4;
      }
    } else {   /*:1214*/
      if (~global) {
	a += 4;
      }
    }
  }
  switch (curcmd) {   /*1217:*/

  case setfont:   /*:1217*/
    define(curfontloc, data, curchr);
    break;

  /*1218:*/
  case def:   /*:1218*/
    if ((curchr & 1) && ~global && globaldefs >= 0) {
      a += 4;
    }
    e = (curchr >= 2);
    getrtoken();
    p = curcs;
    q = scantoks(true, e);
    define(p, call + (a & 3), defref);
    break;
    /*1221:*/

  case let:
    n = curchr;
    getrtoken();
    p = curcs;
    if (n == normal) {
      do {
	gettoken();
      } while (curcmd == spacer);
      if (curtok == othertoken + '=') {
	gettoken();
	if (curcmd == spacer)
	  gettoken();
      }
    } else {
      gettoken();
      q = curtok;
      gettoken();
      backinput();
      curtok = q;
      backinput();
    }
    if (curcmd >= call) {
      addtokenref(curchr);
    }
    define(p, curcmd, curchr);
    break;

  /*:1221*/
  /*1224:*/
  case shorthanddef:
    n = curchr;
    getrtoken();
    p = curcs;
    define(p, relax, 256);
    scanoptionalequals();
    switch (n) {

    case chardefcode:
      scancharnum();
      define(p, chargiven, curval);
      break;

    case mathchardefcode:
      scanfifteenbitint();
      define(p, mathgiven, curval);
      break;

    default:
      scaneightbitint();
      switch (n) {

      case countdefcode:
	define(p, assignint, countbase + curval);
	break;

      case dimendefcode:
	define(p, assigndimen, scaledbase + curval);
	break;

      case skipdefcode:
	define(p, assignglue, skipbase + curval);
	break;

      case muskipdefcode:
	define(p, assignmuglue, muskipbase + curval);
	break;

      case toksdefcode:
	define(p, assigntoks, toksbase + curval);
	break;
      }
      break;
    }
    break;
    /*:1224*/

  /*1225:*/
  case readtocs:   /*:1225*/
    scanint();
    n = curval;
    if (!scankeyword(S(697))) {
      printnl(S(292));
      print(S(856));
      help2(S(964),S(965));
      error();
    }
    getrtoken();
    p = curcs;
    readtoks(n, p);
    define(p, call, curval);
    break;
    /*1226:*/

  case toksregister:
  case assigntoks:   /*:1226*/
    q = curcs;
    if (curcmd == toksregister) {
      scaneightbitint();
      p = toksbase + curval;
    } else
      p = curchr;
    scanoptionalequals();   /*404:*/
    do {
      getxtoken();   /*:404*/
    } while (curcmd == spacer || curcmd == relax);
    if (curcmd != leftbrace) {   /*1227:*/
      if (curcmd == toksregister) {
	scaneightbitint();
	curcmd = assigntoks;
	curchr = toksbase + curval;
      }
      if (curcmd == assigntoks) {
	q = equiv(curchr);
	if (q == 0) {
	  define(p, undefinedcs, 0);
	} else {
	  addtokenref(q);
	  define(p, call, q);
	}
	goto _Ldone;
      }
    }
    /*:1227*/
    backinput();
    curcs = q;
    q = scantoks(false, false);
    if (link(defref) == 0) {
      define(p, undefinedcs, 0);
      freeavail(defref);
    } else {
      if (p == outputroutineloc) {
	link(q) = getavail();
	q = link(q);
	info(q) = rightbracetoken + '}';
	q = getavail();
	info(q) = leftbracetoken + '{';
	link(q) = link(defref);
	link(defref) = q;
      }
      define(p, call, defref);
    }
    break;
    /*1228:*/

  case assignint:
    p = curchr;
    scanoptionalequals();
    scanint();
    worddefine(p, curval);
    break;

  case assigndimen:
    p = curchr;
    scanoptionalequals();
    scannormaldimen();
    worddefine(p, curval);
    break;

  case assignglue:
  case assignmuglue:   /*:1228*/
    p = curchr;
    n = curcmd;
    scanoptionalequals();
    if (n == assignmuglue)
      scanglue(muval);
    else
      scanglue(glueval);
    trapzeroglue();
    define(p, glueref, curval);
    break;
    /*1232:*/

  case defcode:   /*:1232*/
    /*1233:*/
    if (curchr == catcodebase)
      n = maxcharcode;
    else if (curchr == mathcodebase)
      n = 32768L;
    else if (curchr == sfcodebase)
      n = 32767;
    else if (curchr == delcodebase)
      n = 16777215L;
    else
      n = 255;   /*:1233*/
    p = curchr;
    scancharnum();
    p += curval;
    scanoptionalequals();
    scanint();
    if ( (curval < 0 && p < delcodebase) || curval > n) {
      printnl(S(292));
      print(S(966));
      printint(curval);
      if (p < delcodebase)
	print(S(967));
      else
	print(S(968));
      printint(n);
      help1(S(969));
      error();
      curval = 0;
    }
    if (p < mathcodebase) {
      define(p, data, curval);
    } else if (p < delcodebase) {
      define(p, data, curval);
    } else {
      worddefine(p, curval);
    }
    break;
    /*1234:*/

  case deffamily:   /*:1234*/
    p = curchr;
    scanfourbitint();
    p += curval;
    scanoptionalequals();
    scanfontident();
    define(p, data, curval);
    break;
    /*1235:*/

  case register_:
  case advance:
  case multiply:
  case divide:   /*:1235*/
    doregistercommand(a);
    break;
    /*1241:*/

  case setbox:   /*:1241*/
    scaneightbitint();
    if (global) {
      n = curval + 256;
    } else
      n = curval;
    scanoptionalequals();
    if (setboxallowed)
      scanbox(boxflag + n);
    else {
      printnl(S(292));
      print(S(597));
      printesc(S(970));
      help2(S(971),S(972));
      error();
    }
    break;
    /*1242:*/

  case setaux:
    alteraux();
    break;

  case setprevgraf:
    alterprevgraf();
    break;

  case setpagedimen:
    alterpagesofar();
    break;

  case setpageint:
    alterinteger();
    break;

  case setboxdimen:   /*:1242*/
    alterboxdimen();
    break;
    /*1248:*/

  case setshape:   /*:1248*/
    scanoptionalequals();
    scanint();
    n = curval;
    if (n <= 0)
      p = 0;
    else {
      p = getnode(n * 2 + 1);
      info(p) = n;
      for (j = 1; j <= n; j++) {
	scannormaldimen();
	mem[p + j * 2 - memmin - 1].sc = curval;
	scannormaldimen();
	mem[p + j * 2 - memmin].sc = curval;
      }
    }
    define(parshapeloc, shaperef, p);
    break;
    /*1252:*/

  case hyphdata:   /*:1252*/
    if (curchr == 1) {
      newpatterns();
      goto _Ldone;
/* p2c: tex1.p: Note: Deleting unreachable code [255] */
    } else {
      newhyphexceptions();
      goto _Ldone;
    }
    break;
    /*1253:*/

  case assignfontdimen:
    findfontdimen(true);
    k = curval;
    scanoptionalequals();
    scannormaldimen();
    fontinfo[k].sc = curval;
    break;

  case assignfontint:
    n = curchr;
    scanfontident();
    f = curval;
    scanoptionalequals();
    scanint();
    if (n == 0)
      set_hyphenchar(f, curval);
    else
      set_skewchar(f, curval);
    break;
    /*:1253*/

  /*1256:*/
  case deffont:
    newfont(a);
    break;

  /*:1256*/
  /*1264:*/
  case setinteraction:
    newinteraction();
    break;

  /*:1264*/
  default:
    confusion(S(973));
    break;
  }
_Ldone:
  /*1269:*/
  if (aftertoken != 0) {   /*:1269*/
    curtok = aftertoken;
    backinput();
    aftertoken = 0;
  }
_Lexit: ;
}
