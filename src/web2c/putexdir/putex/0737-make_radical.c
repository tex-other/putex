/* According to the rules in the \.{DVI} file specifications, we ensure alignment
@^square roots@>
between a square root sign and the rule above its nucleus by assuming that the
baseline of the square-root symbol is the same as the bottom of the rule. The
height of the square-root symbol will be the thickness of the rule, and the
depth of the square-root symbol should exceed or equal the height-plus-depth
of the nucleus plus a certain minimum clearance~|clr|. The symbol will be
placed so that the actual clearance is |clr| plus half the excess. */
void make_radical(pointer q)
{
  pointer x, y; /* temporary registers for box construction */
  scaled delta, clr; /* dimensions involved in the calculation */

  x=clean_box(nucleus(q),cramped_style(curs_tyle));
  if (cur_style<text_style)
    clr=default_rule_thickness+labs(math_x_height(cur_size))/4;
  else {
    clr=default_rule_thickness; clr+=labs(clr)/4;
  }
  y=var_delimiter(left_delimiter(q),cur_size,height(x)+depth(x)+clr+
          default_rule_thickness);
  delta=depth(y)-height(x)-depth(x)-clr;
  if (delta>0)  clr+=half(delta); /* increase the actual clearance */
  shift_amount(y)=-(height(x)+clr);
  link(y)=overbar(x,clr,height(y));
  info(nucleus(q))=hpack(y,0,additional); math_type(nucleus(q))=sub_box;
}
