Static halfword newnoad(void)
{
  pointer p;
  int i=0;
  p = getnode(noadsize);
  type(p) = ordnoad;
  subtype(p) = normal;
#ifdef BIG_CHARNODE
  for(i=0;i<charnodesize;i++) {
#endif
  mem[nucleus(p) + i - memmin].hh = emptyfield;
  mem[subscr(p) + i - memmin].hh = emptyfield;
  mem[supscr(p) + i - memmin].hh = emptyfield;
#ifdef BIG_CHARNODE
 }
#endif
  return p;
}
