/* Here is a routine that calculates half of an integer, using an
unambiguous convention with respect to signed odd numbers. */
integer half(integer x)
{
  if (odd(x)) return ((x+1)/2);
  else return (x/2);
}
