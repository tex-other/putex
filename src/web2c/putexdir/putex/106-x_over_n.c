/* We also need to divide scaled dimensions by integers. */
static scaled x_overn_(scaled x, integer n)
{
  long Result;
  boolean negative; /* should |remainder| be negated? */
  negative = false;
  if (n==0) {
    arith_error=true; Result=0; remainder=x;
  } else {
    if (n<0) {
      x=-x; n=-n; negative=true;
    }
    if (x>=0) {
      Result=x/n; remainder=x%n;
    } else {
      Result=-(-x/n); remainder=-(-x%n);
    }
  }
  if (negative)
    remainder=-remainder;
  return Result;
}

