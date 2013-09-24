/* The procedure |eq_define| defines an |eqtb| entry having specified
|eq_type| and |equiv| fields, and saves the former value if appropriate.
This procedure is used only for entries in the first four regions of |eqtb|,
i.e., only for entries that have |eq_type| and |equiv| fields.
After calling this routine, it is safe to put four more entries on
|save_stack|, provided that there was room for four more entries before
the call, since |eq_save| makes the necessary test. */
void eq_define(pointer p, quarterword t, halfword e)
/* new data for |eqtb| */
{
  if (eq_level(p)==cur_level) eq_destroy(eqtb[p]);
  else if (cur_level>level_one) eq_save(p,eq_level(p));
  eq_level(p)=cur_level; eq_type(p)=t; equiv(p)=e;
}

