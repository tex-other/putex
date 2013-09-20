Static void unpackage(void)
{
  pointer p;
  char c;

  c = curchr;
  scaneightbitint();
  p = box(curval);
  if (p == 0)
    goto _Lexit;
  if ((labs(mode) == mmode) |
      ((labs(mode) == vmode) & (type(p) != vlistnode)) |
      ((labs(mode) == hmode) & (type(p) != hlistnode))) {
    printnl(S(292));
    print(S(872));
    help3(S(873),S(874),S(875));
    error();
    goto _Lexit;
  }
  if (c == copycode)
    link(tail) = copynodelist(listptr(p));
  else {
    link(tail) = listptr(p);
    box(curval) = 0;
    freenode(p, boxnodesize);
  }
  while (link(tail) != 0)
    tail = link(tail);
_Lexit: ;
}
