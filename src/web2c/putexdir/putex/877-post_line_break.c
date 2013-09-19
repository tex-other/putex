Static void postlinebreak(long finalwidowpenalty)
{   /*878:*/
  pointer q, r, s;
  boolean discbreak, postdiscbreak;
  scaled curwidth, curindent;
  quarterword t;
  long pen;
  halfword curline;

  q = breaknode(bestbet);
  curp = 0;
  do {
    r = q;
    q = prevbreak(q);
    nextbreak(r) = curp;
    curp = r;   /*:878*/
  } while (q != 0);
  curline = prevgraf + 1;
  do {   /*880:*/
    q = curbreak(curp);
    discbreak = false;
    postdiscbreak = false;
    if (q != 0) {
      if (type(q) == gluenode) {
	deleteglueref(glueptr(q));
	glueptr(q) = rightskip;
	subtype(q) = rightskipcode + 1;
	addglueref(rightskip);
	goto _Ldone;
      }
      if (type(q) == discnode) {   /*882:*/
	t = replacecount(q);   /*883:*/
	if (t == 0)
	  r = link(q);
	else {   /*:883*/
	  r = q;
	  while (t > 1) {
	    r = link(r);
	    t--;
	  }
	  s = link(r);
	  r = link(s);
	  link(s) = 0;
	  flushnodelist(link(q));
	  replacecount(q) = 0;
	}
	if (postbreak(q) != 0) {   /*884:*/
	  s = postbreak(q);
	  while (link(s) != 0)
	    s = link(s);
	  link(s) = r;
	  r = postbreak(q);
	  postbreak(q) = 0;
	  postdiscbreak = true;
	}
	/*:884*/
	if (prebreak(q) != 0) {   /*885:*/
	  s = prebreak(q);
	  link(q) = s;
	  while (link(s) != 0)
	    s = link(s);
	  prebreak(q) = 0;
	  q = s;
	}  /*:885*/
	link(q) = r;
	discbreak = true;
      }  /*:882*/
      else if ((type(q) == mathnode) | (type(q) == kernnode))
	width(q) = 0;
    } else {
      q = temphead;
      while (link(q) != 0)
	q = link(q);
    }
    /*886:*/
    r = newparamglue(rightskipcode);
    link(r) = link(q);
    link(q) = r;
    q = r;   /*:886*/
_Ldone:   /*:881*/
    /*887:*/
    r = link(q);
    link(q) = 0;
    q = link(temphead);
    link(temphead) = r;
    if (leftskip != zeroglue) {   /*:887*/
      r = newparamglue(leftskipcode);
      link(r) = q;
      q = r;
    }
    /*889:*/
    if (curline > lastspecialline) {
      curwidth = secondwidth;
      curindent = secondindent;
    } else if (parshapeptr == 0) {
      curwidth = firstwidth;
      curindent = firstindent;
    } else {
      curwidth = mem[parshapeptr + curline * 2 - memmin].sc;
      curindent = mem[parshapeptr + curline * 2 - memmin - 1].sc;
    }
    adjusttail = adjusthead;
    justbox = hpack(q, curwidth, exactly);
    shiftamount(justbox) = curindent;   /*:889*/
    /*888:*/
    appendtovlist(justbox);
    if (adjusthead != adjusttail) {
      link(tail) = link(adjusthead);
      tail = adjusttail;
    }
    adjusttail = 0;   /*:888*/
    /*890:*/
    if (curline + 1 != bestline) {   /*:890*/
      pen = interlinepenalty;
      if (curline == prevgraf + 1)
	pen += clubpenalty;
      if (curline + 2 == bestline)
	pen += finalwidowpenalty;
      if (discbreak)
	pen += brokenpenalty;
      if (pen != 0) {
	r = newpenalty(pen);
	link(tail) = r;
	tail = r;
      }
    }
    /*:880*/
    curline++;
    curp = nextbreak(curp);
    if (curp != 0) {
      if (!postdiscbreak) {   /*879:*/
	r = temphead;
	while (true) {
	  q = link(r);
	  if (q == curbreak(curp))
	    goto _Ldone1;
	  if (ischarnode(q))
	    goto _Ldone1;
	  if (nondiscardable(q)) {
	    goto _Ldone1;
	  }
	  if (type(q) == kernnode) {
	    if (subtype(q) != explicit)
	      goto _Ldone1;
	  }
	  r = q;
	}
_Ldone1:
	if (r != temphead) {
	  link(r) = 0;
	  flushnodelist(link(temphead));
	  link(temphead) = q;
	}
      }
      /*:879*/
    }
  } while (curp != 0);
  /*881:*/
  if ((curline != bestline) | (link(temphead) != 0))
    confusion(S(766));
  prevgraf = bestline - 1;
}
