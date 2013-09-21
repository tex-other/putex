Static halfword fractionrule(long t)
{
  pointer p;

  p = newrule();
  height(p) = t;
  depth(p) = 0;
  return p;
}
