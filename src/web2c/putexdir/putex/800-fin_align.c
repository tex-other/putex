Static void finalign(void)
{
  pointer p, q, r, s, u, v;
  scaled t, w, o, rulesave;
  halfword n;
  memoryword auxsave;

  if (curgroup != aligngroup)
    confusion(S(742));
  unsave();
  if (curgroup != aligngroup)
    confusion(S(743));
  unsave();
  if (nest[nestptr - 1].modefield == mmode)
    o = displayindent;
  else
    o = 0;
  /*801:*/
  q = link(preamble);
  do {   /*804:*/
    flushlist(upart(q));
    flushlist(vpart(q));
    p = link(link(q));
    if (width(q) == nullflag) {   /*802:*/
      width(q) = 0;
      r = link(q);
      s = glueptr(r);
      if (s != zeroglue) {
	addglueref(zeroglue);
	deleteglueref(s);
	glueptr(r) = zeroglue;
      }
    }
    /*:802*/
    if (info(q) != endspan) {   /*803:*/
      t = width(q) + width(glueptr(link(q)));
      r = info(q);
      s = endspan;
      info(s) = p;
      n = minquarterword + 1;
      do {
	width(r) -= t;
	u = info(r);
	while (link(r) > n) {
	  s = info(s);
	  n = link(info(s)) + 1;
	}
	if (link(r) < n) {
	  info(r) = info(s);
	  info(s) = r;
	  (link(r))--;
	  s = r;
	} else {
	  if (width(r) > width(info(s)))
	    width(info(s)) = width(r);
	  freenode(r, spannodesize);
	}
	r = u;
      } while (r != endspan);
    }
    /*:803*/
    type(q) = unsetnode;
    spancount(q) = minquarterword;
    height(q) = 0;
    depth(q) = 0;
    glueorder(q) = normal;
    gluesign(q) = normal;
    gluestretch(q) = 0;
    glueshrink(q) = 0;
    q = p;   /*:801*/
  } while (q != 0);
  saveptr -= 2;
  packbeginline = -modeline;
  if (mode == -vmode) {
    rulesave = overfullrule;
    overfullrule = 0;
    p = hpack(preamble, saved(1), saved(0));
    overfullrule = rulesave;
  } else {
    q = link(preamble);
    do {
      height(q) = width(q);
      width(q) = 0;
      q = link(link(q));
    } while (q != 0);
    p = vpack(preamble, saved(1), saved(0));
    q = link(preamble);
    do {
      width(q) = height(q);
      height(q) = 0;
      q = link(link(q));
    } while (q != 0);
  }
  packbeginline = 0;   /*:804*/
  /*805:*/
  q = link(head);
  s = head;
  while (q != 0) {   /*:805*/
    if (!ischarnode(q)) {
      if (type(q) == unsetnode) {   /*807:*/
	if (mode == -vmode) {
	  type(q) = hlistnode;
	  width(q) = width(p);
	} else {
	  type(q) = vlistnode;
	  height(q) = height(p);
	}
	glueorder(q) = glueorder(p);
	gluesign(q) = gluesign(p);
	glueset(q) = glueset(p);
	shiftamount(q) = o;
	r = link(listptr(q));
	s = link(listptr(p));
	do {   /*808:*/
	  n = spancount(r);
	  t = width(s);
	  w = t;
	  u = holdhead;
	  while (n > minquarterword) {
	    n--;
	    /*809:*/
	    s = link(s);
	    v = glueptr(s);
	    link(u) = newglue(v);
	    u = link(u);
	    subtype(u) = tabskipcode + 1;
	    t += width(v);
	    if (gluesign(p) == stretching) {
	      if (stretchorder(v) == glueorder(p))
		t += round(((double)glueset(p)) * stretch(v));
	    } else if (gluesign(p) == shrinking) {
	      if (shrinkorder(v) == glueorder(p))
		t -= round(((double)glueset(p)) * shrink(v));
	    }
	    s = link(s);
	    link(u) = newnullbox();
	    u = link(u);
	    t += width(s);
	    if (mode == -vmode)
	      width(u) = width(s);
	    else {   /*:809*/
	      type(u) = vlistnode;
	      height(u) = width(s);
	    }
	  }
	  if (mode == -vmode) {   /*810:*/
	    height(r) = height(q);
	    depth(r) = depth(q);
	    if (t == width(r)) {
	      gluesign(r) = normal;
	      glueorder(r) = normal;
	      glueset(r) = 0.0;
	    } else if (t > width(r)) {
	      gluesign(r) = stretching;
	      if (gluestretch(r) == 0)
		glueset(r) = 0.0;
	      else
		glueset(r) = (double)(t - width(r)) / gluestretch(r);
	    } else {
	      glueorder(r) = gluesign(r);
	      gluesign(r) = shrinking;
	      if (glueshrink(r) == 0)
		glueset(r) = 0.0;
	      else if ((glueorder(r) == normal) &
		       (width(r) - t > glueshrink(r)))
		glueset(r) = 1.0;
	      else
		glueset(r) = (double)(width(r) - t) / glueshrink(r);
	    }
	    width(r) = w;
	    type(r) = hlistnode;
	  } else  /*811:*/
	  {   /*:811*/
	    width(r) = width(q);
	    if (t == height(r)) {
	      gluesign(r) = normal;
	      glueorder(r) = normal;
	      glueset(r) = 0.0;
	    } else if (t > height(r)) {
	      gluesign(r) = stretching;
	      if (gluestretch(r) == 0)
		glueset(r) = 0.0;
	      else
		glueset(r) = (double)(t - height(r)) / gluestretch(r);
	    } else {
	      glueorder(r) = gluesign(r);
	      gluesign(r) = shrinking;
	      if (glueshrink(r) == 0)
		glueset(r) = 0.0;
	      else if ((glueorder(r) == normal) &
		       (height(r) - t > glueshrink(r)))
		glueset(r) = 1.0;
	      else
		glueset(r) = (double)(height(r) - t) / glueshrink(r);
	    }
	    height(r) = w;
	    type(r) = vlistnode;
	  }
	  /*:810*/
	  shiftamount(r) = 0;
	  if (u != holdhead) {   /*:808*/
	    link(u) = link(r);
	    link(r) = link(holdhead);
	    r = u;
	  }
	  r = link(link(r));
	  s = link(link(s));
	} while (r != 0);
      }  /*:807*/
      else if (type(q) == rulenode) {
	if (isrunning(width(q))) {
	  width(q) = width(p);
	}
	if (isrunning(height(q))) {
	  height(q) = height(p);
	}
	if (isrunning(depth(q))) {
	  depth(q) = depth(p);
	}
	if (o != 0) {
	  r = link(q);
	  link(q) = 0;
	  q = hpack(q, 0, additional);
	  shiftamount(q) = o;
	  link(q) = r;
	  link(s) = q;
	}
      }
    }
    s = q;
    q = link(q);
  }
  flushnodelist(p);
  popalignment();   /*812:*/
  memcpy(&auxsave, &aux, sizeof(memoryword));
/* p2c: tex1.p: Warning: Incompatible types or sizes [167] */
  p = link(head);
  q = tail;
  popnest();
  if (mode == mmode) {   /*1206:*/
    doassignments();
    if (curcmd != mathshift) {   /*1207:*/
      printnl(S(292));
      print(S(744));
      help2(S(726),S(727));
      backerror();
    } else {   /*1197:*/
      getxtoken();
      if (curcmd != mathshift) {
	printnl(S(292));
	print(S(745));
	help2(S(746),S(747));
	backerror();
      }
    }
    /*:1207*/
    popnest();
    tailappend(newpenalty(predisplaypenalty));
    tailappend(newparamglue(abovedisplayskipcode));
    link(tail) = p;
    if (p != 0)
      tail = q;
    tailappend(newpenalty(postdisplaypenalty));
    tailappend(newparamglue(belowdisplayskipcode));
    prevdepth = auxsave.sc;
    resumeafterdisplay();
    return;
  }
  /*:1206*/
  aux = auxsave;
  link(tail) = p;
  if (p != 0)
    tail = q;
  if (mode == vmode)
    buildpage();

  /*806:*/
  /*:806*/
  /*:1197*/
  /*:812*/
}
