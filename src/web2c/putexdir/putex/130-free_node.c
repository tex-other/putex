Static void free_node(halfword p, halfword s)
{
  pointer q;
  node_size(p) = s;
  link(p) = empty_flag;
  q = llink(rover);
  llink(p) = q;
  rlink(p) = rover;
  llink(rover) = p;
  rlink(q) = p;
  varused -= s;
}
