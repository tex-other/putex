Static void mlisttohlist(void)
{
  pointer mlist, q, r, p=0 /* XXXX */, x=0 /* XXXX */, y, z;
  boolean penalties;
  smallnumber style, savestyle, rtype, t;
  long pen;
  smallnumber s;
  scaled maxh, maxd, delta;

  mlist = curmlist;
  penalties = mlistpenalties;
  style = curstyle;
  q = mlist;
  r = 0;
  rtype = opnoad;
  maxh = 0;
  maxd = 0;   /*703:*/
  if (curstyle < scriptstyle)
    cursize = textsize;
  else
    cursize = (curstyle - textstyle) / 2 * 16;
  curmu = xovern(mathquad(cursize), 18);   /*:703*/
  while (q != 0) {   /*727:*/
_Lreswitch:
    delta = 0;
    switch (type(q)) {

    case binnoad:
      switch (rtype) {

      case binnoad:
      case opnoad:
      case relnoad:
      case opennoad:
      case punctnoad:
      case leftnoad:
	type(q) = ordnoad;
	goto _Lreswitch;
	break;
      }
      break;

    case relnoad:
    case closenoad:
    case punctnoad:
    case rightnoad:  /*729:*/
      if (rtype == binnoad)
	type(r) = ordnoad;   /*:729*/
      if (type(q) == rightnoad)
	goto _Ldonewithnoad_;
      break;
      /*733:*/

    case leftnoad:
      goto _Ldonewithnoad_;
      break;

    case fractionnoad:
      makefraction(q);
      goto _Lcheckdimensions_;
      break;

    case opnoad:
      delta = makeop(q);
      if (subtype(q) == limits)
	goto _Lcheckdimensions_;
      break;

    case ordnoad:
      makeord(q);
      break;

    case opennoad:
    case innernoad:
      /* blank case */
      break;

    case radicalnoad:
      makeradical(q);
      break;

    case overnoad:
      makeover(q);
      break;

    case undernoad:
      makeunder(q);
      break;

    case accentnoad:
      makemathaccent(q);
      break;

    case vcenternoad:   /*:733*/
      makevcenter(q);
      break;
      /*730:*/

    case stylenode:
      curstyle = subtype(q);   /*703:*/
      if (curstyle < scriptstyle)
	cursize = textsize;
      else
	cursize = (curstyle - textstyle) / 2 * 16;
      curmu = xovern(mathquad(cursize), 18);   /*:703*/
      goto _Ldonewithnode_;
      break;

    case choicenode:   /*731:*/
      switch (curstyle / 2) {

      case 0:
	p = displaymlist(q);
	displaymlist(q) = 0;
	break;

      case 1:
	p = textmlist(q);
	textmlist(q) = 0;
	break;

      case 2:
	p = scriptmlist(q);
	scriptmlist(q) = 0;
	break;

      case 3:
	p = scriptscriptmlist(q);
	scriptscriptmlist(q) = 0;
	break;
      }
      flushnodelist(displaymlist(q));
      flushnodelist(textmlist(q));
      flushnodelist(scriptmlist(q));
      flushnodelist(scriptscriptmlist(q));
      type(q) = stylenode;
      subtype(q) = curstyle;
      width(q) = 0;
      depth(q) = 0;
      if (p != 0) {
	z = link(q);
	link(q) = p;
	while (link(p) != 0)
	  p = link(p);
	link(p) = z;
      }
      goto _Ldonewithnode_;
      break;
      /*:731*/

    case insnode:
    case marknode:
    case adjustnode:
    case whatsitnode:
    case penaltynode:
    case discnode:
      goto _Ldonewithnode_;
      break;

    case rulenode:
      if (height(q) > maxh)
	maxh = height(q);
      if (depth(q) > maxd)
	maxd = depth(q);
      goto _Ldonewithnode_;
      break;

    case gluenode:  /*732:*/
      if (subtype(q) == muglue) {
	x = glueptr(q);
	y = mathglue(x, curmu);
	deleteglueref(x);
	glueptr(q) = y;
	subtype(q) = normal;
      } else if ((cursize != textsize) & (subtype(q) == condmathglue)) {
	p = link(q);
	if (p != 0) {
	  if ((type(p) == gluenode) | (type(p) == kernnode)) {
	    link(q) = link(p);
	    link(p) = 0;
	    flushnodelist(p);
	  }
	}
      }
      goto _Ldonewithnode_;
      break;

    case kernnode:   /*:730*/
      mathkern(q, curmu);
      goto _Ldonewithnode_;
      break;

    default:
      confusion(S(720));
      break;
    }
    /*754:*/
    switch (mathtype(nucleus(q))) {

    case mathchar:
    case mathtextchar:   /*:755*/
      /*755:*/
      fetch(nucleus(q));
      if (charexists(curi)) {
	delta = charitalic(curf, curi);
	p = newcharacter(curf, curc - minquarterword);
	if ((mathtype(nucleus(q)) == mathtextchar) & (space(curf) != 0))
	  delta = 0;
	if (mathtype(subscr(q)) == empty && delta != 0) {
	  link(p) = newkern(delta);
	  delta = 0;
	}
      } else
	p = 0;
      break;

    case empty:
      p = 0;
      break;

    case subbox:
      p = info(nucleus(q));
      break;

    case submlist:
      curmlist = info(nucleus(q));
      savestyle = curstyle;
      mlistpenalties = false;
      mlisttohlist();
      curstyle = savestyle;   /*703:*/
      if (curstyle < scriptstyle)
	cursize = textsize;
      else
	cursize = (curstyle - textstyle) / 2 * 16;
      curmu = xovern(mathquad(cursize), 18);   /*:703*/
      p = hpack(link(temphead), 0, additional);
      break;

    default:
      confusion(S(721));
      break;
    }
    newhlist(q) = p;
    if ((mathtype(subscr(q)) == empty) &
	(mathtype(supscr(q)) == empty))
	  /*:754*/
	    goto _Lcheckdimensions_;
    /*:728*/
    makescripts(q, delta);
_Lcheckdimensions_:
    z = hpack(newhlist(q), 0, additional);
    if (height(z) > maxh)
      maxh = height(z);
    if (depth(z) > maxd)
      maxd = depth(z);
    freenode(z, boxnodesize);
_Ldonewithnoad_:
    r = q;
    rtype = type(r);
_Ldonewithnode_:
    q = link(q);
  }
  /*728:*/
  /*:727*/
  /*729:*/
  if (rtype == binnoad)   /*760:*/
    type(r) = ordnoad;   /*:729*/
  p = temphead;
  link(p) = 0;
  q = mlist;
  rtype = 0;
  curstyle = style;   /*703:*/
  if (curstyle < scriptstyle)
    cursize = textsize;
  else
    cursize = (curstyle - textstyle) / 2 * 16;
  curmu = xovern(mathquad(cursize), 18);   /*:703*/
  while (q != 0) {   /*761:*/
    t = ordnoad;
    s = noadsize;
    pen = infpenalty;
    switch (type(q)) {   /*:761*/

    case opnoad:
    case opennoad:
    case closenoad:
    case punctnoad:
    case innernoad:
      t = type(q);
      break;

    case binnoad:
      t = binnoad;
      pen = binoppenalty;
      break;

    case relnoad:
      t = relnoad;
      pen = relpenalty;
      break;

    case ordnoad:
    case vcenternoad:
    case overnoad:
    case undernoad:
      /* blank case */
      break;

    case radicalnoad:
      s = radicalnoadsize;
      break;

    case accentnoad:
      s = accentnoadsize;
      break;

    case fractionnoad:
      t = innernoad;
      s = fractionnoadsize;
      break;

    case leftnoad:
    case rightnoad:
      t = makeleftright(q, style, maxd, maxh);
      break;

    case stylenode:   /*763:*/
      curstyle = subtype(q);
      s = stylenodesize;   /*703:*/
      if (curstyle < scriptstyle)
	cursize = textsize;
      else
	cursize = (curstyle - textstyle) / 2 * 16;
      curmu = xovern(mathquad(cursize), 18);   /*:703*/
      goto _Ldeleteq_;
      break;
      /*:763*/

    case whatsitnode:
    case penaltynode:
    case rulenode:
    case discnode:
    case adjustnode:
    case insnode:
    case marknode:
    case gluenode:
    case kernnode:
      link(p) = q;
      p = q;
      q = link(q);
      link(p) = 0;
      goto _Ldone;
      break;

    default:
      confusion(S(722));
      break;
    }
    /*766:*/
    if (rtype > 0) {   /*:766*/
      const char trans_table[]="0234000122*4000133**3**344*0400400*000000234000111*1111112341011";
      switch (trans_table[rtype * 8 + t - ordnoad * 9]) {
      case '0':
	x = 0;
	break;

      case '1':
	if (curstyle < scriptstyle)
	  x = thinmuskipcode;
	else
	  x = 0;
	break;

      case '2':
	x = thinmuskipcode;
	break;

      case '3':
	if (curstyle < scriptstyle)
	  x = medmuskipcode;
	else
	  x = 0;
	break;

      case '4':
	if (curstyle < scriptstyle)
	  x = thickmuskipcode;
	else
	  x = 0;
	break;

      default:
	confusion(S(723));
	break;
      }
      if (x != 0) {
	y = mathglue(gluepar(x), curmu);
	z = newglue(y);
	gluerefcount(y) = 0;
	link(p) = z;
	p = z;
	subtype(z) = x + 1;
      }
    }
    /*767:*/
    if (newhlist(q) != 0) {
      link(p) = newhlist(q);
      do {
	p = link(p);
      } while (link(p) != 0);
    }
    if (penalties) {
      if (link(q) != 0) {
	if (pen < infpenalty) {   /*:767*/
	  rtype = type(link(q));
	  if (rtype != penaltynode) {
	    if (rtype != relnoad) {
	      z = newpenalty(pen);
	      link(p) = z;
	      p = z;
	    }
	  }
	}
      }
    }
    rtype = t;
_Ldeleteq_:
    r = q;
    q = link(q);
    freenode(r, s);
_Ldone: ;
  }
  /*:760*/

  /*:732*/
}

