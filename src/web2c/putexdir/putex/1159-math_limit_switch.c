Static void mathlimitswitch(void)
{
  if (head != tail) {
    if (type(tail) == opnoad) {
      subtype(tail) = curchr;
      goto _Lexit;
    }
  }
  printnl(S(292));
  print(S(903));
  help1(S(904));
  error();
_Lexit: ;
}
