Static void newwhatsit(smallnumber s, smallnumber w)
{
  pointer p;

  p = getnode(w);
  type(p) = whatsitnode;
  subtype(p) = s;
  link(tail) = p;
  tail = p;
}
