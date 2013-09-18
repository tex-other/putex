Static pointer new_ligature(quarterword f, quarterword c, halfword q)
{
  pointer p;
  p = get_node(small_node_size);
  type(p) = ligature_node;
  font_ligchar(p) = f;
  character_ligchar(p) = c;
  ligptr(p) = q;
  subtype(p) = 0;
  return p;
}
