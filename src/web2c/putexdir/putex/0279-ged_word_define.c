/* The |eq_define| and |eq_word_define| routines take care of local definitions.
@^global definitions@>
Global definitions are done in almost the same way, but there is no need
to save old values, and the new value is associated with |level_one|. */
void geq_word_define(pointer p, integer w) /* global |eq_word_define| */
{
  eqtb[p].int_=w; xeq_level[p]=level_one;
}
