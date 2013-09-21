Static void fatalerror(strnumber s)
{
  normalizeselector();
  printnl(S(292));
  print(S(293));
  helpptr = 1;
  helpline[0] = s;
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
