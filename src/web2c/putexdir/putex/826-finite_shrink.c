Static halfword finiteshrink(halfword p)
{
  pointer q;

  if (noshrinkerroryet) {
    noshrinkerroryet = false;
    printnl(S(292));
    print(S(748));
    help5(S(749),S(750),S(751),S(752),S(753));
    error();
  }
  q = newspec(p);
  shrinkorder(q) = normal;
  deleteglueref(p);
  return q;
}
