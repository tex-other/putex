Static halfword newcharacter(internalfontnumber f, eightbits c)
{
  halfword Result;
  pointer p;

  if (fontbc[f ] <= c) {
    if (fontec[f ] >= c) {
      if (charexists(charinfo(f, c))) {
	p = getavail();
	font(p) = f;
	character(p) = c;
	Result = p;
	goto _Lexit;
      }
    }
  }
  charwarning(f, c);
  Result = 0;
_Lexit:
  return Result;
}
