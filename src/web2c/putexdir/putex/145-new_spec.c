Static pointer new_disc(void)
{
  pointer p;
  p = get_node(small_node_size);
  type(p) = disc_node;
  replace_count(p) = 0;
  preb_reak(p) = 0;
  post_break(p) = 0;
  return p;
}

