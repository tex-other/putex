void make_under(pointer q)
{
  pointer p, x, y; /* temporary registers for box construction */
  scaled delta; /* overall height plus depth */

  x=clean_box(nucleus(q), cur_style);
  p=new_kern(default_rule_thickness*3); link(x)=p;
  link(p)=fraction_rule(default_rule_thickness);
  y=vpack(x,0,additional);
  delta=height(y)+depth(y)+default_rule_thickness;
  height(y)=height(x); depth(y)=delta-height(y);
  info(nucleus(q))=y; math_type(nucleus(q))=sub_box;
}
