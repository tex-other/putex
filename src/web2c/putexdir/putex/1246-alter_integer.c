Static void alterinteger(void)
{
  char c;

  c = curchr;
  scanoptionalequals();
  scanint();
  if (c == 0)
    deadcycles = curval;
  else
    insertpenalties = curval;
}
