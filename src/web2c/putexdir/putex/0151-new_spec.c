/* Here is a function that returns a pointer to a copy of a glue spec.
The reference count in the copy is |null|, because there is assumed
to be exactly one reference to the new specification. */
static pointer new_spec(pointer p) /* duplicates a glue specification */
{
  pointer q; /* the new node */
  q=get_node(glue_spec_size);
  mem[q-memmin]=mem[p-memmin]; glue_ref_count(q)=0;
  width(q)=width(p); stretch(q)=stretch(p); shrink(q)=shrink(p);
  return q;
}
