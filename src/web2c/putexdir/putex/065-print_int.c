void printint(long n)
{
  int k;
  long m;
  char dig[23];

  k = 0;
  if (n < 0) {
    printchar('-');
    if (n > -100000000L)
      n = -n;
    else {
      m = -n - 1;
      n = m / 10;
      m = m % 10 + 1;
      k = 1;
      if (m < 10)
	dig[0] = m;
      else {
	dig[0] = 0;
	n++;
      }
    }
  }
  do {
    dig[k] = n % 10;
    n /= 10;
    k++;
  } while (n != 0);
  printthedigs(k,dig);
}
