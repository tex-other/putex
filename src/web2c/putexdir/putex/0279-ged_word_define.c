Static void geqworddefine(halfword p, long w)
{
  eqtb[p - activebase].int_ = w;
  xeqlevel[p - intbase] = levelone;
}
