Static halfword scantoks(boolean macrodef, boolean xpand)
{
  halfword t, s, unbalance, hashbrace;
  pointer p;

  if (macrodef)
    scannerstatus = defining;
  else
    scannerstatus = absorbing;
  warningindex = curcs;
  defref = getavail();
  tokenrefcount(defref) = 0;
  p = defref;
  hashbrace = 0;
  t = zerotoken;
  if (macrodef) {   /*474:*/
    while (true) {
      gettoken();
      if (curtok < rightbracelimit)
	goto _Ldone1;
      if (curcmd == macparam) {   /*476:*/
	s = matchtoken + curchr;
	gettoken();
	if (curcmd == leftbrace) {
	  hashbrace = curtok;
	  storenewtoken(p,curtok);
	  storenewtoken(p,endmatchtoken);
	  goto _Ldone;
	}
	if (t == zerotoken + 9) {
	  printnl(S(292));
	  print(S(643));
	  help1(S(644));
	  error();
	} else {
	  t++;
	  if (curtok != t) {
	    printnl(S(292));
	    print(S(645));
	    help2(S(646),S(647));
	    backerror();
	  }
	  curtok = s;
	}
      }
      /*:476*/
      storenewtoken(p,curtok);
    }
_Ldone1:
    storenewtoken(p,endmatchtoken);
    if (curcmd == rightbrace) {   /*475:*/
      printnl(S(292));
      print(S(566));
      alignstate++;
      help2(S(648),S(649));
      error();
      goto _Lfound;
    }
    /*:475*/
_Ldone: ;
  } else
    scanleftbrace();
  /*:474*/
  /*477:*/
  unbalance = 1;
  while (true) {   /*:477*/
    if (xpand) {   /*478:*/
      while (true) {
	getnext();
	if (curcmd <= maxcommand)
	  goto _Ldone2;
	if (curcmd != the)
	  expand();
	else {
	  pointer q = thetoks();
	  if (link(temphead) != 0) {
	    link(p) = link(temphead);
	    p = q;
	  }
	}
      }
_Ldone2:
      xtoken();
    } else
      gettoken();
    /*:478*/
    if (curtok < rightbracelimit) {
      if (curcmd < rightbrace)
	unbalance++;
      else {
	unbalance--;
	if (unbalance == 0)
	  goto _Lfound;
      }
    } else if (curcmd == macparam) {
      if (macrodef) {   /*479:*/
	s = curtok;
	if (xpand)
	  getxtoken();
	else
	  gettoken();
	if (curcmd != macparam) {
	  if (curtok <= zerotoken || curtok > t) {
	    printnl(S(292));
	    print(S(650));
	    sprintcs(warningindex);
	    help3(S(651),S(652),S(653));
	    backerror();
	    curtok = s;
	  } else
	    curtok = outparamtoken - '0' + curchr;
	}
      }
      /*:479*/
    }
    storenewtoken(p,curtok);
  }
_Lfound:
  scannerstatus = normal;
  if (hashbrace != 0) {
    storenewtoken(p,hashbrace);
  }
  return p;
}
