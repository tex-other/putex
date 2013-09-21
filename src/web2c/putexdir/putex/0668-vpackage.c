Static halfword vpackage(halfword p, long h, smallnumber m, long l)
{
  pointer r, g;
  scaled w, d, x, s;
  glueord o;

  lastbadness = 0;
  r = getnode(boxnodesize);
  type(r) = vlistnode;
  subtype(r) = minquarterword;
  shiftamount(r) = 0;
  listptr(r) = p;
  w = 0;   /*650:*/
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
  while (p != 0) {   /*669:*/
    if (ischarnode(p))
      confusion(S(710));
    else {
      switch (type(p)) {

      case hlistnode:
      case vlistnode:
      case rulenode:
      case unsetnode:   /*670:*/
	x += d + height(p);
	d = depth(p);
	if (type(p) >= rulenode)
	  s = 0;
	else
	  s = shiftamount(p);
	if (width(p) + s > w)
	  w = width(p) + s;
	break;
	/*:670*/

      case whatsitnode:   /*1359:*/
	break;

      /*:1359*/
      case gluenode:   /*:671*/
	/*671:*/
	x += d;
	d = 0;
	g = glueptr(p);
	x += width(g);
	o = stretchorder(g);
	totalstretch[o - normal] += stretch(g);
	o = shrinkorder(g);
	totalshrink[o - normal] += shrink(g);
	if (subtype(p) >= aleaders) {
	  g = leaderptr(p);
	  if (width(g) > w)
	    w = width(g);
	}
	break;

      case kernnode:
	x += d + width(p);
	d = 0;
	break;
      }
    }
    p = link(p);
  }
  /*:669*/
  width(r) = w;
  if (d > l) {   /*672:*/
    x += d - l;
    depth(r) = l;
  } else
    depth(r) = d;
  if (m == additional)
    h += x;
  height(r) = h;
  x = h - x;
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
      if (listptr(r) != 0) {   /*674:*/
	lastbadness = badness(x, totalstretch[0]);
	if (lastbadness > vbadness) {
	  println();
	  if (lastbadness > 100)
	    printnl(S(699));
	  else
	    printnl(S(700));
	  print(S(711));
	  printint(lastbadness);
	  goto _Lcommonending;
	}
      }
      /*:674*/
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
      glueset(r) = 1.0;   /*677:*/
      if (-x - totalshrink[0] > vfuzz || vbadness < 100) {   /*:677*/
	println();
	printnl(S(712));
	printscaled(-x - totalshrink[0]);
	print(S(713));
	goto _Lcommonending;
      }
    } else if (o == normal) {
      if (listptr(r) != 0) {   /*678:*/
	lastbadness = badness(-x, totalshrink[0]);
	if (lastbadness > vbadness) {
	  println();
	  printnl(S(714));
	  printint(lastbadness);
	  goto _Lcommonending;
	}
      }
      /*:678*/
    }
    goto _Lexit;
  }
_Lcommonending:   /*675:*/
  if (outputactive)
    print(S(705));
  else {
    if (packbeginline != 0) {
      print(S(707));
      printint(labs(packbeginline));
      print(S(708));
    } else
      print(S(709));
    printint(line);
    println();
  }
  begindiagnostic();
  showbox(r);   /*:675*/
  enddiagnostic(true);
_Lexit:
  return r;

  /*673:*/
  /*659:*/
  /*:673*/
  /*676:*/
  /*665:*/
  /*:676*/
  /*:672*/
}

