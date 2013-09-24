/* We need to put \TeX's ``primitive'' control sequences into the hash
table, together with their command code (which will be the |eq_type|)
and an operand (which will be the |equiv|). The |primitive| procedure
does this, in a way that no \TeX\ user can. The global value |cur_val|
contains the new |eqtb| pointer after |primitive| has acted. */
void primitive(str_number s, quarterword c, halfword o)
{
  if (s<256) cur_val=s+single_base;
  else {
	cur_val = idlookup_s(s,false);
    flush_string();
    text(cur_val)=s;
  }
  eq_level(cur_val)=level_one; eq_type(cur_val)=c; equiv(cur_val)=o;
}
