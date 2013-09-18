Static halfword badness(long t, long s)
{
  long r;

  if (t == 0)
    return 0;
  else if (s <= 0)
    return infbad;
  else {
    if (t <= 7230584L)
      r = t * 297 / s;
    else if (s >= 1663497L)
      r = t / (s / 297);
    else
      r = t;
    if (r > 1290)
      return infbad;
    else
      return ((r * r * r + 131072L) / 262144L);
  }
}
