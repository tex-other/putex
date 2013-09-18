Static void vlistout(void)
{
  scaled leftedge, topedge, saveh, savev, leaderht, lx, edge;
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
  leftedge = curh;
  curv -= height(thisbox);
  topedge = curv;
  while (p != 0) {   /*630:*/
    if (ischarnode(p))
      confusion(S(685));
    else  /*631:*/
    {   /*:631*/
      switch (type(p)) {

      case hlistnode:
      case vlistnode:   /*632:*/
	if (listptr(p) == 0)
	  curv += height(p) + depth(p);
	else {   /*:632*/
	  curv += height(p);
	  synchv();
	  saveh = dvih;
	  savev = dviv;
	  curh = leftedge + shiftamount(p);
	  tempptr = p;
	  if (type(p) == vlistnode)
	    vlistout();
	  else
	    hlistout();
	  dvih = saveh;
	  dviv = savev;
	  curv = savev + depth(p);
	  curh = leftedge;
	}
	break;

      case rulenode:
	ruleht = height(p);
	ruledp = depth(p);
	rulewd = width(p);
	goto _Lfinrule_;
	break;

      case whatsitnode:   /*1366:*/
	outwhat(p);
	break;
	/*:1366*/

      case gluenode:   /*634:*/
	{
	pointer g = glueptr(p);
	ruleht = width(g);
	if (gsign != normal) {
	  if (gsign == stretching) {
	    if (stretchorder(g) == gorder) {
	      vetglue(glueset(thisbox) * stretch(g));
	      ruleht += (long)floor(gluetemp + 0.5);
	    }
	  } else if (shrinkorder(g) == gorder) {
	    vetglue(glueset(thisbox) * shrink(g));
	    ruleht -= (long)floor(gluetemp + 0.5);
	  }
	}
	}
	if (subtype(p) >= aleaders) {   /*635:*/
	  leaderbox = leaderptr(p);
	  if (type(leaderbox) == rulenode) {
	    rulewd = width(leaderbox);
	    ruledp = 0;
	    goto _Lfinrule_;
	  }
	  leaderht = height(leaderbox) + depth(leaderbox);
	  if (leaderht > 0 && ruleht > 0) {
	    ruleht += 10;
	    edge = curv + ruleht;
	    lx = 0;   /*636:*/
	    if (subtype(p) == aleaders) {
	      savev = curv;
	      curv = topedge + leaderht * ((curv - topedge) / leaderht);
	      if (curv < savev)
		curv += leaderht;
	    } else {   /*:636*/
	      lq = ruleht / leaderht;
	      lr = ruleht % leaderht;
/* p2c: tex1.p: Note: Using % for possibly-negative arguments [317] */
	      if (subtype(p) == cleaders)
		curv += lr / 2;
	      else {
		lx = (lr * 2 + lq + 1) / (lq * 2 + 2);
		curv += (lr - (lq - 1) * lx) / 2;
	      }
	    }
	    while (curv + leaderht <= edge) {   /*637:*/
	      curh = leftedge + shiftamount(leaderbox);
	      synchh();
	      saveh = dvih;
	      curv += height(leaderbox);
	      synchv();
	      savev = dviv;
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
	      curh = leftedge;
	      curv = savev - height(leaderbox) + leaderht + lx;
	    }
	    /*:637*/
	    curv = edge - 10;
	    goto _Lnextp_;
	  }
	}
	/*:635*/
	goto _Lmovepast_;
	break;
	/*:634*/

      case kernnode:
	curv += width(p);
	break;
      }
      goto _Lnextp_;
_Lfinrule_:   /*633:*/
      if (isrunning(rulewd)) {
	rulewd = width(thisbox);
      }
      ruleht += ruledp;
      curv += ruleht;
      if (ruleht > 0 && rulewd > 0) {
	synchh();
	synchv();
	dvi_putrule( ruleht,  rulewd);
      }
      goto _Lnextp_;   /*:633*/
_Lmovepast_:
      curv += ruleht;
    }
_Lnextp_:
    p = link(p);
  }
  /*:630*/
  prunemovements(saveloc);
  if (curs > 0)
    dvipop(saveloc);
  curs--;
}
