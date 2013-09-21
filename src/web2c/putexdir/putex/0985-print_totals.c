Static void printtotals(void)
{
  printscaled(pagetotal);
  if (pagesofar[2] != 0) {
    print(S(331));
    printscaled(pagesofar[2]);
    print(S(385));
  }
  if (pagesofar[3] != 0) {
    print(S(331));
    printscaled(pagesofar[3]);
    print(S(330));
  }
  if (pagesofar[4] != 0) {
    print(S(331));
    printscaled(pagesofar[4]);
    print(S(802));
  }
  if (pagesofar[5] != 0) {
    print(S(331));
    printscaled(pagesofar[5]);
    print(S(803));
  }
  if (pageshrink != 0) {
    print(S(332));
    printscaled(pageshrink);
  }
}

