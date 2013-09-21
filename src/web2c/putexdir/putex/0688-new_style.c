Static halfword newstyle(smallnumber s)
{
  pointer p;

  p = getnode(stylenodesize);
  type(p) = stylenode;
  subtype(p) = s;
  width(p) = 0;
  depth(p) = 0;
  return p;
} 
