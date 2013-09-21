Static halfword hpack(halfword p, long w, smallnumber m)
{
  pointer r, q, g;
  scaled h, d, x, s;
  glueord o;
  eightbits hd;

  lastbadness = 0;
  r = getnode(boxnodesize);
  type(r) = hlistnode;
  subtype(r) = minquarterword;
  shiftamount(r) = 0;
  q = r + listoffset;
  link(q) = p;
  h = 0;   /*650:*/
  d = 0;
  x = 0;
  totalstretch[0] = 0;
  totalshrink[0] = 0;
  totalstretch[fil - normal] = 0;
  totalshrink[fil - normal] = 0;
  totalstretch[fill - normal] = 0;
  totalshrink[fill - normal] = 0;
  totalstretch[filll - normal] = 0;
  totalshrink[filll - normal] = 0;   /*:650*/
  while (p != 0) {   /*651:*/
_Lreswitch:
    while (ischarnode(p)) {   /*654:*/
      internalfontnumber f = font(p);
      fourquarters i = charinfo(f, character(p));
      hd = heightdepth(i);
      x += charwidth(f, i);
      s = charheight(f, hd);
      if (s > h)
	h = s;
      s = chardepth(f, hd);
      if (s > d)
	d = s;
      p = link(p);
    }
    /*:654*/
    if (p == 0)
      break;
    switch (type(p)) {

    case hlistnode:
    case vlistnode:
    case rulenode:
    case unsetnode:   /*653:*/
      x += width(p);
      if (type(p) >= rulenode)
	s = 0;
      else
	s = shiftamount(p);
      if (height(p) - s > h)
	h = height(p) - s;
      if (depth(p) + s > d)
	d = depth(p) + s;
      break;
      /*:653*/

    case insnode:
    case marknode:
    case adjustnode:
      if (adjusttail != 0) {   /*655:*/
	while (link(q) != p)
	  q = link(q);
	if (type(p) == adjustnode) {
	  link(adjusttail) = adjustptr(p);
	  while (link(adjusttail) != 0)
	    adjusttail = link(adjusttail);
	  p = link(p);
	  freenode(link(q), smallnodesize);
	} else {
	  link(adjusttail) = p;
	  adjusttail = p;
	  p = link(p);
	}
	link(q) = p;
	p = q;
      }
      /*:655*/
      break;

    case whatsitnode:   /*1360:*/
      break;
      /*:1360*/

    case gluenode:   /*656:*/
      g = glueptr(p);
      x += width(g);
      o = stretchorder(g);
      totalstretch[o - normal] += stretch(g);
      o = shrinkorder(g);
      totalshrink[o - normal] += shrink(g);
      if (subtype(p) >= aleaders) {
	g = leaderptr(p);
	if (height(g) > h)
	  h = height(g);
	if (depth(g) > d)
	  d = depth(g);
      }
      break;
      /*:656*/

    case kernnode:
    case mathnode:
      x += width(p);
      break;

    case ligaturenode:   /*652:*/
	type(ligtrick) = charnodetype;
      font(ligtrick) = font_ligchar(p);
      character(ligtrick) = character_ligchar(p);
      link(ligtrick) = link(p);
      p = ligtrick;
      goto _Lreswitch;
      break;
      /*:652*/
    }
    p = link(p);
  }
  /*:651*/
  if (adjusttail != 0)
    link(adjusttail) = 0;
  height(r) = h;
  depth(r) = d;   /*657:*/
  if (m == additional)
    w += x;
  width(r) = w;
  x = w - x;
  if (x == 0) {
    gluesign(r) = normal;
    glueorder(r) = normal;
    glueset(r) = 0.0;
    goto _Lexit;
  } else if (x > 0) {
    if (totalstretch[filll - normal] != 0)
      o = filll;
    else if (totalstretch[fill - normal] != 0)
      o = fill;
    else if (totalstretch[fil - normal] != 0)
      o = fil;
    else {
      o = normal;
      /*:
659*/
    }
    glueorder(r) = o;
    gluesign(r) = stretching;
    if (totalstretch[o - normal] != 0)
      glueset(r) = (double)x / totalstretch[o - normal];
    else {
      gluesign(r) = normal;
      glueset(r) = 0.0;
    }
    if (o == normal) {
      if (listptr(r) != 0) {   /*660:*/
	lastbadness = badness(x, totalstretch[0]);
	if (lastbadness > hbadness) {
	  println();
	  if (lastbadness > 100)
	    printnl(S(699));
	  else
	    printnl(S(700));
	  print(S(701));
	  printint(lastbadness);
	  goto _Lcommonending;
	}
      }
      /*:660*/
    }
    goto _Lexit;
  } else {
    if (totalshrink[filll - normal] != 0)
      o = filll;
    else if (totalshrink[fill - normal] != 0)
      o = fill;
    else if (totalshrink[fil - normal] != 0)
      o = fil;
    else
      o = normal;   /*:665*/
    glueorder(r) = o;
    gluesign(r) = shrinking;
    if (totalshrink[o - normal] != 0)
      glueset(r) = (double)(-x) / totalshrink[o - normal];
    else {
      gluesign(r) = normal;
      glueset(r) = 0.0;
    }
    if ((totalshrink[o - normal] < -x && o == normal) & (listptr(r) != 0)) {
      lastbadness = 1000000L;
      glueset(r) = 1.0;   /*666:*/
      if (-x - totalshrink[0] > hfuzz || hbadness < 100) {   /*:666*/
	if (overfullrule > 0 && -x - totalshrink[0] > hfuzz) {
	  while (link(q) != 0)
	    q = link(q);
	  link(q) = newrule();
	  width(link(q)) = overfullrule;
	}
	println();
	printnl(S(702));
	printscaled(-x - totalshrink[0]);
	print(S(703));
	goto _Lcommonending;
      }
    } else if (o == normal) {
      if (listptr(r) != 0) {   /*667:*/
	lastbadness = badness(-x, totalshrink[0]);
	if (lastbadness > hbadness) {
	  println();
	  printnl(S(704));
	  printint(lastbadness);
	  goto _Lcommonending;
	}
      }
      /*:667*/
    }
    goto _Lexit;
  }
_Lcommonending:   /*663:*/
  if (outputactive)
    print(S(705));
  else {
    if (packbeginline != 0) {
      if (packbeginline > 0)
	print(S(706));
      else
	print(S(707));
      printint(labs(packbeginline));
      print(S(708));
    } else
      print(S(709));
    printint(line);
  }
  println();
  fontinshortdisplay = nullfont;
  shortdisplay(listptr(r));
  println();
  begindiagnostic();
  showbox(r);   /*:663*/
  enddiagnostic(true);
_Lexit:
  return r;

  /*658:*/
  /*659:*/
  /*:658*/
  /*664:*/
  /*665:*/
  /*:664*/
  /*:657*/
}
