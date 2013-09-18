Static void writeout(halfword p)
{   /*1371:*/
  char oldsetting;
  long oldmode;
  /* smallnumber */ int j; /* INT */
  pointer q, r;

  q = getavail();
  info(q) = rightbracetoken + '}';
  r = getavail();
  link(q) = r;
  info(r) = endwritetoken;
  inslist(q);
  begintokenlist(writetokens(p), writetext);
  q = getavail();
  info(q) = leftbracetoken + '{';
  inslist(q);
  oldmode = mode;
  mode = 0;
  curcs = writeloc;
  q = scantoks(false, true);
  gettoken();
  if (curtok != endwritetoken) {   /*1372:*/
    printnl(S(292));
    print(S(680));
    help2(S(681),S(682));
    error();
    do {
      gettoken();
    } while (curtok != endwritetoken);
  }
  /*:1372*/
  mode = oldmode;   /*:1371*/
  endtokenlist();
  oldsetting = selector;
  j = writestream(p);
  if (writeopen[j])
    selector = j;
  else {
    if (j == 17 && selector == termandlog)
      selector = logonly;
    printnl(S(385));
  }
  tokenshow(defref);
  println();
  flushlist(defref);
  selector = oldsetting;
}
