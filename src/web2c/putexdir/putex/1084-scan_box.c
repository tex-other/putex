Static void scanbox(long boxcontext)
{   /*404:*/
  do {
    getxtoken();   /*:404*/
  } while (curcmd == spacer || curcmd == relax);
  if (curcmd == makebox) {
    beginbox(boxcontext);
    return;
  }
  if (boxcontext >= leaderflag && (curcmd == hrule || curcmd == vrule)) {
    curbox = scanrulespec();
    boxend(boxcontext);
    return;
  }
  printnl(S(292));
  print(S(859));
  help3(S(860),S(861),S(862));
  backerror();
}
