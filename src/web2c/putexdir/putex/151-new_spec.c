Static pointer new_spec(halfword p)
{
  pointer q;
  q = get_node(glue_spec_size);
  mem[q-memmin] = mem[p-memmin];
  glue_ref_count(q) = 0;
  width(q) = width(p);
  stretch(q) = stretch(p);
  shrink(q) = shrink(p);
  return q;
}
