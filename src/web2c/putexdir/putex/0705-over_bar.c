Static halfword overbar(halfword b, long k, long t)
{
  pointer p, q;

  p = newkern(k);
  link(p) = b;
  q = fractionrule(t);
  link(q) = p;
  p = newkern(t);
  link(p) = q;
  return (vpack(p, 0, additional));
/* p2c: tex1.p: Warning: Too many arguments for foovpack [299] */
}
