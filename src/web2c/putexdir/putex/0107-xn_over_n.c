/* Then comes the multiplication of a scaled number by a fraction |n/d|,
where |n| and |d| are nonnegative integers |<=@t$2^{16}$@>| and |d| is
positive. It would be too dangerous to multiply by~|n| and then divide
by~|d|, in separate operations, since overflow might well occur; and it
would be too inaccurate to divide by |d| and then multiply by |n|. Hence
this subroutine simulates 1.5-precision arithmetic. */
scaled xn_over_d(scaled x, integer n, integer d)
{
  long Result;
  boolean positive; /* was |x>=0|? */
  non_negative_integer t, u, v; /* intermediate quantities */
  if (x>=0) positive=true;
  else {
    x=-x; positive=false;
  }
  t=(x%32768L)*n;
  u=(x/32768L)*n+(t/32768L);
  v=(u%d)*32768L+(t%32768L);
  if (u/d>=32768L) arith_error=true;
  else u=(u/d)*32768L+(v/d);
  if (positive) {
    Result=u; remainder=v%d;
  } else {
    Result=-u; remainder=-(v%d);
  }
  return Result;
}
