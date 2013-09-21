/* The |new_null_box| function returns a pointer to an |hlist_node| in
which all subfields have the values corresponding to `\.{\\hbox\{\}}'.
The |subtype| field is set to |min_quarterword|, since that's the desired
|span_count| value if this |hlist_node| is changed to an |unset_node|. */
static pointer new_null_box(void) /* creates a new box node */
{
  pointer p; /* the new node */
  p=get_node(box_node_size); type(p)=hlist_node;
  subtype(p)=min_quarterword;
  width(p)=0; depth(p)=0; height(p)=0; shift_amount(p)=0; list_ptr(p)=0;
  glue_sign(p)=normal; glue_order(p)=normal; glue_set(p)=0.0;
  return p;
}
