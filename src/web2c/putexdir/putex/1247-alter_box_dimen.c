Static void alterboxdimen(void)
{
  smallnumber c;
  eightbits b;

  c = curchr;
  scaneightbitint();
  b = curval;
  scanoptionalequals();
  scannormaldimen();
  if (box(b) != 0)
    mem[box(b) + c - memmin].sc = curval;
}
