Static void showcurcmdchr(void)
{
  begindiagnostic();
  printnl('{');
  if (mode != shownmode) {
    printmode(mode);
    print(S(488));
    shownmode = mode;
  }
  printcmdchr(curcmd, curchr);
  printchar('}');
  enddiagnostic(false);
}

