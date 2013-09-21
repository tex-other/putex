Static void appendtovlist(halfword b)
{
  scaled d;
  pointer p;

  if (prevdepth > ignoredepth) {
    d = width(baselineskip) - prevdepth - height(b);
    if (d < lineskiplimit)
      p = newparamglue(lineskipcode);
    else {
      p = newskipparam(baselineskipcode);
      width(tempptr) = d;
    }
    link(tail) = p;
    tail = p;
  }
  link(tail) = b;
  tail = b;
  prevdepth = depth(b);
}
