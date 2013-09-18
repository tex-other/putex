Static void scanleftbrace(void)
{  /*404:*/
  do {
    getxtoken();   /*:404*/
  } while (curcmd == spacer || curcmd == relax);
  if (curcmd == leftbrace)
    return;
  printnl(S(292));
  print(S(566));
  help4(S(567),S(568),S(569),S(570));
  backerror();
  curtok = leftbracetoken + '{';
  curcmd = leftbrace;
  curchr = '{';
  alignstate++;
}

