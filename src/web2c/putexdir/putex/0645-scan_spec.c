Static void scanspec(groupcode c, boolean threecodes)
{
  long s=0 /* XXXX */;
  char speccode;

  if (threecodes)
    s = saved(0);
  if (scankeyword(S(697)))
    speccode = exactly;
  else if (scankeyword(S(698)))
    speccode = additional;
  else {
    speccode = additional;
    curval = 0;
    goto _Lfound;
  }
  scannormaldimen();
_Lfound:
  if (threecodes) {
    saved(0) = s;
    saveptr++;
  }
  saved(0) = speccode;
  saved(1) = curval;
  saveptr += 2;
  newsavelevel(c);
  scanleftbrace();
}
