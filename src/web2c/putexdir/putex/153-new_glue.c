Static pointer new_glue(halfword q)
{
  pointer p;
  p = get_node(small_node_size);
  type(p) = glue_node;
  subtype(p) = normal;
  leader_ptr(p) = 0;
  glue_ptr(p) = q;
  (glue_ref_count(q))++;
  return p;
}
