void print_delimiter(pointer p) /* prints a delimiter as 24-bit hex value */
{
  integer a; /* accumulator */
  a=small_fam(p)*256+qo(small_char(p));
  a=a*4096+large_fam(p)*256+qo(large_char(p));
  if (a<0) print_int(a); /* this should never happen */
  else print_hex(a);
}
