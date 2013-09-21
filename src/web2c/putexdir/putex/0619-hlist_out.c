Static void hlistout(void)
{
  scaled baseline, leftedge, saveh, savev, leaderwd, lx, edge;
  pointer thisbox, p, leaderbox;
  glueord gorder;
  char gsign;
  long saveloc;
  boolean outerdoingleaders;
  double gluetemp;

  thisbox = tempptr;
  gorder = glueorder(thisbox);
  gsign = gluesign(thisbox);
  p = listptr(thisbox);
  curs++;
  if (curs > 0) {
    dvi_push();
  }
  if (curs > maxpush)
    maxpush = curs;
  saveloc = get_dvi_mark();
  baseline = curv;
  leftedge = curh;
  while (p != 0) {   /*620:*/
_Lreswitch:
    if (ischarnode(p)) {
      synchh();
      synchv();
      do {
	quarterword c;
	quarterword f = font(p);
	c = character(p);
	if (f != dvif) {   /*621:*/
	  dvi_set_font(f);
	  dvif = f;
	}
	/*:621*/
	dvi_set_char(c);
	curh += charwidth(f, charinfo(f, c));
	p = link(p);
      } while (ischarnode(p));
      dvih = curh;
      continue;
    }
    switch (type(p)) {

    case hlistnode:
    case vlistnode:   /*623:*/
      if (listptr(p) == 0)
	curh += width(p);
      else {   /*:623*/
	saveh = dvih;
	savev = dviv;
	curv = baseline + shiftamount(p);
	tempptr = p;
	edge = curh;
	if (type(p) == vlistnode)
	  vlistout();
	else
	  hlistout();
	dvih = saveh;
	dviv = savev;
	curh = edge + width(p);
	curv = baseline;
      }
      break;

    case rulenode:
      ruleht = height(p);
      ruledp = depth(p);
      rulewd = width(p);
      goto _Lfinrule_;
      break;

    case whatsitnode:   /*1367:*/
      outwhat(p);
      break;
      /*:1367*/

    case gluenode:   /*625:*/
      {pointer g = glueptr(p);
      rulewd = width(g);
      if (gsign != normal) {
	if (gsign == stretching) {
	  if (stretchorder(g) == gorder) {
	    vetglue(glueset(thisbox) * stretch(g));
	    rulewd += (long)floor(gluetemp + 0.5);
	  }
	} else if (shrinkorder(g) == gorder) {
	  vetglue(glueset(thisbox) * shrink(g));
	  rulewd -= (long)floor(gluetemp + 0.5);
	}
      }
      }
      if (subtype(p) >= aleaders) {   /*626:*/
	leaderbox = leaderptr(p);
	if (type(leaderbox) == rulenode) {
	  ruleht = height(leaderbox);
	  ruledp = depth(leaderbox);
	  goto _Lfinrule_;
	}
	leaderwd = width(leaderbox);
	if (leaderwd > 0 && rulewd > 0) {
	  rulewd += 10;
	  edge = curh + rulewd;
	  lx = 0;   /*627:*/
	  if (subtype(p) == aleaders) {
	    saveh = curh;
	    curh = leftedge + leaderwd * ((curh - leftedge) / leaderwd);
	    if (curh < saveh)
	      curh += leaderwd;
	  } else {   /*:627*/
	    lq = rulewd / leaderwd;
	    lr = rulewd % leaderwd;
/* p2c: tex1.p: Note: Using % for possibly-negative arguments [317] */
	    if (subtype(p) == cleaders)
	      curh += lr / 2;
	    else {
	      lx = (lr * 2 + lq + 1) / (lq * 2 + 2);
	      curh += (lr - (lq - 1) * lx) / 2;
	    }
	  }
	  while (curh + leaderwd <= edge) {   /*628:*/
	    curv = baseline + shiftamount(leaderbox);
	    synchv();
	    savev = dviv;
	    synchh();
	    saveh = dvih;
	    tempptr = leaderbox;
	    outerdoingleaders = doingleaders;
	    doingleaders = true;
	    if (type(leaderbox) == vlistnode)
	      vlistout();
	    else
	      hlistout();
	    doingleaders = outerdoingleaders;
	    dviv = savev;
	    dvih = saveh;
	    curv = baseline;
	    curh = saveh + leaderwd + lx;
	  }
	  /*:628*/
	  curh = edge - 10;
	  goto _Lnextp_;
	}
      }  /*:626*/
      goto _Lmovepast_;
      break;
      /*:625*/

    case kernnode:
    case mathnode:
      curh += width(p);
      break;

    case ligaturenode:   /*652:*/
	type(ligtrick) = charnodetype;
      font(ligtrick) = font_ligchar(p);
      character(ligtrick) = character_ligchar(p);
      link(ligtrick) = link(p);
      p = ligtrick;
      goto _Lreswitch;
      break;
      /*:652*/
    }
    goto _Lnextp_;
_Lfinrule_:   /*624:*/
    if (isrunning(ruleht)) {
      ruleht = height(thisbox);
    }
    if (isrunning(ruledp)) {
      ruledp = depth(thisbox);
    }
    ruleht += ruledp;
    if (ruleht > 0 && rulewd > 0) {   /*:624*/
      synchh();
      curv = baseline + ruledp;
      synchv();
      dvi_setrule(ruleht,rulewd);
      curv = baseline;
      dvih += rulewd;
    }
_Lmovepast_:
    curh += rulewd;
_Lnextp_:
    p = link(p);
  }
  prunemovements(saveloc);
  if (curs > 0)
    dvipop(saveloc);
  curs--;

  /*622:*/
  /*:622*/
  /*:620*/
}
