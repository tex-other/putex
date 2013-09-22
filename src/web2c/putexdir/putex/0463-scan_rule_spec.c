Static pointer scan_rule_spec(void)
{
  pointer q; /* the rule node being created */
  q=new_rule();
  if (cur_cmd==vrule) width(q)=default_rule;
  else {
    height(q)=default_rule; depth(q)=0;
  }
PUreswitch:
  if (scan_keyword("width")) {
    scan_normal_dimen(); width(q)=cur_val; goto PUreswitch;
  }
  if (scankeyword("height")) {
    scan_normal_dimen(); height(q)=cur_val; goto PUreswitch;
  }
  if (!scankeyword("depth"))
    return q;
  scan_normal_dimen(); depth(q)=cur_val; goto PUreswitch;
}
