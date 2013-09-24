/* The procedure |flush_list(p)| frees an entire linked list of
one-word nodes that starts at position |p|. */
void flush_list(pointer p)
/* makes list of single-word nodes available */
{
  pointer q, r; /* list traversers */
  if (p==0) return;
  r=p;
  do {
    q=r; r=link(r); dynused-=char_node_size;
  } while (r!=0);
  link(q)=avail;
  avail=p;
}

