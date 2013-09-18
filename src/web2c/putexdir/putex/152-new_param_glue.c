Static pointer new_param_glue(small_number n)
{
  pointer p, q;
  p = get_node(small_node_size);
  type(p) = glue_node;
  subtype(p) = n + 1;
  leader_ptr(p) = 0;
  q = glue_par(n);
  glueptr(p) = q;
  (glue_ref_count(q))++;
  return p;
}
