Static void maincontrol(void)
{
  long t;

  if (everyjob != 0)
    begintokenlist(everyjob, everyjobtext);
_Lbigswitch_:
  getxtoken();
_Lreswitch:   /*1031:*/
  if (interrupt != 0) {
    if (OKtointerrupt) {
      backinput();
      checkinterrupt();
      goto _Lbigswitch_;
    }  /*_DEBUG*/
  }
  if (panicking)   /*_ENDDEBUG*/
    checkmem(false);
  if (tracingcommands > 0)   /*:1031*/
    showcurcmdchr();
  switch (labs(mode) + curcmd) {

  case hmode + letter:
  case hmode + otherchar:
  case hmode + chargiven:
    goto _Lmainloop;
    break;

  case hmode + charnum:
    scancharnum();
    curchr = curval;
    goto _Lmainloop;
    break;

  case hmode + noboundary:
    getxtoken();
    if (curcmd == letter || curcmd == otherchar || curcmd == chargiven ||
	curcmd == charnum)
      cancelboundary = true;
    goto _Lreswitch;
    break;

  case hmode + spacer:
    if (spacefactor == 1000)
      goto _Lappendnormalspace_;
    appspace();
    break;

  case hmode + exspace:
  case mmode + exspace:   /*1045:*/
    goto _Lappendnormalspace_;
    break;

  case vmode:
  case hmode:
  case mmode:
  case vmode + spacer:
  case mmode + spacer:
  case mmode + noboundary:
    /* blank case */
    break;

  case vmode + ignorespaces:
  case hmode + ignorespaces:
  case mmode + ignorespaces:  /*406:*/
    do {
      getxtoken();   /*:406*/
    } while (curcmd == spacer);
    goto _Lreswitch;
    break;

  case vmode + stop:   /*1048:*/
    if (itsallover())
      goto _Lexit;
    break;

  case vmode + vmove:
  case hmode + hmove:
  case mmode + hmove:
  case vmode + lastitem:
  case hmode + lastitem:
  case mmode + lastitem:
  case vmode + vadjust:
  case vmode + italcorr:
  case vmode + eqno:
  case hmode + eqno:
  case vmode + macparam:
  case hmode + macparam:
  case mmode + macparam:   /*:1048*/
    reportillegalcase();
    break;
    /*1098:*/
    /*:1098*/
    /*1111:*/
    /*:1111*/
    /*1144:*/
    /*:1144*/
    /*1046:*/

  case vmode + supmark:
  case hmode + supmark:
  case vmode + submark:
  case hmode + submark:
  case vmode + mathcharnum:
  case hmode + mathcharnum:
  case vmode + mathgiven:
  case hmode + mathgiven:
  case vmode + mathcomp:
  case hmode + mathcomp:
  case vmode + delimnum:
  case hmode + delimnum:
  case vmode + leftright:
  case hmode + leftright:
  case vmode + above:
  case hmode + above:
  case vmode + radical:
  case hmode + radical:
  case vmode + mathstyle:
  case hmode + mathstyle:
  case vmode + mathchoice:
  case hmode + mathchoice:
  case vmode + vcenter:
  case hmode + vcenter:
  case vmode + nonscript:
  case hmode + nonscript:
  case vmode + mkern:
  case hmode + mkern:
  case vmode + limitswitch:
  case hmode + limitswitch:
  case vmode + mskip:
  case hmode + mskip:
  case vmode + mathaccent:
  case hmode + mathaccent:
  case mmode + endv:
  case mmode + parend:
  case mmode + stop:
  case mmode + vskip:
  case mmode + unvbox:
  case mmode + valign:
  case mmode + hrule:   /*:1046*/
    insertdollarsign();
    break;

  /*1056:*/
  case vmode + hrule:
  case hmode + vrule:
  case mmode + vrule:   /*:1056*/
    tailappend(scanrulespec());
    if (labs(mode) == vmode)
      prevdepth = ignoredepth;
    else if (labs(mode) == hmode)
      spacefactor = 1000;
    break;
    /*1057:*/

  case vmode + vskip:
  case hmode + hskip:
  case mmode + hskip:
  case mmode + mskip:
    appendglue();
    break;

  case vmode + kern:
  case hmode + kern:
  case mmode + kern:
  case mmode + mkern:   /*:1057*/
    appendkern();
    break;
    /*1063:*/

  case vmode + leftbrace:
  case hmode + leftbrace:
    newsavelevel(simplegroup);
    break;

  case vmode + begingroup:
  case hmode + begingroup:
  case mmode + begingroup:
    newsavelevel(semisimplegroup);
    break;

  case vmode + endgroup:
  case hmode + endgroup:
  case mmode + endgroup:   /*:1063*/
    if (curgroup == semisimplegroup)
      unsave();
    else
      offsave();
    break;
    /*1067:*/

  case vmode + rightbrace:
  case hmode + rightbrace:
  case mmode + rightbrace:
    handlerightbrace();
    break;

  /*:1067*/
  /*1073:*/
  case vmode + hmove:
  case hmode + vmove:
  case mmode + vmove:
    t = curchr;
    scannormaldimen();
    if (t == 0)
      scanbox(curval);
    else
      scanbox(-curval);
    break;

  case vmode + leadership:
  case hmode + leadership:
  case mmode + leadership:
    scanbox(leaderflag - aleaders + curchr);
    break;

  case vmode + makebox:
  case hmode + makebox:
  case mmode + makebox:
    beginbox(0);
    break;

  /*:1073*/
  /*1090:*/
  case vmode + startpar:
    newgraf(curchr > 0);
    break;

  case vmode + letter:
  case vmode + otherchar:
  case vmode + charnum:
  case vmode + chargiven:
  case vmode + mathshift:
  case vmode + unhbox:
  case vmode + vrule:
  case vmode + accent:
  case vmode + discretionary:
  case vmode + hskip:
  case vmode + valign:
  case vmode + exspace:
  case vmode + noboundary:   /*:1090*/
    backinput();
    newgraf(true);
    break;
    /*1092:*/

  case hmode + startpar:
  case mmode + startpar:   /*:1092*/
    indentinhmode();
    break;
    /*1094:*/

  case vmode + parend:
    normalparagraph();
    if (mode > 0)
      buildpage();
    break;

  case hmode + parend:
    if (alignstate < 0)
      offsave();
    endgraf();
    if (mode == vmode)
      buildpage();
    break;

  case hmode + stop:
  case hmode + vskip:
  case hmode + hrule:
  case hmode + unvbox:
  case hmode + halign:   /*:1094*/
    headforvmode();
    break;
    /*1097:*/

  case vmode + insert_:
  case hmode + insert_:
  case mmode + insert_:
  case hmode + vadjust:
  case mmode + vadjust:
    begininsertoradjust();
    break;

  case vmode + mark_:
  case hmode + mark_:
  case mmode + mark_:   /*:1097*/
    makemark();
    break;

  /*1102:*/
  case vmode + breakpenalty:
  case hmode + breakpenalty:
  case mmode + breakpenalty:
    appendpenalty();
    break;

  /*:1102*/
  /*1104:*/
  case vmode + removeitem:
  case hmode + removeitem:
  case mmode + removeitem:   /*:1104*/
    deletelast();
    break;

  /*1109:*/
  case vmode + unvbox:
  case hmode + unhbox:
  case mmode + unhbox:
    unpackage();
    break;

  /*:1109*/
  /*1112:*/
  case hmode + italcorr:
    appenditaliccorrection();
    break;

  case mmode + italcorr:   /*:1112*/
    tailappend(newkern(0));
    break;
    /*1116:*/

  case hmode + discretionary:
  case mmode + discretionary:   /*:1116*/
    appenddiscretionary();
    break;

  /*1122:*/
  case hmode + accent:
    makeaccent();
    break;

  /*:1122*/
  /*1126:*/
  case vmode + carret:
  case hmode + carret:
  case mmode + carret:
  case vmode + tabmark:
  case hmode + tabmark:
  case mmode + tabmark:
    alignerror();
    break;

  case vmode + noalign:
  case hmode + noalign:
  case mmode + noalign:
    noalignerror();
    break;

  case vmode + omit:
  case hmode + omit:
  case mmode + omit:   /*:1126*/
    omiterror();
    break;
    /*1130:*/

  case vmode + halign:
  case hmode + valign:
    initalign();
    break;

  case mmode + halign:
    if (privileged()) {
      if (curgroup == mathshiftgroup)
	initalign();
      else
	offsave();
    }
    break;

  case vmode + endv:
  case hmode + endv:   /*:1130*/
    doendv();
    break;
    /*1134:*/

  case vmode + endcsname:
  case hmode + endcsname:
  case mmode + endcsname:   /*:1134*/
    cserror();
    break;
    /*1137:*/

  case hmode + mathshift:   /*:1137*/
    initmath();
    break;
    /*1140:*/

  case mmode + eqno:   /*:1140*/
    if (privileged()) {
      if (curgroup == mathshiftgroup)
	starteqno();
      else
	offsave();
    }
    break;
    /*1150:*/

  case mmode + leftbrace:   /*:1150*/
    tailappend(newnoad());
    backinput();
    scanmath(nucleus(tail));
    break;
    /*1154:*/

  case mmode + letter:
  case mmode + otherchar:
  case mmode + chargiven:
    setmathchar(mathcode(curchr));
    break;

  case mmode + charnum:
    scancharnum();
    curchr = curval;
    setmathchar(mathcode(curchr));
    break;

  case mmode + mathcharnum:
    scanfifteenbitint();
    setmathchar(curval);
    break;

  case mmode + mathgiven:
    setmathchar(curchr);
    break;

  case mmode + delimnum:   /*:1154*/
    scantwentysevenbitint();
    setmathchar(curval / 4096);
    break;
    /*1158:*/

  case mmode + mathcomp:
    tailappend(newnoad());
    type(tail) = curchr;
    scanmath(nucleus(tail));
    break;

  case mmode + limitswitch:   /*:1158*/
    mathlimitswitch();
    break;
    /*1162:*/

  case mmode + radical:   /*:1162*/
    mathradical();
    break;
    /*1164:*/

  case mmode + accent:
  case mmode + mathaccent:   /*:1164*/
    mathac();
    break;
    /*1167:*/

  case mmode + vcenter:
    scanspec(vcentergroup, false);
    normalparagraph();
    pushnest();
    mode = -vmode;
    prevdepth = ignoredepth;
    if (everyvbox != 0)
      begintokenlist(everyvbox, everyvboxtext);
    break;
    /*:1167*/

  /*1171:*/
  case mmode + mathstyle:
    tailappend(newstyle(curchr));
    break;

  case mmode + nonscript:
    tailappend(newglue(zeroglue));
    subtype(tail) = condmathglue;
    break;

  case mmode + mathchoice:
    appendchoices();
    break;

  /*:1171*/
  /*1175:*/
  case mmode + submark:
  case mmode + supmark:
    subsup();
    break;

  /*:1175*/
  /*1180:*/
  case mmode + above:   /*:1180*/
    mathfraction();
    break;
    /*1190:*/

  case mmode + leftright:
    mathleftright();
    break;

  /*:1190*/
  /*1193:*/
  case mmode + mathshift:
    if (curgroup == mathshiftgroup)
      aftermath();
    else
      offsave();
    break;

  /*:1193*/
  /*1210:*/
  case vmode + toksregister:
  case hmode + toksregister:
  case mmode + toksregister:
  case vmode + assigntoks:
  case hmode + assigntoks:
  case mmode + assigntoks:
  case vmode + assignint:
  case hmode + assignint:
  case mmode + assignint:
  case vmode + assigndimen:
  case hmode + assigndimen:
  case mmode + assigndimen:
  case vmode + assignglue:
  case hmode + assignglue:
  case mmode + assignglue:
  case vmode + assignmuglue:
  case hmode + assignmuglue:
  case mmode + assignmuglue:
  case vmode + assignfontdimen:
  case hmode + assignfontdimen:
  case mmode + assignfontdimen:
  case vmode + assignfontint:
  case hmode + assignfontint:
  case mmode + assignfontint:
  case vmode + setaux:
  case hmode + setaux:
  case mmode + setaux:
  case vmode + setprevgraf:
  case hmode + setprevgraf:
  case mmode + setprevgraf:
  case vmode + setpagedimen:
  case hmode + setpagedimen:
  case mmode + setpagedimen:
  case vmode + setpageint:
  case hmode + setpageint:
  case mmode + setpageint:
  case vmode + setboxdimen:
  case hmode + setboxdimen:
  case mmode + setboxdimen:
  case vmode + setshape:
  case hmode + setshape:
  case mmode + setshape:
  case vmode + defcode:
  case hmode + defcode:
  case mmode + defcode:
  case vmode + deffamily:
  case hmode + deffamily:
  case mmode + deffamily:
  case vmode + setfont:
  case hmode + setfont:
  case mmode + setfont:
  case vmode + deffont:
  case hmode + deffont:
  case mmode + deffont:
  case vmode + register_:
  case hmode + register_:
  case mmode + register_:
  case vmode + advance:
  case hmode + advance:
  case mmode + advance:
  case vmode + multiply:
  case hmode + multiply:
  case mmode + multiply:
  case vmode + divide:
  case hmode + divide:
  case mmode + divide:
  case vmode + prefix:
  case hmode + prefix:
  case mmode + prefix:
  case vmode + let:
  case hmode + let:
  case mmode + let:
  case vmode + shorthanddef:
  case hmode + shorthanddef:
  case mmode + shorthanddef:
  case vmode + readtocs:
  case hmode + readtocs:
  case mmode + readtocs:
  case vmode + def:
  case hmode + def:
  case mmode + def:
  case vmode + setbox:
  case hmode + setbox:
  case mmode + setbox:
  case vmode + hyphdata:
  case hmode + hyphdata:
  case mmode + hyphdata:
  case vmode + setinteraction:
  case hmode + setinteraction:
  case mmode + setinteraction:   /*:1210*/
    prefixedcommand();
    break;
    /*1268:*/

  case vmode + afterassignment:
  case hmode + afterassignment:
  case mmode + afterassignment:   /*:1268*/
    gettoken();
    aftertoken = curtok;
    break;
    /*1271:*/

  case vmode + aftergroup:
  case hmode + aftergroup:
  case mmode + aftergroup:   /*:1271*/
    gettoken();
    saveforafter(curtok);
    break;
    /*1274:*/

  case vmode + instream:
  case hmode + instream:
  case mmode + instream:   /*:1274*/
    openorclosein();
    break;

  /*1276:*/
  case vmode + message:
  case hmode + message:
  case mmode + message:
    issuemessage();
    break;

  /*:1276*/
  /*1285:*/
  case vmode + caseshift:
  case hmode + caseshift:
  case mmode + caseshift:
    shiftcase();
    break;

  /*:1285*/
  /*1290:*/
  case vmode + xray:
  case hmode + xray:
  case mmode + xray:
    showwhatever();
    break;

  /*:1290*/
  /*1347:*/
  case vmode + extension:
  case hmode + extension:
  case mmode + extension:   /*:1347*/
    doextension();
    break;
    /*:1045*/
  }
  goto _Lbigswitch_;
_Lmainloop:
  /*1034:*/
  adjustspacefactor();
  mainf = curfont;
  bchar = fontbchar[mainf ];
  falsebchar = fontfalsebchar[mainf ];
  if (mode > 0) {
    if (language != clang)
      fixlanguage();
  }
  fastgetavail(ligstack);
  font(ligstack) = mainf;
  curl = curchr;
  character(ligstack) = curl;
  curq = tail;
  if (cancelboundary) {
    cancelboundary = false;
    maink = nonaddress;
  } else
    maink = bcharlabel[mainf ];
  if (maink == nonaddress)
    goto _Lmainloopmove2;
  curr = curl;
  curl = nonchar;
  goto _Lmainligloop1;
_Lmainloopwrapup:   /*1035:*/
  wrapup(rthit);   /*:1035*/
_Lmainloopmove:   /*1036:*/
  if (ligstack == 0)
    goto _Lreswitch;
  curq = tail;
  curl = character(ligstack);
_Lmainloopmove1:
  if (!ischarnode(ligstack))
    goto _Lmainloopmovelig;
_Lmainloopmove2:
  if (curchr < fontbc[mainf ] || curchr > fontec[mainf ]) {
    charwarning(mainf, curchr);
    freeavail(ligstack);
    goto _Lbigswitch_;
  }
  maini = charinfo(mainf, curl);
  if (!charexists(maini)) {
    charwarning(mainf, curchr);
    freeavail(ligstack);
    goto _Lbigswitch_;
  }
  tailappend(ligstack);   /*:1036*/
_Lmainlooplookahead:   /*1038:*/
  getnext();
  if (curcmd == letter)
    goto _Lmainlooplookahead1;
  if (curcmd == otherchar)
    goto _Lmainlooplookahead1;
  if (curcmd == chargiven)
    goto _Lmainlooplookahead1;
  xtoken();
  if (curcmd == letter)
    goto _Lmainlooplookahead1;
  if (curcmd == otherchar)
    goto _Lmainlooplookahead1;
  if (curcmd == chargiven)
    goto _Lmainlooplookahead1;
  if (curcmd == charnum) {
    scancharnum();
    curchr = curval;
    goto _Lmainlooplookahead1;
  }
  if (curcmd == noboundary)
    bchar = nonchar;
  curr = bchar;
  ligstack = 0;
  goto _Lmainligloop;
_Lmainlooplookahead1:
  adjustspacefactor();
  fastgetavail(ligstack);
  font(ligstack) = mainf;
  curr = curchr;
  character(ligstack) = curr;
  if (curr == falsebchar)
    curr = nonchar;   /*:1038*/
_Lmainligloop:   /*1039:*/
  if (chartag(maini) != ligtag) {
    goto _Lmainloopwrapup;
  }
  maink = ligkernstart(mainf,maini);
  mainj = fontinfo[maink].qqqq;
  if (skipbyte(mainj) <= stopflag)
    goto _Lmainligloop2;
  maink = ligkernrestart(mainf,mainj);
_Lmainligloop1:
  mainj = fontinfo[maink].qqqq;
_Lmainligloop2:
  if (nextchar(mainj) == curr) {
    if (skipbyte(mainj) <= stopflag) {   /*1040:*/
      if (opbyte(mainj) >= kernflag) {
	wrapup(rthit);
	tailappend(newkern(charkern(mainf, mainj)));
	goto _Lmainloopmove;
      }
      if (curl == nonchar)
	lfthit = true;
      else if (ligstack == 0)
	rthit = true;
      checkinterrupt();
      switch (opbyte(mainj)) {

      case minquarterword + 1:
      case minquarterword + 5:
	curl = rembyte(mainj);
	maini = charinfo(mainf, curl);
	ligaturepresent = true;
	break;

      case minquarterword + 2:
      case minquarterword + 6:
	curr = rembyte(mainj);
	if (ligstack == 0) {
	  ligstack = newligitem(curr);
	  bchar = nonchar;
	} else if (ischarnode(ligstack)) {
	  mainp = ligstack;
	  ligstack = newligitem(curr);
	  ligptr(ligstack) = mainp;
	} else
	  character(ligstack) = curr;
	break;

      case minquarterword + 3:
	curr = rembyte(mainj);
	mainp = ligstack;
	ligstack = newligitem(curr);
	link(ligstack) = mainp;
	break;

      case minquarterword + 7:
      case minquarterword + 11:
	wrapup(false);
	curq = tail;
	curl = rembyte(mainj);
	maini = charinfo(mainf, curl);
	ligaturepresent = true;
	break;

      default:
	curl = rembyte(mainj);
	ligaturepresent = true;
	if (ligstack == 0)
	  goto _Lmainloopwrapup;
	else
	  goto _Lmainloopmove1;
	break;
      }
      if (opbyte(mainj) > minquarterword + 4) {
	if (opbyte(mainj) != minquarterword + 7)
	  goto _Lmainloopwrapup;
      }
      if (curl < nonchar)
	goto _Lmainligloop;
      maink = bcharlabel[mainf ];
      goto _Lmainligloop1;
    }
    /*:1040*/
  }
  if (skipbyte(mainj) == minquarterword)
    maink++;
  else {
    if (skipbyte(mainj) >= stopflag)
      goto _Lmainloopwrapup;
    maink += skipbyte(mainj) - minquarterword + 1;
  }
  goto _Lmainligloop1;   /*:1039*/
_Lmainloopmovelig:   /*1037:*/
  mainp = ligptr(ligstack);
  if (mainp > 0) {
    tailappend(mainp);
  }
  tempptr = ligstack;
  ligstack = link(tempptr);
  freenode(tempptr, smallnodesize);
  maini = charinfo(mainf, curl);
  ligaturepresent = true;
  if (ligstack == 0) {
    if (mainp > 0)
      goto _Lmainlooplookahead;
    curr = bchar;
  } else
    curr = character(ligstack);
  goto _Lmainligloop;   /*:1037*/
  /*:1034*/
_Lappendnormalspace_:   /*1041:*/
  if (spaceskip == zeroglue) {  /*1042:*/
    mainp = fontglue[curfont ];
    if (mainp == 0) {   /*:1042*/
      fontindex mmaink;
      mainp = newspec(zeroglue);
      mmaink = parambase[curfont ] + spacecode;
#if 1
	maink = mmaink;
#endif
      width(mainp) = fontinfo[mmaink].sc;
      stretch(mainp) = fontinfo[mmaink + 1].sc;
      shrink(mainp) = fontinfo[mmaink + 2].sc;
      fontglue[curfont ] = mainp;
    }
    tempptr = newglue(mainp);
  } else
    tempptr = newparamglue(spaceskipcode);
  link(tail) = tempptr;
  tail = tempptr;
  goto _Lbigswitch_;   /*:1041*/
_Lexit: ;
}
