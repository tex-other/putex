/* The |math_kern| subroutine removes |mu_glue| from a kern node, given
the value of the math unit. */
void math_kern(pointer p, scaled m)
{
  integer n; /* integer part of |m| */
  scaled f; /* fraction part of |m| */

  if (subtype(p)!=mu_glue) return;
  n = x_over_n(m,65536L); f=remainder;
  if (f<0) {
    n--; f+=65536L;
  }
  width(p)=mult_and_add(n,width(p),xn_over_d(width(p),f,65536L),1073741823L);
  subtype(p) = explicit;
}
