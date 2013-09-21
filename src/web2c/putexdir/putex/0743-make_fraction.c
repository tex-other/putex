Static void makefraction(halfword q)
{
  pointer p, v, x, y, z;
  scaled delta, delta1, delta2, shiftup, shiftdown, clr;

  if (thickness(q) == defaultcode)   /*744:*/
    thickness(q) = defaultrulethickness;
  x = cleanbox(numerator(q), numstyle(curstyle));
  z = cleanbox(denominator(q), denomstyle(curstyle));
  if (width(x) < width(z))
    x = rebox(x, width(z));
  else
    z = rebox(z, width(x));
  if (curstyle < textstyle) {
    shiftup = num1(cursize);
    shiftdown = denom1(cursize);
  } else {   /*:744*/
    shiftdown = denom2(cursize);
    if (thickness(q) != 0)
      shiftup = num2(cursize);
    else
      shiftup = num3(cursize);
  }
  if (thickness(q) == 0) {   /*745:*/
    if (curstyle < textstyle)
      clr = defaultrulethickness * 7;
    else
      clr = defaultrulethickness * 3;
    delta = half(clr - shiftup + depth(x) + height(z) - shiftdown);
    if (delta > 0) {
      shiftup += delta;
      shiftdown += delta;
    }
  } else  /*746:*/
  {   /*:746*/
    if (curstyle < textstyle)
      clr = thickness(q) * 3;
    else
      clr = thickness(q);
    delta = half(thickness(q));
    delta1 = clr - shiftup + depth(x) + axisheight(cursize) + delta;
    delta2 = clr - axisheight(cursize) + delta + height(z) - shiftdown;
    if (delta1 > 0)
      shiftup += delta1;
    if (delta2 > 0)
      shiftdown += delta2;
  }
  /*:745*/
  /*747:*/
  v = newnullbox();
  type(v) = vlistnode;
  height(v) = shiftup + height(x);
  depth(v) = depth(z) + shiftdown;
  width(v) = width(x);
  if (thickness(q) == 0) {
    p = newkern(shiftup - depth(x) - height(z) + shiftdown);
    link(p) = z;
  } else {
    y = fractionrule(thickness(q));
    p = newkern(axisheight(cursize) - delta - height(z) + shiftdown);
    link(y) = p;
    link(p) = z;
    p = newkern(shiftup - depth(x) - axisheight(cursize) - delta);
    link(p) = y;
  }
  link(x) = p;
  listptr(v) = x;   /*:747*/
  /*748:*/
  if (curstyle < textstyle)
    delta = delim1(cursize);
  else
    delta = delim2(cursize);
  x = vardelimiter(leftdelimiter(q), cursize, delta);
  link(x) = v;
  z = vardelimiter(rightdelimiter(q), cursize, delta);
  link(v) = z;
  newhlist(q) = hpack(x, 0, additional);   /*:748*/
}
