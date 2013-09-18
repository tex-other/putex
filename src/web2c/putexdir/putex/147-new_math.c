Static pointer new_math(long w, small_number s)
{
  pointer p;
  p = get_node(small_node_size);
  type(p) = math_node;
  subtype(p) = s;
  width(p) = w;
  return p;
}
