Static void mathkern(halfword p, long m)
{
  long n;
  scaled f;

  if (subtype(p) != muglue)
    return;
  n = xovern(m, 65536L);
  f = texremainder;
  if (f < 0) {
    n--;
    f += 65536L;
  }
  width(p) = multandadd(n, width(p), xnoverd(width(p), f, 65536L),
			    1073741823L);
  subtype(p) = explicit;
}
