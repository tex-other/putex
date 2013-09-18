Static void boxerror(eightbits n)
{
  error();
  begindiagnostic();
  printnl(S(690));
  showbox(box(n));
  enddiagnostic(true);
  flushnodelist(box(n));
  box(n) = 0;
}
