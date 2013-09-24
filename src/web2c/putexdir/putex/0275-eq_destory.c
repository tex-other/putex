/* Just before an entry of |eqtb| is changed, the following procedure should
be called to update the other data structures properly. It is important
to keep in mind that reference counts in |mem| include references from
within |save_stack|, so these counts must be handled carefully. */
void eq_destroy(memory_word w) /* gets ready to forget |w| */
{
  pointer q; /* |equiv| field of |w| */
  switch (eq_type_field(w)) {
    case call: case long_call: case outer_call: case long_outer_call:
      delete_token_ref(equiv_field(w)); break;
    case glue_ref: delete_glue_ref(equiv_field(w)); break;
    case shape_ref: q=equiv_field(w); /* we need to free a \.{\\parshape} block */
      if (q!=0)  free_node(q,info(q)+info(q)+1); break;
      /* such a block is |2n+1| words long, where |n=info(q)| */
    case box_ref: flush_node_list(equiv_field(w)); break;
    default: ;
  }
}
