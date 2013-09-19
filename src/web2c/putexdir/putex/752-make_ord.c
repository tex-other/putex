Static void makeord(halfword q)
{
  long a;
  pointer p, r;

_Lrestart:
  if (mathtype(subscr(q)) == empty) {
    if (mathtype(supscr(q)) == empty) {
      if (mathtype(nucleus(q)) == mathchar) {
	p = link(q);
	if (p != 0) {
	  if ((type(p) >= ordnoad) & (type(p) <= punctnoad)) {
	    if (mathtype(nucleus(p)) == mathchar) {
	      if (fam(nucleus(p)) == fam(nucleus(q))) {
		mathtype(nucleus(q)) = mathtextchar;
		fetch(nucleus(q));
		if (chartag(curi) == ligtag) {
		  a = ligkernstart(curf,curi); 
		  curc = character(nucleus(p));
		  curi = fontinfo[a].qqqq;
		  if (skipbyte(curi) > stopflag) {
		    a = ligkernrestart(curf,curi); 
		    curi = fontinfo[a].qqqq;
		  }
		  while (true) {  /*753:*/
		    if (nextchar(curi) == curc) {
		      if (skipbyte(curi) <= stopflag) {
			if (opbyte(curi) >= kernflag) {
			  p = newkern(charkern(curf, curi));
			  link(p) = link(q);
			  link(q) = p;
			  goto _Lexit;
			} else {   /*:753*/
			  checkinterrupt();
			  switch (opbyte(curi)) {

			  case minquarterword + 1:
			  case minquarterword + 5:
			    character(nucleus(q)) = rembyte(curi);
			    break;

			  case minquarterword + 2:
			  case minquarterword + 6:
			    character(nucleus(p)) = rembyte(curi);
			    break;

			  case minquarterword + 3:
			  case minquarterword + 7:
			  case minquarterword + 11:
			    r = newnoad();
			    character(nucleus(r)) = rembyte(curi);
			    fam(nucleus(r)) = fam(nucleus(q));
			    link(q) = r;
			    link(r) = p;
			    if (opbyte(curi) < minquarterword + 11)
			      mathtype(nucleus(r)) = mathchar;
			    else
			      mathtype(nucleus(r)) = mathtextchar;
			    break;

			  default:
			    link(q) = link(p);
			    character(nucleus(q)) = rembyte(curi);
			    mem[subscr(q) - memmin] =
			      mem[subscr(p) - memmin];
			    mem[supscr(q) - memmin] =
			      mem[supscr(p) - memmin];
			    freenode(p, noadsize);
			    break;
			  }
			  if (opbyte(curi) > minquarterword + 3)
			    goto _Lexit;
			  mathtype(nucleus(q)) = mathchar;
			  goto _Lrestart;
			}
		      }
		    }
		    if (skipbyte(curi) >= stopflag)
		      goto _Lexit;
		    a += skipbyte(curi) - minquarterword + 1;
		    curi = fontinfo[a].qqqq;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
_Lexit: ;
}
