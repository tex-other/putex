Static void makemathaccent(halfword q)
{
  pointer p, x, y;
  long a;
  quarterword c;
  internalfontnumber f;
  fourquarters i;
  scaled s, h, delta, w;

  fetch(accentchr(q));
  if (!charexists(curi))
    return;
  i = curi;
  c = curc;
  f = curf;   /*741:*/
  s = 0;
  if (mathtype(nucleus(q)) == mathchar) {
    fetch(nucleus(q));
    if (chartag(curi) == ligtag) {
      a = ligkernstart(curf,curi); 
      curi = fontinfo[a].qqqq;
      if (skipbyte(curi) > stopflag) {
	a = ligkernrestart(curf,curi); 
	curi = fontinfo[a].qqqq;
      }
      while (true) {
	if (nextchar(curi) - minquarterword == get_skewchar(curf)) {
	  if (opbyte(curi) >= kernflag) {
	    if (skipbyte(curi) <= stopflag)
	      s = charkern(curf, curi);
	  }
	  goto _Ldone1;
	}
	if (skipbyte(curi) >= stopflag)
	  goto _Ldone1;
	a += skipbyte(curi) - minquarterword + 1;
	curi = fontinfo[a].qqqq;
      }
    }
  }
_Ldone1:   /*:741*/
  x = cleanbox(nucleus(q), crampedstyle(curstyle));
  w = width(x);
  h = height(x);   /*740:*/
  while (true) {
    if (chartag(i) != listtag) {
      goto _Ldone;
    }
    y = rembyte(i);
    i = charinfo(f, y);
    if (!charexists(i))
      goto _Ldone;
    if (charwidth(f, i) > w)
      goto _Ldone;
    c = y;
  }
_Ldone:   /*:740*/
  if (h < xheight(f))
    delta = h;
  else
    delta = xheight(f);
  if ((mathtype(supscr(q)) != empty) |
      (mathtype(subscr(q)) != empty)) {
    if (mathtype(nucleus(q)) == mathchar) {   /*742:*/
      flushnodelist(x);
      x = newnoad();
      mem[nucleus(x) - memmin] = mem[nucleus(q) - memmin];
      mem[supscr(x) - memmin] = mem[supscr(q) - memmin];
      mem[subscr(x) - memmin] = mem[subscr(q) - memmin];
      mem[supscr(q) - memmin].hh = emptyfield;
      mem[subscr(q) - memmin].hh = emptyfield;
      mathtype(nucleus(q)) = submlist;
      info(nucleus(q)) = x;
      x = cleanbox(nucleus(q), curstyle);
      delta += height(x) - h;
      h = height(x);
    }
    /*:742*/
  }
  y = charbox(f, c);
  shiftamount(y) = s + half(w - width(y));
  width(y) = 0;
  p = newkern(-delta);
  link(p) = x;
  link(y) = p;
  y = vpack(y, 0, additional);
/* p2c: tex1.p: Warning: Too many arguments for foovpack [299] */
  width(y) = width(x);
  if (height(y) < h) {   /*739:*/
    p = newkern(h - height(y));
    link(p) = listptr(y);
    listptr(y) = p;
    height(y) = h;
  }  /*:739*/
  info(nucleus(q)) = y;
  mathtype(nucleus(q)) = subbox;
}
