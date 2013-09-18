Static void mathradical(void)
{
  int i=0;
  tailappend(getnode(radicalnoadsize));
  type(tail) = radicalnoad;
  subtype(tail) = normal;
#ifdef BIG_CHARNODE
  for(i=0;i<charnodesize;i++) {
#endif
  mem[nucleus(tail) + i - memmin].hh = emptyfield;
  mem[subscr(tail) + i - memmin].hh = emptyfield;
  mem[supscr(tail) + i - memmin].hh = emptyfield;
#ifdef BIG_CHARNODE
  }
#endif
  scandelimiter(leftdelimiter(tail), true);
  scanmath(nucleus(tail));
}
