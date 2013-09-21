Static void print_delimiter(halfword p)
{
  long a;
  a = small_fam(p) * 256 + small_char(p) - minquarterword;
  a = a * 4096 + large_fam(p) * 256 + large_char(p) - minquarterword;
  if (a < 0)
    print_int(a);
  else
    print_hex(a);
}
