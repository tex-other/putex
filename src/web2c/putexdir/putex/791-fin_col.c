Static boolean fincol(void)
{
  boolean Result;
  pointer p, q, r, s, u;
  scaled w;
  glueord o;
  halfword n;

  if (curalign == 0)
    confusion(S(735));
  q = link(curalign);
  if (q == 0)
    confusion(S(735));
  if (alignstate < 500000L)
    fatalerror(S(509));
  p = link(q);
  /*792:*/
  if ((p == 0) & (extrainfo(curalign) < crcode)) {
    if (curloop != 0) {   /*793:*/
      link(q) = newnullbox();
      p = link(q);
      info(p) = endspan;
      width(p) = nullflag;
      curloop = link(curloop);   /*794:*/
      q = holdhead;
      r = upart(curloop);
      while (r != 0) {
	link(q) = getavail();
	q = link(q);
	info(q) = info(r);
	r = link(r);
      }
      link(q) = 0;
      upart(p) = link(holdhead);
      q = holdhead;
      r = vpart(curloop);
      while (r != 0) {
	link(q) = getavail();
	q = link(q);
	info(q) = info(r);
	r = link(r);
      }
      link(q) = 0;
      vpart(p) = link(holdhead);   /*:794*/
      curloop = link(curloop);
      link(p) = newglue(glueptr(curloop));
    } else {   /*:792*/
      printnl(S(292));
      print(S(736));
      printesc(S(737));
      help3(S(738),S(739),S(740));
      extrainfo(curalign) = crcode;
      error();
    }
    /*:793*/
  }
  if (extrainfo(curalign) != spancode) {
    unsave();
    newsavelevel(aligngroup);   /*796:*/
    if (mode == -hmode) {
      adjusttail = curtail;
      u = hpack(link(head), 0, additional);
      w = width(u);
      curtail = adjusttail;
      adjusttail = 0;
    } else {
      u = vpackage(link(head), 0, additional, 0);
      w = height(u);
    }
    n = minquarterword;
    if (curspan != curalign) {   /*798:*/
      q = curspan;
      do {
	n++;
	q = link(link(q));
      } while (q != curalign);
      if (n > maxquarterword)
	confusion(S(741));
      q = curspan;
      while (link(info(q)) < n)
	q = info(q);
      if (link(info(q)) > n) {
	s = getnode(spannodesize);
	info(s) = info(q);
	link(s) = n;
	info(q) = s;
	width(s) = w;
      } else if (width(info(q)) < w)
	width(info(q)) = w;
    } else if (w > width(curalign))   /*:798*/
      width(curalign) = w;
    type(u) = unsetnode;
    spancount(u) = n;   /*659:*/
    if (totalstretch[filll - normal] != 0)
      o = filll;
    else if (totalstretch[fill - normal] != 0)
      o = fill;
    else if (totalstretch[fil - normal] != 0)
      o = fil;
    else {
      o = normal;
      /*:
659*/
    }
    glueorder(u) = o;
    gluestretch(u) = totalstretch[o - normal];   /*665:*/
    if (totalshrink[filll - normal] != 0)
      o = filll;
    else if (totalshrink[fill - normal] != 0)
      o = fill;
    else if (totalshrink[fil - normal] != 0)
      o = fil;
    else
      o = normal;   /*:665*/
    gluesign(u) = o;
    glueshrink(u) = totalshrink[o - normal];
    popnest();
    link(tail) = u;
    tail = u;   /*:796*/
    /*795:*/
    tailappend(newglue(glueptr(link(curalign))));
    subtype(tail) = tabskipcode + 1;   /*:795*/
    if (extrainfo(curalign) >= crcode) {
      Result = true;
      goto _Lexit;
    }
    initspan(p);
  }
  alignstate = 1000000L;   /*406:*/
  do {
    getxtoken();   /*:406*/
  } while (curcmd == spacer);
  curalign = p;
  initcol();
  Result = false;
_Lexit:
  return Result;
}
