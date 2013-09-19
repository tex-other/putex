void overflow(strnumber s, long n)
{
  normalizeselector();
  printnl(S(292));
  print(S(294));
  print(s);
  printchar('=');
  printint(n);
  printchar(']');
  help2(S(295),S(296));
  if (interaction == errorstopmode)
    interaction = scrollmode;
  if (logopened)   /*_DEBUG*/
    error();
  if (interaction > batchmode)
    debughelp();
  /*_ENDDEBUG*/
  history = fatalerrorstop;
  jumpout();
}
