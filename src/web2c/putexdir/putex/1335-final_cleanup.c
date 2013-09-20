Static void finalcleanup(void)
{
  smallnumber c;

  c = curchr;
  if (jobname == 0)
    openlogfile();
  while (inputptr > 0) {
    if (state == tokenlist)
      endtokenlist();
    else
      endfilereading();
  }
  while (openparens > 0) {
    print(S(1019));
    openparens--;
  }
  if (curlevel > levelone) {
    printnl('(');
    printesc(S(1020));
    print(S(1021));
    printint(curlevel - levelone);
    printchar(')');
  }
  while (condptr != 0) {
    printnl('(');
    printesc(S(1020));
    print(S(1022));
    printcmdchr(iftest, curif);
    if (ifline != 0) {
      print(S(1023));
      printint(ifline);
    }
    print(S(1024));
    ifline = iflinefield(condptr);
    curif = subtype(condptr);
    tempptr = condptr;
    condptr = link(condptr);
    freenode(tempptr, ifnodesize);
  }
  if (history != spotless) {
    if (history == warningissued || interaction < errorstopmode) {
      if (selector == termandlog) {
	selector = termonly;
	printnl(S(1025));
	selector = termandlog;
      }
    }
  }
  if (c == 1) {
    for (c = topmarkcode; c <= splitbotmarkcode; c++) {
      if (curmark[c - topmarkcode] != 0)
	deletetokenref(curmark[c - topmarkcode]);
    }
    storefmtfile();
    goto _Lexit;
/* p2c: tex1.p: Note: Deleting unreachable code [255] */
  }
_Lexit: ;
}
