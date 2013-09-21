Static void sort_avail(void)
{
  pointer p, q, r;
  pointer old_rover;
  p = get_node(1073741824L);
  p = rlink(rover);
  rlink(rover) = max_halfword;
  old_rover = rover;
  while (p != oldr_over) {   /*132:*/
    if (p < rover) {
      q = p;
      p = rlink(q);
      rlink(q) = rover;
      rover = q;
      continue;
    }
    q = rover;
    while (rlink(q) < p)
      q = rlink(q);
    r = rlink(p);
    rlink(p) = rlink(q);
    rlink(q) = p;
    p = r;
  }
  p = rover;
  while (rlink(p) != max_halfword) {
    llink(rlink(p)) = p;
    p = rlink(p);
  }
  rlink(p) = rover;
  llink(rover) = p;
}
