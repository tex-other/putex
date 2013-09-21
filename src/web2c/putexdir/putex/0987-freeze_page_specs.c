Static void freezepagespecs(smallnumber s)
{
  pagecontents = s;
  pagegoal = vsize;
  pagemaxdepth = maxdepth;
  pagedepth = 0;
  setpagesofarzero(1);
  setpagesofarzero(2);
  setpagesofarzero(3);
  setpagesofarzero(4);
  setpagesofarzero(5);
  setpagesofarzero(6);
  leastpagecost = awfulbad;   /*_STAT*/
  if (tracingpages <= 0)   /*_ENDSTAT*/
    return;
  begindiagnostic();
  printnl(S(804));
  printscaled(pagegoal);
  print(S(805));
  printscaled(pagemaxdepth);
  enddiagnostic(false);
}
