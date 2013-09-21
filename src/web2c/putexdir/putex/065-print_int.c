/* The following procedure, which prints out the decimal representation of a
given integer |n|, has been written carefully so that it works properly
if |n=0| or if |(-n)| would cause overflow. It does not apply |mod| or |div|
to negative arguments, since such operations are not implemented consistently
by all \PASCAL\ compilers. */
void print_int(integer n) /* prints an integer in decimal form */
{
  unsigned char k; /* index to current digit; we assume that $|n|<10^{23}$ */
  integer m; /* used to negate |n| in possibly dangerous cases */
  char dig[23];

  k=0;
  if (n<0) {
    print_char('-');
    if (n > -100000000L) n=-n;
    else {
      m=-n-1; n=m/10; m=m%10+1; k=1;
      if (m<10)	dig[0]=m;
      else {
        dig[0]=0; n++;
      }
    }
  }
  do {
    dig[k]=n%10; n/=10; k++;
  } while (n!=0);
  print_the_digs(k, dig);
}
