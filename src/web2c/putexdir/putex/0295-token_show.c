/* Here's the way we sometimes want to display a token list, given a pointer
to its reference count; the pointer may be null. */
void token_show(pointer p)
{
  if (p!=0) show_token_list(link(p),0,10000000L);
} 
