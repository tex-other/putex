Static long half(long x)
{
  if (x & 1)
    return ((x + 1) / 2);
  else
    return (x / 2);
}
