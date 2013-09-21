Static void deletetokenref(halfword p)
{
  if (tokenrefcount(p) == 0)
    flushlist(p);
  else
    (tokenrefcount(p))--;
}
