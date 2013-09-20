Static void primitive(strnumber s, quarterword c, halfword o)
{
  if (s < 256)
    curval = s + singlebase;
  else {
	curval = idlookup_s(s,false);
    flushstring();
    text(curval) = s;
  }
  eqlevel(curval) = levelone;
  eqtype(curval) = c;
  equiv(curval) = o;
}
