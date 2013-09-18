void printhex(long n)
{
  int k;
  char digs[23];

  k = 0;
  printchar('"');
  do {
    digs[k] = n & 15;
    n /= 16;
    k++;
  } while (n != 0);
  printthedigs(k,digs);
}
