Static void popalignment(void)
{
  pointer p;

  freeavail(curhead);
  p = alignptr;
  curtail = link(p + 4);
  curhead = info(p + 4);
  alignstate = mem[p - memmin + 3].int_;
  curloop = mem[p - memmin + 2].int_;
  curspan = rlink(p);
  preamble = llink(p);
  curalign = info(p);
  alignptr = link(p);
  freenode(p, alignstacknodesize);
}
