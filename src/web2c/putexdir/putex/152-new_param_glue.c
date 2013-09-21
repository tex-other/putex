/* And here's a function that creates a glue node for a given parameter
identified by its code number; for example,
|new_param_glue(line_skip_code)| returns a pointer to a glue node for the
current \.{\\lineskip}. */
static pointer new_param_glue(small_number n)
{
  pointer p; /* the new node */
  pointer q; /* the glue specifiation */
  p=get_node(small_node_size); type(p)=glue_node; subtype(p)=n+1;
  leader_ptr(p)=0;
  q=glue_par(n);
  glue_ptr(p)=q; (glue_ref_count(q))++;
  return p;
}
