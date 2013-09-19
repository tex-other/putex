Static void aftermath(void)
{
  boolean l, danger;
  long m;
  pointer p, a;
  /*1198:*/
  pointer b, r, t;   /*:1198*/
  scaled w, z, e, q, d, s;
  smallnumber g1, g2;

  danger = false;   /*1195:*/
  if ((fontparams[famfnt(textsize + 2) ] < totalmathsyparams) |
      (fontparams[famfnt(scriptsize + 2) ] < totalmathsyparams) |
      (fontparams[famfnt(scriptscriptsize + 2) ] <
       totalmathsyparams)) {
    printnl(S(292));
    print(S(926));
    help3(S(927),S(928),S(929));
    error();
    flushmath();
    danger = true;
  } else if ((fontparams[famfnt(textsize + 3) ] <
	      totalmathexparams) |
	     (fontparams[famfnt(scriptsize + 3) ] <
	      totalmathexparams) |
	     (fontparams[famfnt(scriptscriptsize + 3) ] <
	      totalmathexparams)) {
    printnl(S(292));
    print(S(930));
    help3(S(931),S(932),S(933));
    error();
    flushmath();
    danger = true;
  }
  m = mode;
  l = false;
  p = finmlist(0);
  if (mode == -m) {   /*1197:*/
    getxtoken();
    if (curcmd != mathshift) {   /*:1197*/
      printnl(S(292));
      print(S(745));
      help2(S(746),S(747));
      backerror();
    }
    curmlist = p;
    curstyle = textstyle;
    mlistpenalties = false;
    mlisttohlist();
    a = hpack(link(temphead), 0, additional);
    unsave();
    saveptr--;
    if (saved(0) == 1)
      l = true;
    danger = false;   /*1195:*/
    if ((fontparams[famfnt(textsize + 2) ] < totalmathsyparams) |
	(fontparams[famfnt(scriptsize + 2) ] < totalmathsyparams) |
	(fontparams[famfnt(scriptscriptsize + 2) ] <
	 totalmathsyparams)) {
      printnl(S(292));
      print(S(926));
      help3(S(927),S(928),S(929));
      error();
      flushmath();
      danger = true;
    } else if ((fontparams[famfnt(textsize + 3) ] <
		totalmathexparams) |
	       (fontparams[famfnt(scriptsize + 3) ] <
		totalmathexparams) |
	       (fontparams[famfnt(scriptscriptsize + 3) ] <
		totalmathexparams)) {
      printnl(S(292));
      print(S(930));
      help3(S(931),S(932),S(933));
      error();
      flushmath();
      danger = true;
    }
    m = mode;
    p = finmlist(0);
  } else
    a = 0;
  if (m < 0) {   /*1196:*/
    tailappend(newmath(mathsurround, before));
    curmlist = p;
    curstyle = textstyle;
    mlistpenalties = (mode > 0);
    mlisttohlist();
    link(tail) = link(temphead);
    while (link(tail) != 0)
      tail = link(tail);
    tailappend(newmath(mathsurround, after));
    spacefactor = 1000;
    unsave();
    return;
  }
  /*:1196*/
  if (a == 0) {   /*1197:*/
    getxtoken();
    if (curcmd != mathshift) {
      printnl(S(292));
      print(S(745));
      help2(S(746),S(747));
      backerror();
    }
  }
  /*:1197*/
  /*1199:*/
  curmlist = p;
  curstyle = displaystyle;
  mlistpenalties = false;
  mlisttohlist();
  p = link(temphead);
  adjusttail = adjusthead;
  b = hpack(p, 0, additional);
  p = listptr(b);
  t = adjusttail;
  adjusttail = 0;
  w = width(b);
  z = displaywidth;
  s = displayindent;
  if (a == 0 || danger) {
    e = 0;
    q = 0;
  } else {
    e = width(a);
    q = e + mathquad(textsize);
  }
  if (w + q > z) {   /*1201:*/
    if (e != 0 && (w - totalshrink[0] + q <= z ||
		   totalshrink[fil - normal] != 0 ||
		   totalshrink[fill - normal] != 0 ||
		   totalshrink[filll - normal] != 0)) {
      freenode(b, boxnodesize);
      b = hpack(p, z - q, exactly);
    } else {
      e = 0;
      if (w > z) {
	freenode(b, boxnodesize);
	b = hpack(p, z, exactly);
      }
    }
    w = width(b);
  }
  /*:1201*/
  /*1202:*/
  d = half(z - w);
  if (e > 0 && d < e * 2) {   /*:1202*/
    d = half(z - w - e);
    if (p != 0) {
      if (!ischarnode(p)) {
	if (type(p) == gluenode)
	  d = 0;
      }
    }
  }
  /*1203:*/
  tailappend(newpenalty(predisplaypenalty));
  if (d + s <= predisplaysize || l) {
    g1 = abovedisplayskipcode;
    g2 = belowdisplayskipcode;
  } else {
    g1 = abovedisplayshortskipcode;
    g2 = belowdisplayshortskipcode;
  }
  if (l && e == 0) {   /*1204:*/
    shiftamount(a) = s;
    appendtovlist(a);
    tailappend(newpenalty(infpenalty));
  } else {
    tailappend(newparamglue(g1));   /*:1203*/
  }
  if (e != 0) {
    r = newkern(z - w - e - d);
    if (l) {
      link(a) = r;
      link(r) = b;
      b = a;
      d = 0;
    } else {
      link(b) = r;
      link(r) = a;
    }
    b = hpack(b, 0, additional);
  }
  shiftamount(b) = s + d;   /*:1204*/
  appendtovlist(b);   /*1205:*/
  if (a != 0 && e == 0 && !l) {
    tailappend(newpenalty(infpenalty));
    shiftamount(a) = s + z - width(a);
    appendtovlist(a);
    g2 = 0;
  }
  if (t != adjusthead) {
    link(tail) = link(adjusthead);
    tail = t;
  }
  tailappend(newpenalty(postdisplaypenalty));
  if (g2 > 0) {   /*:1199*/
    tailappend(newparamglue(g2));   /*:1205*/
  }
  resumeafterdisplay();

  /*:1195*/
  /*:1195*/
}

