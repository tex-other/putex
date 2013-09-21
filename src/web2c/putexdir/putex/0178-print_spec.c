Static void printspec(long p, str_number s)
{
  if (p >= lo_mem_max) {
    printchar('*');
    return;
  }
  print_scaled(width(p));
  if (s != 0)
    print(s);
  if (stretch(p) != 0) {
    print(S(331));
    print_glue(stretch(p), stretch_order(p), s);
  }
  if (shrink(p) != 0) {
    print(S(332));
    print_glue(shrink(p), shrink_order(p), s);
  }
}

