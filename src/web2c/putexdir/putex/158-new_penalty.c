Static pointer new_penalty(long m)
{
  pointer p;
  p = get_node(small_node_size);
  type(p) = penalty_node;
  subtype(p) = 0;
  penalty(p) = m;
  return p;
}
