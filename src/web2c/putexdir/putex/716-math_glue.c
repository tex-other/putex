Static halfword mathglue(halfword g, long m)
{
  pointer p;
  long n;
  scaled f;

  n = xovern(m, 65536L);
  f = texremainder;
  if (f < 0) {
    n--;
    f += 65536L;
  }
  p = getnode(gluespecsize);
  width(p) = multandadd(n, width(g), xnoverd(width(g), f, 65536L),
			    1073741823L);
  stretchorder(p) = stretchorder(g);
  if (stretchorder(p) == normal)
    stretch(p) = multandadd(n, stretch(g),
	xnoverd(stretch(g), f, 65536L), 1073741823L);
  else
    stretch(p) = stretch(g);
  shrinkorder(p) = shrinkorder(g);
  if (shrinkorder(p) == normal)
    shrink(p) = multandadd(n, shrink(g),
			       xnoverd(shrink(g), f, 65536L),
			       1073741823L);
  else
    shrink(p) = shrink(g);
  return p;
}
