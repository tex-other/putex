Static void makescripts(halfword q, long delta)
{
  pointer p, x, y, z;
  scaled shiftup, shiftdown, clr;
  smallnumber t;

  p = newhlist(q);
  if (ischarnode(p)) {
    shiftup = 0;
    shiftdown = 0;
  } else {
    z = hpack(p, 0, additional);
    if (curstyle < scriptstyle)
      t = scriptsize;
    else
      t = scriptscriptsize;
    shiftup = height(z) - supdrop(t);
    shiftdown = depth(z) + subdrop(t);
    freenode(z, boxnodesize);
  }
  if (mathtype(supscr(q)) == empty) {   /*757:*/
    x = cleanbox(subscr(q), substyle(curstyle));
    width(x) += scriptspace;
    if (shiftdown < sub1(cursize))
      shiftdown = sub1(cursize);
    clr = height(x) - labs(mathxheight(cursize) * 4) / 5;
    if (shiftdown < clr)
      shiftdown = clr;
    shiftamount(x) = shiftdown;
  } else {  /*758:*/
    x = cleanbox(supscr(q), supstyle(curstyle));
    width(x) += scriptspace;
    if (curstyle & 1)
      clr = sup3(cursize);
    else if (curstyle < textstyle)
      clr = sup1(cursize);
    else
      clr = sup2(cursize);
    if (shiftup < clr)
      shiftup = clr;
    clr = depth(x) + labs(mathxheight(cursize)) / 4;
    if (shiftup < clr)   /*:758*/
      shiftup = clr;
    if (mathtype(subscr(q)) == empty)
      shiftamount(x) = -shiftup;
    else {   /*759:*/
      y = cleanbox(subscr(q), substyle(curstyle));
      width(y) += scriptspace;
      if (shiftdown < sub2(cursize))
	shiftdown = sub2(cursize);
      clr = defaultrulethickness * 4 - shiftup + depth(x) +
	    height(y) - shiftdown;
      if (clr > 0) {
	shiftdown += clr;
	clr = labs(mathxheight(cursize) * 4) / 5 - shiftup + depth(x);
	if (clr > 0) {
	  shiftup += clr;
	  shiftdown -= clr;
	}
      }
      shiftamount(x) = delta;
      p = newkern(shiftup - depth(x) - height(y) + shiftdown);
      link(x) = p;
      link(p) = y;
      x = vpack(x, 0, additional);
/* p2c: tex1.p: Warning: Too many arguments for foovpack [299] */
      shiftamount(x) = shiftdown;
    }
  }
  /*:757*/
  if (newhlist(q) == 0) {
    newhlist(q) = x;
    return;
  }
  p = newhlist(q);
  while (link(p) != 0)
    p = link(p);
  link(p) = x;

  /*:759*/
}
