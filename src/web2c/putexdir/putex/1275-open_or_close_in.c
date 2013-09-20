Static void openorclosein(void)
{
  int c;
  int n;

  c = curchr;
  scanfourbitint();
  n = curval;
  if (readopen[n] != closed) {
    aclose(&readfile[n]);
    readopen[n] = closed;
  }
  if (c == 0)
    return;
  scanoptionalequals();
  scanfilename();
  if (curext == S(385))
    curext = S(669);
  packfilename(curname,S(677),curext);
  if (aopenin(&readfile[n]))
    readopen[n] = justopen;
}

