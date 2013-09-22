/* The following function is used to create a scaled integer from a given decimal
fraction $(.d_0d_1\ldots d_{k-1})$, where |0<=k<=17|. The digit $d_i$ is
given in |dig[i]|, and the calculation produces a correctly rounded result. */
static scaled round_decimals(small_number k, char * digs)
{
  /* converts a decimal fraction */
  integer a;
  a = 0;
  while (k>0) {
    k--; a=(a+digs[k]*two)/10;
  }
  return ((a+1)/2);
}
