Static void makeradical(halfword q)
{
  pointer x, y;
  scaled delta, clr;

  x = cleanbox(nucleus(q), crampedstyle(curstyle));
  if (curstyle < textstyle)
    clr = defaultrulethickness + labs(mathxheight(cursize)) / 4;
  else {
    clr = defaultrulethickness;
    clr += labs(clr) / 4;
  }
  y = vardelimiter(leftdelimiter(q), cursize,
		   height(x) + depth(x) + clr + defaultrulethickness);
  delta = depth(y) - height(x) - depth(x) - clr;
  if (delta > 0)
    clr += half(delta);
  shiftamount(y) = -(height(x) + clr);
  link(y) = overbar(x, clr, height(y));
  info(nucleus(q)) = hpack(y, 0, additional);
  mathtype(nucleus(q)) = subbox;
}
