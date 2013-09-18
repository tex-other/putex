Static void subsup(void)
{
  smallnumber t;
  pointer p;

  t = empty;
  p = 0;
  if (tail != head) {
    if (scriptsallowed(tail)) {
	if(curcmd == supmark) {
		p = supscr(tail);
	} else { 
		p = subscr(tail);
	}
      t = mathtype(p);
    }
  }
  if (p == 0 || t != empty) {   /*1177:*/
    tailappend(newnoad());
    if(curcmd == supmark) { 
	p = supscr(tail);
    } else { 
	p = subscr(tail);
    }
    if (t != empty) {
      if (curcmd == supmark) {
	printnl(S(292));
	print(S(917));
	help1(S(918));
      } else {
	printnl(S(292));
	print(S(919));
	help1(S(920));
      }
      error();
    }
  }
  /*:1177*/
  scanmath(p);
}
