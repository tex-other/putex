Static long multandadd(long n, long x, long y, long maxanswer)
{
  if (n < 0) {
    x = -x;
    n = -n;
  }
  if (n == 0)
    return y;
  else if (x <= (maxanswer - y) / n && -x <= (maxanswer + y) / n)
    return (n * x + y);
  else {
    aritherror = true;
    return 0;
  }
}
