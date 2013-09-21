/* The |new_noad| function creates an |ord_noad| that is completely null. */
static pointer new_noad(void)
{
  pointer p;
  int i=0;
  p=get_node(noadsize);
  type(p)=ord_noad; subtype(p)=normal;
#ifdef BIG_CHARNODE
  for(i=0;i<charnodesize;i++) {
#endif
  mem[nucleus(p)+i-memmin].hh=empty_field;
  mem[subscr(p)+i-memmin].hh=empty_field;
  mem[supscr(p)+i-memmin].hh=empty_field;
#ifdef BIG_CHARNODE
 }
#endif
  return p;
}
