Static halfword vertbreak(halfword p, long h, long d)
{
  pointer prevp, q, r, bestplace=p /* XXXX */ ;
  long pi=0 /* XXXX */, b, leastcost;
  scaled prevdp;
  smallnumber t;

  prevp = p;
  leastcost = awfulbad;
  setheightzero(1);
  setheightzero(2);
  setheightzero(3);
  setheightzero(4);
  setheightzero(5);
  setheightzero(6);
  prevdp = 0;
  while (true) {  /*972:*/
    if (p == 0)   /*974:*/
      pi = ejectpenalty;
    else {  /*973:*/
      switch (type(p)) {   /*:973*/

      case hlistnode:
      case vlistnode:
      case rulenode:
	curheight += prevdp + height(p);
	prevdp = depth(p);
	goto _Lnotfound;
	break;

      case whatsitnode:   /*1365:*/
	goto _Lnotfound;   /*:1365*/
	break;

      case gluenode:
	if (!precedesbreak(prevp))
	  goto _Lupdateheights_;
	pi = 0;
	break;

      case kernnode:
	if (link(p) == 0)
	  t = penaltynode;
	else
	  t = type(link(p));
	if (t != gluenode)
	  goto _Lupdateheights_;
	pi = 0;
	break;

      case penaltynode:
	pi = penalty(p);
	break;

      case marknode:
      case insnode:
	goto _Lnotfound;
	break;

      default:
	confusion(S(792));
	break;
      }
    }
    if (pi < infpenalty)   /*:974*/
    {  /*975:*/
      if (curheight < h) {
	if (activeheight[2] != 0 || activeheight[3] != 0 ||
	    activeheight[4] != 0)
	  b = 0;
	else
	  b = badness(h - curheight, activeheight[1]);
      } else if (curheight - h > activeheight[5])
	b = awfulbad;
      else
	b = badness(curheight - h, activeheight[5]);   /*:975*/
      if (b < awfulbad) {
	if (pi <= ejectpenalty)
	  b = pi;
	else if (b < infbad)
	  b += pi;
	else
	  b = deplorable;
      }
      if (b <= leastcost) {
	bestplace = p;
	leastcost = b;
	bestheightplusdepth = curheight + prevdp;
      }
      if (b == awfulbad || pi <= ejectpenalty)
	goto _Ldone;
    }
    if ((type(p) < gluenode) | (type(p) > kernnode))
      goto _Lnotfound;
_Lupdateheights_:   /*976:*/
    if (type(p) == kernnode)
      q = p;
    else {
      q = glueptr(p);
      activeheight[stretchorder(q) + 1] += stretch(q);
      activeheight[5] += shrink(q);
      if ((shrinkorder(q) != normal) & (shrink(q) != 0)) {
	printnl(S(292));
	print(S(793));
	help4(S(794),S(795),S(796),S(753));
	error();
	r = newspec(q);
	shrinkorder(r) = normal;
	deleteglueref(q);
	glueptr(p) = r;
	q = r;
      }
    }
    curheight += prevdp + width(q);
    prevdp = 0;   /*:976*/
_Lnotfound:
    if (prevdp > d) {   /*:972*/
      curheight += prevdp - d;
      prevdp = d;
    }
    prevp = p;
    p = link(prevp);
  }
_Ldone:
  return bestplace;
}

