Static halfword rebox(halfword b, long w)
{
  pointer p;
  internalfontnumber f;
  scaled v;

  if ((width(b) != w) & (listptr(b) != 0)) {
    if (type(b) == vlistnode)
      b = hpack(b, 0, additional);
    p = listptr(b);
    if (ischarnode(p) & (link(p) == 0)) {
      f = font(p);
      v = charwidth(f, charinfo(f, character(p)));
      if (v != width(b))
	link(p) = newkern(width(b) - v);
    }
    freenode(b, boxnodesize);
    b = newglue(ssglue);
    link(b) = p;
    while (link(p) != 0)
      p = link(p);
    link(p) = newglue(ssglue);
    return (hpack(b, w, exactly));
  } else {
    width(b) = w;
    return b;
  }
}
