Static pointer new_kern(long w)
{
  pointer p;
  p = get_node(small_node_size);
  type(p) = kern_node;
  subtype(p) = normal;
  width(p) = w;
  return p;
}
