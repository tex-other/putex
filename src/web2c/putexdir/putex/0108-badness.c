/* The next subroutine is used to compute the ``badness'' of glue, when a
total~|t| is supposed to be made from amounts that sum to~|s|.  According
to {\sl The \TeX book}, the badness of this situation is $100(t/s)^3$;
however, badness is simply a heuristic, so we need not squeeze out the
last drop of accuracy when computing it. All we really want is an
approximation that has similar properties.
@:TeXbook}{\sl The \TeX book@>

The actual method used to compute the badness is easier to read from the
program than to describe in words. It produces an integer value that is a
reasonably close approximation to $100(t/s)^3$, and all implementations
of \TeX\ should use precisely this method. Any badness of $2^{13}$ or more is
treated as infinitely bad, and represented by 10000.

It is not difficult to prove that $$\hbox{|badness(t+1,s)>=badness(t,s)
>=badness(t,s+1)|}.$$ The badness function defined here is capable of
computing at most 1095 distinct values, but that is plenty. */
halfword badness(scaled t, scaled s) /* compute badness, given |t>=0| */
{
  integer r; /* approximation to $\alpha t/s$, where $\alpha^3\approx
  100\cdot2^{18}$ */
  if (t==0) return 0;
  else if (s<=0) return inf_bad;
  else {
    if (t<=7230584L) r=t*297/s; /* $297^3=99.94\times2^{18}$ */
    else if (s >= 1663497L) r=t/(s/297);
    else r=t;
    if (r>1290) return inf_bad;
    else return ((r*r*r+131072L)/262144L);
  } /* that was $r^3/2^{18}$, rounded to the nearest integer */
}
