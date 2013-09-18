Static long xovern(long x, long n)
{
  long Result;
  boolean negative;

  negative = false;
  if (n == 0) {
    aritherror = true;
    Result = 0;
    texremainder = x;
  } else {
    if (n < 0) {
      x = -x;
      n = -n;
      negative = true;
    }
    if (x >= 0) {
      Result = x / n;
      texremainder = x % n;
    } else {
      Result = -(-x / n);
      texremainder = -(-x % n);
    }
  }
  if (negative)
    texremainder = -texremainder;
  return Result;
}

