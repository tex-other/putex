Static long rounddecimals(int k, char * digs )
{
  long a;

  a = 0;
  while (k > 0) {
    k--;
    a = (a + digs[k] * two) / 10;
  }
  return ((a + 1) / 2);
}
