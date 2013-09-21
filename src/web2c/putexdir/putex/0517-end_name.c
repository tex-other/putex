Static void endname(void)
{
  curarea = S(385);
  if (extdelimiter == 0) {
	curext = S(385);
	curname = makestring();
  } else { 
	curname = extdelimiter;
	curext = makestring();
  }
}
