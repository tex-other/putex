Static void makevcenter(halfword q)
{
  pointer v;
  scaled delta;

  v = info(nucleus(q));
  if (type(v) != vlistnode)
    confusion(S(415));
  delta = height(v) + depth(v);
  height(v) = axisheight(cursize) + half(delta);
  depth(v) = delta - height(v);
}
