Static smallnumber makeleftright(halfword q, smallnumber style, long maxd,
				 long maxh)
{
  scaled delta, delta1, delta2;

  if (style < scriptstyle)
    cursize = textsize;
  else
    cursize = (style - textstyle) / 2 * 16;
  delta2 = maxd + axisheight(cursize);
  delta1 = maxh + maxd - delta2;
  if (delta2 > delta1)
    delta1 = delta2;
  delta = delta1 / 500 * delimiterfactor;
  delta2 = delta1 + delta1 - delimitershortfall;
  if (delta < delta2)
    delta = delta2;
  newhlist(q) = vardelimiter(delimiter(q), cursize, delta);
  return (type(q) - leftnoad + opennoad);
}
