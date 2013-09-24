/* Just before \.{INITEX} writes out the memory, it sorts the doubly linked
available space list. The list is probably very short at such times, so a
simple insertion sort is used. The smallest available location will be
pointed to by |rover|, the next-smallest by |rlink(rover)|, etc. */
void sort_avail(void)
/* sorts the available variable-size nodes by location */
{
  pointer p, q, r; /* indices into |mem| */
  pointer old_rover; /* initial |rover| setting */
  p=get_node(1073741824L); /* merge adjacent free areas */
  p=rlink(rover); rlink(rover)=max_halfword; old_rover=rover;
  while (p!=old_rover) {
    if (p<rover) {
      q=p; p=rlink(q); rlink(q)=rover; rover=q;
      continue;
    } else {
      q=rover;
      while (rlink(q)<p) q=rlink(q);
      r=rlink(p); rlink(p)=rlink(q); rlink(q)=p; p=r;
    }
  }
  p=rover;
  while (rlink(p)!=max_halfword) {
    llink(rlink(p))=p; p=rlink(p);
  }
  rlink(p)=rover; llink(rover)=p;
}
