Static void initmath(void)
{
  scaled w, l, s, v, d;
  pointer p, q;
  internalfontnumber f;
  long n;

  gettoken();
  if (curcmd == mathshift && mode > 0) {   /*1145:*/
    if (head == tail) {
      popnest();
      w = -maxdimen;
    } else {
      linebreak(displaywidowpenalty);   /*1146:*/
      v = shiftamount(justbox) + quad(curfont) * 2;
      w = -maxdimen;
      p = listptr(justbox);
      while (p != 0) {  /*1147:*/
_Lreswitch:
	if (ischarnode(p)) {
	  f = font(p);
	  d = charwidth(f, charinfo(f, character(p)));
	  goto _Lfound;
	}
	switch (type(p)) {   /*:1147*/

	case hlistnode:
	case vlistnode:
	case rulenode:
	  d = width(p);
	  goto _Lfound;
	  break;

	case ligaturenode:   /*652:*/
      type(ligtrick) = charnodetype ;
      font(ligtrick) = font_ligchar(p);
      character(ligtrick) = character_ligchar(p);
	  link(ligtrick) = link(p);
	  p = ligtrick;
	  goto _Lreswitch;
	  break;
	  /*:652*/

	case kernnode:
	case mathnode:
	  d = width(p);
	  break;

	case gluenode:   /*1148:*/
	  q = glueptr(p);
	  d = width(q);
	  if (gluesign(justbox) == stretching) {
	    if ((glueorder(justbox) == stretchorder(q)) &
		(stretch(q) != 0))
	      v = maxdimen;
	  } else if (gluesign(justbox) == shrinking) {
	    if ((glueorder(justbox) == shrinkorder(q)) &
		(shrink(q) != 0))
	      v = maxdimen;
	  }
	  if (subtype(p) >= aleaders)
	    goto _Lfound;
	  break;
	  /*:1148*/

	case whatsitnode:   /*1361:*/
	  d = 0;   /*:1361*/
	  break;

	default:
	  d = 0;
	  break;
	}
	if (v < maxdimen)
	  v += d;
	goto _Lnotfound;
_Lfound:
	if (v >= maxdimen) {
	  w = maxdimen;
	  goto _Ldone;
	}
	v += d;
	w = v;
_Lnotfound:
	p = link(p);
      }
_Ldone: ;   /*:1146*/
    }  /*1149:*/
    if (parshapeptr == 0) {
      if (hangindent != 0 &&
	  ( (hangafter >= 0 && prevgraf + 2 > hangafter) ||
	   prevgraf + 1 < -hangafter) ) {
	l = hsize - labs(hangindent);
	if (hangindent > 0)
	  s = hangindent;
	else
	  s = 0;
      } else {
	l = hsize;
	s = 0;
      }
    } else {   /*:1149*/
      n = info(parshapeptr);
      if (prevgraf + 2 >= n)
	p = parshapeptr + n * 2;
      else
	p = parshapeptr + (prevgraf + 2) * 2;
      s = mem[p - memmin - 1].sc;
      l = mem[p - memmin].sc;
    }
    pushmath(mathshiftgroup);
    mode = mmode;
    eqworddefine(intbase + curfamcode, -1);
    eqworddefine(dimenbase + predisplaysizecode, w);
    eqworddefine(dimenbase + displaywidthcode, l);
    eqworddefine(dimenbase + displayindentcode, s);
    if (everydisplay != 0)
      begintokenlist(everydisplay, everydisplaytext);
    if (nestptr == 1)
      buildpage();
    return;
  }
  /*:1145*/
  backinput();   /*1139:*/
  pushmath(mathshiftgroup);
  eqworddefine(intbase + curfamcode, -1);
  if (everymath != 0)   /*:1139*/
    begintokenlist(everymath, everymathtext);
}

