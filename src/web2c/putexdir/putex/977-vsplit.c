Static halfword vsplit(eightbits n, long h)
{
  halfword Result;
  pointer v, p, q;

  v = box(n);
  if (splitfirstmark != 0) {
    deletetokenref(splitfirstmark);
    splitfirstmark = 0;
    deletetokenref(splitbotmark);
    splitbotmark = 0;
  }
  /*978:*/
  if (v == 0) {
    Result = 0;
    goto _Lexit;
  }
  if (type(v) != vlistnode) {   /*:978*/
    printnl(S(292));
    print(S(385));
    printesc(S(797));
    print(S(798));
    printesc(S(799));
    help2(S(800),S(801));
    error();
    Result = 0;
    goto _Lexit;
  }
  q = vertbreak(listptr(v), h, splitmaxdepth);   /*979:*/
  p = listptr(v);
  if (p == q)
    listptr(v) = 0;
  else {
    while (true) {
      if (type(p) == marknode) {
	if (splitfirstmark == 0) {
	  splitfirstmark = markptr(p);
	  splitbotmark = splitfirstmark;
	  tokenrefcount(splitfirstmark) += 2;
	} else {
	  deletetokenref(splitbotmark);
	  splitbotmark = markptr(p);
	  addtokenref(splitbotmark);
	}
      }
      if (link(p) == q) {
	link(p) = 0;
	goto _Ldone;
      }
      p = link(p);
    }
  }
_Ldone:   /*:979*/
  q = prunepagetop(q);
  p = listptr(v);
  freenode(v, boxnodesize);
  if (q == 0)
    box(n) = 0;
  else {
    box(n) = vpack(q, 0, additional);
/* p2c: tex1.p: Warning: Too many arguments for foovpack [299] */
  }
  Result = vpackage(p, h, exactly, splitmaxdepth);
_Lexit:
  return Result;
}
