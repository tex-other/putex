/* The first arithmetical subroutine we need computes $nx+y$, where |x|
and~|y| are |scaled| and |n| is an integer. We will also use it to
multiply integers. */
scaled mult_and_add(integer n, scaled x, scaled y, scaled max_answer)
{
  if (n<0) {
    x=-x; n=-n;
  }
  if (n==0) return y;
  else if (x<=(maxanswer-y)/n && -x<=(maxanswer+y)/n)
    return (n*x+y);
  else {
    arith_error=true; return 0;
  }
}
