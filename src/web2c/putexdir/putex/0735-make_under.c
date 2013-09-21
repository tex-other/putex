Static void makeunder(halfword q)
{
  pointer p, x, y;
  scaled delta;

  x = cleanbox(nucleus(q), curstyle);
  p = newkern(defaultrulethickness * 3);
  link(x) = p;
  link(p) = fractionrule(defaultrulethickness);
  y = vpack(x, 0, additional);
/* p2c: tex1.p: Warning: Too many arguments for foovpack [299] */
  delta = height(y) + depth(y) + defaultrulethickness;
  height(y) = height(x);
  depth(y) = delta - height(y);
  info(nucleus(q)) = y;
  mathtype(nucleus(q)) = subbox;
}
