void make_over(pointer q)
{
  info(nucleus(q))=
      overbar(clean_box(nucleus(q),cramped_style(curs_tyle)),
      default_rule_thickness*3, default_rule_thickness);
  math_type(nucleus(q))=sub_box;
}
