Static void doassignments(void)
{
  while (true) {   /*404:*/
    do {
      getxtoken();   /*:404*/
    } while (curcmd == spacer || curcmd == relax);
    if (curcmd <= maxnonprefixedcommand)
      goto _Lexit;
    setboxallowed = false;
    prefixedcommand();
    setboxallowed = true;
  }
_Lexit: ;
}
