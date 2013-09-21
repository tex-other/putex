/* Here is a routine that calculates half of an integer, using an
unambiguous convention with respect to signed odd numbers. */
static integer half(integer x)
{
  if (x & 1) return ((x+1)/2);
  else return (x/2);
}
