Static void confusion(strnumber s)
{
  normalizeselector();
  if (history < errormessageissued) {
    printnl(S(292));
    print(S(297));
    print(s);
    printchar(')');
    help1(S(298));
  } else {
    printnl(S(292));
    print(S(299));
    help2(S(300),S(301));
  }
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
