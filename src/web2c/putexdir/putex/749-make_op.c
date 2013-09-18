Static long makeop(halfword q)
{
  scaled delta, shiftup, shiftdown;
  pointer p, v, x, y, z;
  quarterword c;
  fourquarters i;

  if (subtype(q) == normal && curstyle < textstyle)
    subtype(q) = limits;
  if (mathtype(nucleus(q)) == mathchar) {
    fetch(nucleus(q));
    if ((curstyle < textstyle) & (chartag(curi) == listtag)) {
      c = rembyte(curi);
      i = charinfo(curf, c);
      if (charexists(i)) {
	curc = c;
	curi = i;
	character(nucleus(q)) = c;
      }
    }
    delta = charitalic(curf, curi);
    x = cleanbox(nucleus(q), curstyle);
    if ((mathtype(subscr(q)) != empty) & (subtype(q) != limits))
      width(x) -= delta;
    shiftamount(x) =
      half(height(x) - depth(x)) - axisheight(cursize);
    mathtype(nucleus(q)) = subbox;
    info(nucleus(q)) = x;
  } else
    delta = 0;
  if (subtype(q) != limits)   /*750:*/
    return delta;
  /*:750*/
  x = cleanbox(supscr(q), supstyle(curstyle));
  y = cleanbox(nucleus(q), curstyle);
  z = cleanbox(subscr(q), substyle(curstyle));
  v = newnullbox();
  type(v) = vlistnode;
  width(v) = width(y);
  if (width(x) > width(v))
    width(v) = width(x);
  if (width(z) > width(v))
    width(v) = width(z);
  x = rebox(x, width(v));
  y = rebox(y, width(v));
  z = rebox(z, width(v));
  shiftamount(x) = half(delta);
  shiftamount(z) = -shiftamount(x);
  height(v) = height(y);
  depth(v) = depth(y);   /*751:*/
  if (mathtype(supscr(q)) == empty) {
    freenode(x, boxnodesize);
    listptr(v) = y;
  } else {
    shiftup = bigopspacing3 - depth(x);
    if (shiftup < bigopspacing1)
      shiftup = bigopspacing1;
    p = newkern(shiftup);
    link(p) = y;
    link(x) = p;
    p = newkern(bigopspacing5);
    link(p) = x;
    listptr(v) = p;
    height(v) += bigopspacing5 + height(x) + depth(x) + shiftup;
  }
  if (mathtype(subscr(q)) == empty)
    freenode(z, boxnodesize);
  else {   /*:751*/
    shiftdown = bigopspacing4 - height(z);
    if (shiftdown < bigopspacing2)
      shiftdown = bigopspacing2;
    p = newkern(shiftdown);
    link(y) = p;
    link(p) = z;
    p = newkern(bigopspacing5);
    link(z) = p;
    depth(v) += bigopspacing5 + height(z) + depth(z) + shiftdown;
  }
  newhlist(q) = v;
  return delta;
}
