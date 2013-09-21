/* Here is a trivial procedure to print two digits; it is usually called with
a parameter in the range |0<=n<=99|. */
static void print_two(integer n) /* prints two least significant digits */
{
  n=labs(n)%100; print_char('0'+n/10);
  print_char('0'+n%10);
}
