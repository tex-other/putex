Static void printromanint(long n)
{
  int j, k;
  nonnegativeinteger u, v;
  static char romstr[]="m2d5c2l5x2v5i";

  j = 0 ; 
  v = 1000;
  while (true) {
    while (n >= v) {
      printchar(romstr[j]);
      n -= v;
    }
    if (n <= 0)
      goto _Lexit;
    k = j + 2;
    u = v / (romstr[k - 1] - '0');
    if (romstr[k - 1] == '2') {
      k += 2;
      u /= romstr[k - 1] - '0';
    }
    if (n + u >= v) {
      printchar(romstr[k]);
      n += u;
    } else {
      j += 2;
      v /= romstr[j - 1] - '0';
    }
  }
_Lexit: ;
}
