Static void initalign(void)
{
  pointer savecsptr, p;

  savecsptr = curcs;
  pushalignment();
  alignstate = -1000000L;   /*776:*/
  if (mode == mmode && (tail != head || incompleatnoad != 0)) {   /*:776*/
    printnl(S(292));
    print(S(597));
    printesc(S(724));
    print(S(725));
    help3(S(726),S(727),S(728));
    error();
    flushmath();
  }
  pushnest();   /*775:*/
  if (mode == mmode) {
    mode = -vmode;
    prevdepth = nest[nestptr - 2].auxfield.sc;
  } else if (mode > 0)
    mode = -mode;   /*:775*/
  scanspec(aligngroup, false);
  /*777:*/
  preamble = 0;
  curalign = alignhead;
  curloop = 0;
  scannerstatus = aligning;
  warningindex = savecsptr;
  alignstate = -1000000L;
  while (true) {   /*778:*/
    link(curalign) = newparamglue(tabskipcode);
    curalign = link(curalign);   /*:778*/
    if (curcmd == carret)
      goto _Ldone;
    /*779:*/
    /*783:*/
    p = holdhead;
    link(p) = 0;
    while (true) {
      getpreambletoken();
      if (curcmd == macparam)
	goto _Ldone1;
      if (curcmd <= carret && curcmd >= tabmark && alignstate == -1000000L) {
	if (p == holdhead && curloop == 0 && curcmd == tabmark) {
	  curloop = curalign;
	  continue;
	} else {
	  printnl(S(292));
	  print(S(729));
	  help3(S(730),S(731),S(732));
	  backerror();
	  goto _Ldone1;
	}
      } else {
	if (curcmd != spacer || p != holdhead) {
	  link(p) = getavail();
	  p = link(p);
	  info(p) = curtok;
	}
	continue;
      }
    }
_Ldone1:   /*:783*/
    link(curalign) = newnullbox();
    curalign = link(curalign);
    info(curalign) = endspan;
    width(curalign) = nullflag;
    upart(curalign) = link(holdhead);   /*784:*/
    p = holdhead;
    link(p) = 0;
    while (true) {
_Llabcontinue:
      getpreambletoken();
      if (curcmd <= carret && curcmd >= tabmark && alignstate == -1000000L)
	goto _Ldone2;
      if (curcmd == macparam) {
	printnl(S(292));
	print(S(733));
	help3(S(730),S(731),S(734));
	error();
	goto _Llabcontinue;
      }
      link(p) = getavail();
      p = link(p);
      info(p) = curtok;
    }
_Ldone2:
    link(p) = getavail();
    p = link(p);
    info(p) = endtemplatetoken;   /*:784*/
    vpart(curalign) = link(holdhead);   /*:779*/
  }
_Ldone:
  scannerstatus = normal;   /*:777*/
  newsavelevel(aligngroup);
  if (everycr != 0)
    begintokenlist(everycr, everycrtext);
  alignpeek();
}
