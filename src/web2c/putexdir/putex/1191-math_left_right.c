Static void mathleftright(void)
{
  smallnumber t;
  pointer p;

  t = curchr;
  if (t == rightnoad && curgroup != mathleftgroup) {   /*1192:*/
    if (curgroup != mathshiftgroup) {
      offsave();
      return;
    }
    scandelimiter(garbage, false);
    printnl(S(292));
    print(S(558));
    printesc(S(419));
    help1(S(925));
    error();
    return;
  }
  /*:1192*/
  p = newnoad();
  type(p) = t;
  scandelimiter(delimiter(p), false);
  if (t == leftnoad) {
    pushmath(mathleftgroup);
    link(head) = p;
    tail = p;
    return;
  }
  p = finmlist(p);
  unsave();
  tailappend(newnoad());
  type(tail) = innernoad;
  mathtype(nucleus(tail)) = submlist;
  info(nucleus(tail)) = p;
}
