Static void enddiagnostic(boolean blankline)
{
  printnl(S(385));
  if (blankline)
    println();
  selector = diag_oldsetting;
}
