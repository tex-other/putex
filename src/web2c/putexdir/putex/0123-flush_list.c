Static void flushlist(halfword p)
{
  pointer q, r;

  if (p == 0)
    return;
  r = p;
  do {
    q = r;
    r = link(r);   /*_STAT*/
    dynused-=charnodesize;   /*_ENDSTAT*/
  } while (r != 0);
  link(q) = avail;
  avail = p;
}

