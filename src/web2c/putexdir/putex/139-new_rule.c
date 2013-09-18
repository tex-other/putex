Static pointer new_rule(void)
{
  pointer p;
  p = getnode(rule_node_size);
  type(p) = rule_node;
  subtype(p) = 0;
  width(p) = null_flag;
  depth(p) = null_flag;
  height(p) = null_flag;
  return p;
}
