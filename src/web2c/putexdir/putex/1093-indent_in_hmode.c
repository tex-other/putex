Static void indentinhmode(void)
{
  pointer p, q;

  if (curchr <= 0)
    return;
  p = newnullbox();
  width(p) = parindent;
  if (labs(mode) == hmode)
    spacefactor = 1000;
  else {
    q = newnoad();
    mathtype(nucleus(q)) = subbox;
    info(nucleus(q)) = p;
    p = q;
  }
  tailappend(p);
}
