Static long heightplusdepth(internalfontnumber f, quarterword c)
{
  fourquarters q;
  eightbits hd;

  q = charinfo(f, c);
  hd = heightdepth(q);
  return (charheight(f, hd) + chardepth(f, hd));
}
