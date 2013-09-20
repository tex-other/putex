Static void eqworddefine(halfword p, long w)
{
  if (xeqlevel[p - intbase] != curlevel) {
    eqsave(p, xeqlevel[p - intbase]);
    xeqlevel[p - intbase] = curlevel;
  }
  eqtb[p - activebase].int_ = w;
}

