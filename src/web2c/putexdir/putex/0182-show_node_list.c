Static void shownodelist(long p)
{
  long n;
  double g;

  if (get_cur_length() > depththreshold) {
    if (p > 0)
      print(S(334));
    goto _Lexit;
  }
  n = 0;
  while (p > memmin) {
    println();
    printcurrentstring();
    if (p > memend) {
      print(S(360));
      goto _Lexit;
    }
    n++;
    if (n > breadthmax) {
      print(S(361));
      goto _Lexit;
    }  /*183:*/
    if (ischarnode(p))
      printfontandchar(p);
    else {
      switch (type(p)) {   /*:183*/

      case hlistnode:
      case vlistnode:
      case unsetnode:   /*184:*/
	if (type(p) == hlistnode)
	  printesc('h');
	else if (type(p) == vlistnode)
	  printesc('v');
	else
	  printesc(S(362));
	print(S(363));
	printscaled(height(p));
	printchar('+');
	printscaled(depth(p));
	print(S(364));
	printscaled(width(p));
	if (type(p) == unsetnode) {   /*185:*/
	  if (spancount(p) != minquarterword) {
	    print(S(303));
	    printint(spancount(p) - minquarterword + 1);
	    print(S(365));
	  }
	  if (gluestretch(p) != 0) {
	    print(S(366));
	    printglue(gluestretch(p), glueorder(p), 0);
	  }
	  if (glueshrink(p) != 0) {
	    print(S(367));
	    printglue(glueshrink(p), gluesign(p), 0);
	  }
	}  /*:185*/
	else {   /*186:*/
	  g = glueset(p);
	  if ((g != 0.0) & (gluesign(p) != normal)) {   /*:186*/
	    print(S(368));
	    if (gluesign(p) == shrinking)
	      print(S(369));
	    if (0 /*labs(mem[p + glueoffset - memmin].int_) < 1048576L */)
	      print(S(370));
	    else {
	      if (fabs(g) > 20000.0) {
		if (g > 0.0)
		  printchar('>');
		else
		  print(S(371));
		printglue(unity * 20000, glueorder(p), 0);
	      } else
		printglue((long)floor(unity * g + 0.5), glueorder(p), 0);
	    }
	  }
	  if (shiftamount(p) != 0) {
	    print(S(372));
	    printscaled(shiftamount(p));
	  }
	}
	nodelistdisplay(listptr(p));
	break;
	/*:184*/

      case rulenode:   /*187:*/
	printesc(S(373));
	printruledimen(height(p));
	printchar('+');
	printruledimen(depth(p));
	print(S(364));
	printruledimen(width(p));
	break;
	/*:187*/

      case insnode:   /*188:*/
	printesc(S(374));
	printint(subtype(p) - minquarterword);
	print(S(375));
	printscaled(height(p));
	print(S(376));
	printspec(splittopptr(p), 0);
	printchar(',');
	printscaled(depth(p));
	print(S(377));
	printint(floatcost(p));
	nodelistdisplay(insptr(p));
	break;
	/*:188*/

      case whatsitnode:   /*1356:*/
	switch (subtype(p)) {   /*:1356*/

	case opennode:
	  printwritewhatsit(S(378), p);
	  printchar('=');
	  printfilename(openname(p), openarea(p), openext(p));
	  break;

	case writenode:
	  printwritewhatsit(S(379), p);
	  printmark(writetokens(p));
	  break;

	case closenode:
	  printwritewhatsit(S(380), p);
	  break;

	case specialnode:
	  printesc(S(381));
	  printmark(writetokens(p));
	  break;

	case languagenode:
	  printesc(S(382));
	  printint(whatlang(p));
	  print(S(383));
	  printint(whatlhm(p));
	  printchar(',');
	  printint(whatrhm(p));
	  printchar(')');
	  break;

	default:
	  print(S(384));
	  break;
	}
	break;

      case gluenode:   /*189:*/
	if (subtype(p) >= aleaders) {   /*190:*/
	  printesc(S(385));
	  if (subtype(p) == cleaders)
	    printchar('c');
	  else if (subtype(p) == xleaders)
	    printchar('x');
	  print(S(386));
	  printspec(glueptr(p), 0);
	  nodelistdisplay(leaderptr(p));
	}  /*:190*/
	else {   /*:189*/
	  printesc(S(387));
	  if (subtype(p) != normal) {
	    printchar('(');
	    if (subtype(p) < condmathglue)
	      printskipparam(subtype(p) - 1);
	    else {
	      if (subtype(p) == condmathglue)
		printesc(S(388));
	      else
		printesc(S(389));
	    }
	    printchar(')');
	  }
	  if (subtype(p) != condmathglue) {
	    printchar(' ');
	    if (subtype(p) < condmathglue)
	      printspec(glueptr(p), 0);
	    else
	      printspec(glueptr(p), S(390));
	  }
	}
	break;

      case kernnode:   /*191:*/
	if (subtype(p) != muglue) {
	  printesc(S(391));
	  if (subtype(p) != normal)
	    printchar(' ');
	  printscaled(width(p));
	  if (subtype(p) == acckern)
	    print(S(392));
	} else {   /*:191*/
	  printesc(S(393));
	  printscaled(width(p));
	  print(S(390));
	}
	break;

      case mathnode:   /*192:*/
	printesc(S(394));
	if (subtype(p) == before)
	  print(S(395));
	else
	  print(S(396));
	if (width(p) != 0) {
	  print(S(397));
	  printscaled(width(p));
	}
	break;
	/*:192*/

      case ligaturenode:   /*193:*/
#ifdef BIG_CHARNODE
	{
	pointer pp=getavail();
	type(pp) = charnodetype;
	font(pp) = font_ligchar(p);
	character(pp) = character_ligchar(p);
	printfontandchar(pp);
	freeavail(pp);
	}
#else
	printfontandchar(ligchar(p));
#endif
	print(S(398));
	if (subtype(p) > 1)
	  printchar('|');
	fontinshortdisplay = font_ligchar(p);
	shortdisplay(ligptr(p));
	if ((subtype(p)) & 1)
	  printchar('|');
	printchar(')');
	break;
	/*:193*/

      case penaltynode:   /*194:*/
	printesc(S(399));
	printint(penalty(p));
	break;
	/*:194*/

      case discnode:   /*195:*/
	printesc(S(400));
	if (replacecount(p) > 0) {
	  print(S(401));
	  printint(replacecount(p));
	}
	nodelistdisplay(prebreak(p));
	appendchar('|');
	shownodelist(postbreak(p));
	flushchar();
	break;
	/*:195*/

      case marknode:   /*196:*/
	printesc(S(402));
	printmark(markptr(p));
	break;
	/*:196*/

      case adjustnode:   /*197:*/
	printesc(S(403));
	nodelistdisplay(adjustptr(p));
	break;
	/*:197*/
	/*690:*/

      case stylenode:
	printstyle(subtype(p));
	break;

      case choicenode:   /*695:*/
	printesc(S(404));
	appendchar('D');
	shownodelist(displaymlist(p));
	flushchar();
	appendchar('T');
	shownodelist(textmlist(p));
	flushchar();
	appendchar('S');
	shownodelist(scriptmlist(p));
	flushchar();
	appendchar('s');
	shownodelist(scriptscriptmlist(p));
	flushchar();
	break;
	/*:695*/

      case ordnoad:
      case opnoad:
      case binnoad:
      case relnoad:
      case opennoad:
      case closenoad:
      case punctnoad:
      case innernoad:
      case radicalnoad:
      case overnoad:
      case undernoad:
      case vcenternoad:
      case accentnoad:
      case leftnoad:
      case rightnoad:   /*696:*/
	switch (type(p)) {

	case ordnoad:
	  printesc(S(405));
	  break;

	case opnoad:
	  printesc(S(406));
	  break;

	case binnoad:
	  printesc(S(407));
	  break;

	case relnoad:
	  printesc(S(408));
	  break;

	case opennoad:
	  printesc(S(409));
	  break;

	case closenoad:
	  printesc(S(410));
	  break;

	case punctnoad:
	  printesc(S(411));
	  break;

	case innernoad:
	  printesc(S(412));
	  break;

	case overnoad:
	  printesc(S(413));
	  break;

	case undernoad:
	  printesc(S(414));
	  break;

	case vcenternoad:
	  printesc(S(415));
	  break;

	case radicalnoad:
	  printesc(S(416));
	  printdelimiter(leftdelimiter(p));
	  break;

	case accentnoad:
	  printesc(S(417));
	  printfamandchar(accentchr(p));
	  break;

	case leftnoad:
	  printesc(S(418));
	  printdelimiter(nucleus(p));
	  break;

	case rightnoad:
	  printesc(S(419));
	  printdelimiter(nucleus(p));
	  break;
	}
	if (subtype(p) != normal) {
	  if (subtype(p) == limits)
	    printesc(S(420));
	  else
	    printesc(S(421));
	}
	if (type(p) < leftnoad)
	  printsubsidiarydata(nucleus(p), '.');
	printsubsidiarydata(supscr(p), '^');
	printsubsidiarydata(subscr(p), '_');
	break;
	/*:696*/

      case fractionnoad:   /*697:*/
	printesc(S(422));
	if (thickness(p) == defaultcode)
	  print(S(423));
	else
	  printscaled(thickness(p));
	if ((smallfam(leftdelimiter(p)) != 0) |
	    (smallchar(leftdelimiter(p)) != minquarterword) |
	    (largefam(leftdelimiter(p)) != 0) |
	    (largechar(leftdelimiter(p)) != minquarterword)) {
	  print(S(424));
	  printdelimiter(leftdelimiter(p));
	}
	if ((smallfam(rightdelimiter(p)) != 0) |
	    (smallchar(rightdelimiter(p)) != minquarterword) |
	    (largefam(rightdelimiter(p)) != 0) |
	    (largechar(rightdelimiter(p)) != minquarterword)) {
	  print(S(425));
	  printdelimiter(rightdelimiter(p));
	}
	printsubsidiarydata(numerator(p), '\\');
	printsubsidiarydata(denominator(p), '/');
	break;
	/*:697*/
	/*:690*/

      default:
	print(S(426));
	break;
      }
    }
    p = link(p);
  }
_Lexit: ;
}
