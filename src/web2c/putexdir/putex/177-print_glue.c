Static void print_glue(long d, long order, str_number s)
{
  print_scaled(d);
  if ((unsigned long)order > filll) {
    print(S(329));
    return;
  }
  if (order <= normal) {
    if (s != 0)
      print(s);
    return;
  }
  print(S(330));
  while (order > fil) {
    print_char('l');
    order--;
  }
}
