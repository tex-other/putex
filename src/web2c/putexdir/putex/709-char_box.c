Static halfword charbox(internalfontnumber f, quarterword c)
{
  fourquarters q;
  eightbits hd;
  pointer b, p;

  q = charinfo(f, c);
  hd = heightdepth(q);
  b = newnullbox();
  width(b) = charwidth(f, q) + charitalic(f, q);
  height(b) = charheight(f, hd);
  depth(b) = chardepth(f, hd);
  p = getavail();
  character(p) = c;
  font(p) = f;
  listptr(b) = p;
  return b;
}

