Static pointer new_null_box(void)
{
  pointer p;
  p = getnode(box_node_size);
  type(p) = hlist_node;
  subtype(p) = min_quarterword;
  width(p) = 0;
  depth(p) = 0;
  height(p) = 0;
  shift_amount(p) = 0;
  listptr(p) = 0;
  gluesign(p) = normal;
  glueorder(p) = normal;
  glueset(p) = 0.0;
  return p;
}
