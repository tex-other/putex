Static void pushalignment(void)
{
  pointer p;

  p = getnode(alignstacknodesize);
  link(p) = alignptr;
  info(p) = curalign;
  llink(p) = preamble;
  rlink(p) = curspan;
  mem[p - memmin + 2].int_ = curloop;
  mem[p - memmin + 3].int_ = alignstate;
  info(p + 4) = curhead;
  link(p + 4) = curtail;
  alignptr = p;
  curhead = getavail();
}
