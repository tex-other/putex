/* Here is a subroutine that creates a new glue specification from another
one that is expressed in `\.{mu}', given the value of the math unit. */
pointer mathglue(pointer g, scaled m)
{
  pointer p; /* the new glue specification */
  integer n; /* integer part of |m| */
  scaled f; /* fraction part of |m| */

  n=x_over_n(m,65536L);f=remainder;
  if (f<0) {
    decr(n); f+=65536L;
  }
  p=get_node(glue_spec_size);
  width(p)=mult_and_add(n,width(g),xn_over_d(width(g),f,65536L),1073741823L); /* convert \.{mu} to \.{pt} */
  stretch_order(p)=stretch_order(g);
  if (stretch_order(p) == normal) stretch(p)=mult_and_add(n,stretch(g),xn_over_d(stretch(g),f,65536L),1073741823L);
  else stretch(p)=stretch(g);
  shrink_order(p)=shrink_order(g);
  if (shrink_order(p)==normal) shrink(p)=mult_and_add(n,shrink(g),xn_over_d(shrink(g),f,65536L),1073741823L);
  else shrink(p)=shrink(g);
  return p;
}
