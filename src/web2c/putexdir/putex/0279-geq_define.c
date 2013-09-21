Static void geqdefine(halfword p, quarterword t, halfword e)
{
  eqdestroy(eqtb[p - activebase]);
  eqlevel(p) = levelone;
  eqtype(p) = t;
  equiv(p) = e;
}

