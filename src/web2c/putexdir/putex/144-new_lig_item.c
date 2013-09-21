static pointer new_lig_item(quarterword c)
{
  pointer p; /* the new node */
  p=get_node(small_node_size); character(p)=c; lig_ptr(p)=0;
  return p;
}
