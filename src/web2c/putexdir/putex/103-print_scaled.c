void printscaled(long s)
{
  scaled delta;

  if (s < 0) {
    printchar('-');
    s = -s;
  }
  printint(s / unity);
  printchar('.');
  s = (s & (unity - 1)) * 10 + 5;
  delta = 10;
  do {
    if (delta > unity)
      s -= 17232;
    printchar('0' + s / unity);
    s = (s & (unity - 1)) * 10;
    delta *= 10;
  } while (s > delta);
}
