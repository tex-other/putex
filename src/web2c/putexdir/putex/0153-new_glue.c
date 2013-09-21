/* Glue nodes that are more or less anonymous are created by |new_glue|,
whose argument points to a glue specification. */
static pointer new_glue(pointer q)
{
  pointer p; /* the new node */
  p=get_node(small_node_size); type(p)=glue_node; subtype(p)=normal;
  leader_ptr(p)=0; glue_ptr(p)=q; (glue_ref_count(q))++;
  return p;
}
