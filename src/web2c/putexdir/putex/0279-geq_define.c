Static void geq_define(pointer p, quarterword t, halfword e)
{
  eq_destroy(eqtb[p]);
  eq_level(p)=level_one; eq_type(p)=t; equiv(p)=e;
}

