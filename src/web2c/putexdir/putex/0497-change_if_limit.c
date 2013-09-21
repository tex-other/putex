Static void changeiflimit(smallnumber l, halfword p)
{
  pointer q;

  if (p == condptr)
    iflimit = l;
  else {
    q = condptr;
    while (true) {
      if (q == 0)
	confusion(S(658));
      if (link(q) == p) {
	type(q) = l;
	goto _Lexit;
      }
      q = link(q);
    }
  }
_Lexit: ;
}
