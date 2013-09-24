/* The counterpart of |eq_define| for the remaining (fullword) positions in
|eqtb| is called |eq_word_define|. Since |xeq_level[p]>=level_one| for all
|p|, a `|restore_zero|' will never be used in this case. */
void eq_word_define(halfword p, integer w)
{
  if (xeq_level[p]!=cur_level) {
    eq_save(p,xeq_level[p]); xeq_level[p]=cur_level;
  }
  eqtb[p].int_=w;
}

