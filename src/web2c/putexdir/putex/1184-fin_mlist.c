Static halfword finmlist(halfword p)
{
  pointer q;

  if (incompleatnoad != 0) {   /*1185:*/
    mathtype(denominator(incompleatnoad)) = submlist;
    info(denominator(incompleatnoad)) = link(head);
    if (p == 0)
      q = incompleatnoad;
    else {
      q = info(numerator(incompleatnoad));
      if (type(q) != leftnoad)
	confusion(S(419));
      info(numerator(incompleatnoad)) = link(q);
      link(q) = incompleatnoad;
      link(incompleatnoad) = p;
    }
  }  /*:1185*/
  else {
    link(tail) = p;
    q = link(head);
  }
  popnest();
  return q;
}

