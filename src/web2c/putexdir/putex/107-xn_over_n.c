long xnoverd(long x, long n, long d)
{
  long Result;
  boolean positive;
  nonnegativeinteger t, u, v;

  if (x >= 0)
    positive = true;
  else {
    x = -x;
    positive = false;
  }
  t = x % 32768L * n;
  u = x / 32768L * n + t / 32768L;
  v = u % d * 32768L + t % 32768L;
  if (u / d >= 32768L)
    aritherror = true;
  else
    u = u / d * 32768L + v / d;
  if (positive) {
    Result = u;
    texremainder = v % d;
  } else {
    Result = -u;
    texremainder = -(v % d);
  }
  return Result;
}
