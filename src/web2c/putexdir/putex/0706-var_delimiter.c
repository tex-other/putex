Static halfword vardelimiter(halfword d, smallnumber s, long v)
{
  pointer b;
  internalfontnumber f, g;
  quarterword c=0 /* XXXX */, x, y;
  long m, n;
  scaled u, w;
  fourquarters q;
  eightbits hd;
  fourquarters r;
  smallnumber z;
  boolean largeattempt;

  f = nullfont;
  w = 0;
  largeattempt = false;
  z = smallfam(d);
  x = smallchar(d);
  while (true) {  /*707:*/
    if (z != 0 || x != minquarterword) {   /*:707*/
      z += s + 16;
      do {
	z -= 16;
	g = famfnt(z);
	if (g != nullfont) {   /*708:*/
	  y = x;
	  if (y - minquarterword >= fontbc[g ] &&
	      y - minquarterword <= fontec[g ]) {
_Llabcontinue:
	    q = charinfo(g, y);
	    if (charexists(q)) {
	      if (chartag(q) == exttag) {
		f = g;
		c = y;
		goto _Lfound;
	      }
	      hd = heightdepth(q);
	      u = charheight(g, hd) + chardepth(g, hd);
	      if (u > w) {
		f = g;
		c = y;
		w = u;
		if (u >= v)
		  goto _Lfound;
	      }
	      if (chartag(q) == listtag) {
		y = rembyte(q);
		goto _Llabcontinue;
	      }
	    }
	  }
	}
	/*:708*/
      } while (z >= 16);
    }
    if (largeattempt)
      goto _Lfound;
    largeattempt = true;
    z = largefam(d);
    x = largechar(d);
  }
_Lfound:
  if (f != nullfont) {   /*710:*/
    if (chartag(q) == exttag) {   /*713:*/
      b = newnullbox();
      type(b) = vlistnode;
      r = exteninfo(f,q); /* fontinfo[extenbase[f ] + rembyte(q)].qqqq; */
  /*714:*/
      c = extrep(r);
      u = heightplusdepth(f, c);
      w = 0;
      q = charinfo(f, c);
      width(b) = charwidth(f, q) + charitalic(f, q);
      c = extbot(r);
      if (c != minquarterword)
	w += heightplusdepth(f, c);
      c = extmid(r);
      if (c != minquarterword)
	w += heightplusdepth(f, c);
      c = exttop(r);
      if (c != minquarterword)
	w += heightplusdepth(f, c);
      n = 0;
      if (u > 0) {
	while (w < v) {   /*:714*/
	  w += u;
	  n++;
	  if (extmid(r) != minquarterword)
	    w += u;
	}
      }
      c = extbot(r);
      if (c != minquarterword)
	stackintobox(b, f, c);
      c = extrep(r);
      for (m = 1; m <= n; m++)
	stackintobox(b, f, c);
      c = extmid(r);
      if (c != minquarterword) {
	stackintobox(b, f, c);
	c = extrep(r);
	for (m = 1; m <= n; m++)
	  stackintobox(b, f, c);
      }
      c = exttop(r);
      if (c != minquarterword)
	stackintobox(b, f, c);
      depth(b) = w - height(b);
    } else
      b = charbox(f, c);   /*:710*/
    /*:713*/
  } else {
    b = newnullbox();
    width(b) = nulldelimiterspace;
  }
  shiftamount(b) = half(height(b) - depth(b)) - axisheight(s);
  return b;
}
