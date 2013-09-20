Static void alterpagesofar(void)
{
  int c;

  c = curchr;
  scanoptionalequals();
  scannormaldimen();
  pagesofar[c] = curval;
}
