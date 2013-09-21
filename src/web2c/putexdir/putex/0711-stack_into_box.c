Static void stackintobox(halfword b, internalfontnumber f, quarterword c)
{
  pointer p;

  p = charbox(f, c);
  link(p) = listptr(b);
  listptr(b) = p;
  height(b) = height(p);
}
