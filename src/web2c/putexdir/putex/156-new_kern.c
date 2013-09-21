/* The |new_kern| function creates a kern node having a given width. */
static pointer new_kern(scaled w)
{
  pointer p; /* the new node */
  p=get_node(small_node_size); type(p)=kern_node;
  subtype(p)=normal;
  width(p)=w;
  return p;
}
