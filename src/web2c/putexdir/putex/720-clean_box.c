Static void mlisttohlist(void);


Static halfword cleanbox(halfword p, smallnumber s)
{
  pointer q, x, r;
  smallnumber savestyle;

  switch (mathtype(p)) {

  case mathchar:
    curmlist = newnoad();
    mem[nucleus(curmlist) - memmin] = mem[p - memmin];
    break;

  case subbox:
    q = info(p);
    goto _Lfound;
    break;

  case submlist:
    curmlist = info(p);
    break;

  default:
    q = newnullbox();
    goto _Lfound;
    break;
  }
  savestyle = curstyle;
  curstyle = s;
  mlistpenalties = false;
  mlisttohlist();
  q = link(temphead);
  curstyle = savestyle;   /*703:*/
  if (curstyle < scriptstyle)
    cursize = textsize;
  else
    cursize = (curstyle - textstyle) / 2 * 16;
  curmu = xovern(mathquad(cursize), 18);   /*:703*/
_Lfound:
  if (ischarnode(q) || q == 0)   /*721:*/
    x = hpack(q, 0, additional);
  else if ((link(q) == 0) & (type(q) <= vlistnode) &
	   (shiftamount(q) == 0))
    x = q;
  else
    x = hpack(q, 0, additional);
  q = listptr(x);
  if (!ischarnode(q))   /*:721*/
    return x;
  r = link(q);
  if (r == 0)
    return x;
  if (link(r) != 0)
    return x;
  if (ischarnode(r))
    return x;
  if (type(r) == kernnode) {
    freenode(r, smallnodesize);
    link(q) = 0;
  }
  return x;
}
