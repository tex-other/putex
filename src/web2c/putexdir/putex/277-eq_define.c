Static void eqdefine(halfword p, quarterword t, halfword e)
{
  if (eqlevel(p) == curlevel)
    eqdestroy(eqtb[p - activebase]);
  else if (curlevel > levelone)
    eqsave(p, eqlevel(p));
  eqlevel(p) = curlevel;
  eqtype(p) = t;
  equiv(p) = e;
}

