Static void makemark(void)
{
  pointer p;

  p = scantoks(false, true);
  p = getnode(smallnodesize);
  type(p) = marknode;
  subtype(p) = 0;
  markptr(p) = defref;
  link(tail) = p;
  tail = p;
}
