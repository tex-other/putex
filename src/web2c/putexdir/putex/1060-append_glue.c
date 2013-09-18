Static void appendglue(void)
{
  smallnumber s;

  s = curchr;
  switch (s) {

  case filcode:
    curval = filglue;
    break;

  case fillcode:
    curval = fillglue;
    break;

  case sscode:
    curval = ssglue;
    break;

  case filnegcode:
    curval = filnegglue;
    break;

  case skipcode:
    scanglue(glueval);
    break;

  case mskipcode:
    scanglue(muval);
    break;
  }
  tailappend(newglue(curval));
  if (s < skipcode)
    return;
  (gluerefcount(curval))--;
  if (s > skipcode)
    subtype(tail) = muglue;
}
