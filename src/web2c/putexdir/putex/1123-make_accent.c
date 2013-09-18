Static void makeaccent(void)
{
  double s, t;
  pointer p, q, r;
  internalfontnumber f;
  scaled a, h, x, w, delta;
  fourquarters i;

  scancharnum();
  f = curfont;
  p = newcharacter(f, curval);
  if (p == 0)
    return;
  x = xheight(f);
  s = slant(f) / 65536.0;
  a = charwidth(f, charinfo(f, character(p)));
  doassignments();   /*1124:*/
  q = 0;
  f = curfont;
  if (curcmd == letter || curcmd == otherchar || curcmd == chargiven)
    q = newcharacter(f, curchr);
  else if (curcmd == charnum) {
    scancharnum();
    q = newcharacter(f, curval);
  } else
    backinput();
  if (q != 0) {   /*1125:*/
    t = slant(f) / 65536.0;
    i = charinfo(f, character(q));
    w = charwidth(f, i);
    h = charheight(f, heightdepth(i));
    if (h != x) {
      p = hpack(p, 0, additional);
      shiftamount(p) = x - h;
    }
    delta = (long)floor((w - a) / 2.0 + h * t - x * s + 0.5);
    r = newkern(delta);
    subtype(r) = acckern;
    link(tail) = r;
    link(r) = p;
    tail = newkern(-a - delta);
    subtype(tail) = acckern;
    link(p) = tail;
    p = q;
  }
  /*:1125*/
  link(tail) = p;
  tail = p;
  spacefactor = 1000;

  /*:1124*/
}
