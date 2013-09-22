/* Conversely, here is a procedure analogous to |print_int|. If the output
of this procedure is subsequently read by \TeX\ and converted by the
|round_decimals| routine above, it turns out that the original value will
be reproduced exactly; the ``simplest'' such decimal number is output,
but there is always at least one digit following the decimal point.

The invariant relation in the \&{repeat} loop is that a sequence of
decimal digits yet to be printed will yield the original number if and only if
they form a fraction~$f$ in the range $s-\delta\L10\cdot2^{16}f<s$.
We can stop if and only if $f=0$ satisfies this condition; the loop will
terminate before $s$ can possibly become zero. */
void print_scaled(scaled s)
{
  scaled delta; /* amount of allowable inaccuracy */
  if (s<0) {
    print_char('-'); negate(s); /* print the sign, if negative */
  }
  print_int(s/unity); /* print the integer part */
  print_char('.');
  s=(s&(unity-1))*10+5; delta=10;
  do {
    if (delta>unity) s-=17232; /* round the last digit */
    print_char('0'+s/unity); s=(s&(unity-1))*10; delta*=10;
  } while (s>delta);
}
