Static halfword prunepagetop(halfword p)
{
  pointer prevp, q;

  prevp = temphead;
  link(temphead) = p;
  while (p != 0) {
    switch (type(p)) {

    case hlistnode:
    case vlistnode:
    case rulenode:   /*969:*/
      q = newskipparam(splittopskipcode);
      link(prevp) = q;
      link(q) = p;
      if (width(tempptr) > height(p))
	width(tempptr) -= height(p);
      else
	width(tempptr) = 0;
      p = 0;
      break;
      /*:969*/

    case whatsitnode:
    case marknode:
    case insnode:
      prevp = p;
      p = link(prevp);
      break;

    case gluenode:
    case kernnode:
    case penaltynode:
      q = p;
      p = link(q);
      link(q) = 0;
      link(prevp) = p;
      flushnodelist(q);
      break;

    default:
      confusion(S(791));
      break;
    }
  }
  return (link(temphead));
}
