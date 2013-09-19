Static void handlerightbrace(void)
{
  pointer p, q;
  scaled d;
  long f;

  switch (curgroup) {

  case simplegroup:
    unsave();
    break;

  case bottomlevel:
    printnl(S(292));
    print(S(1002));
    help2(S(1003),S(1004));
    error();
    break;

  case semisimplegroup:
  case mathshiftgroup:
  case mathleftgroup:
    extrarightbrace();
    break;

  /*1085:*/
  case hboxgroup:
    package(0);
    break;

  case adjustedhboxgroup:
    adjusttail = adjusthead;
    package(0);
    break;

  case vboxgroup:
    endgraf();
    package(0);
    break;

  case vtopgroup:   /*:1085*/
    endgraf();
    package(vtopcode);
    break;
    /*1100:*/

  case insertgroup:
    endgraf();
    q = splittopskip;
    addglueref(q);
    d = splitmaxdepth;
    f = floatingpenalty;
    unsave();
    saveptr--;
    p = vpack(link(head), 0, additional);
/* p2c: tex1.p: Warning: Too many arguments for foovpack [299] */
    popnest();
    if (saved(0) < 255) {
      tailappend(getnode(insnodesize));
      type(tail) = insnode;
      subtype(tail) = saved(0);
      height(tail) = height(p) + depth(p);
      insptr(tail) = listptr(p);
      splittopptr(tail) = q;
      depth(tail) = d;
      floatcost(tail) = f;
    } else {
      tailappend(getnode(smallnodesize));
      type(tail) = adjustnode;
      subtype(tail) = 0;
      adjustptr(tail) = listptr(p);
      deleteglueref(q);
    }
    freenode(p, boxnodesize);
    if (nestptr == 0)
      buildpage();
    break;

  case outputgroup:   /*1026:*/
    if (loc != 0 || (tokentype != outputtext && tokentype != backedup) )
    {   /*:1027*/
      printnl(S(292));
      print(S(1005));
      help2(S(1006),S(682));
      error();
      do {
	gettoken();
      } while (loc != 0);
    }
    endtokenlist();
    endgraf();
    unsave();
    outputactive = false;
    insertpenalties = 0;   /*1028:*/
    if (box(255) != 0) {   /*:1028*/
      printnl(S(292));
      print(S(1007));
      printesc(S(464));
      printint(255);
      help3(S(1008),S(1009),S(1010));
      boxerror(255);
    }
    if (tail != head) {
      link(pagetail) = link(head);
      pagetail = tail;
    }
    if (link(pagehead) != 0) {
      if (link(contribhead) == 0)
	contribtail = pagetail;
      link(pagetail) = link(contribhead);
      link(contribhead) = link(pagehead);
      link(pagehead) = 0;
      pagetail = pagehead;
    }
    popnest();
    buildpage();
    break;
    /*:1026*/
    /*:1100*/
    /*1118:*/

  case discgroup:   /*:1118*/
    builddiscretionary();
    break;
    /*1132:*/

  case aligngroup:   /*:1132*/
    backinput();
    curtok = cstokenflag + frozencr;
    printnl(S(292));
    print(S(554));
    printesc(S(737));
    print(S(555));
    help1(S(1011));
    inserror();
    break;
    /*1133:*/

  case noaligngroup:   /*:1133*/
    endgraf();
    unsave();
    alignpeek();
    break;
    /*1168:*/

  case vcentergroup:   /*:1168*/
    endgraf();
    unsave();
    saveptr -= 2;
    p = vpack(link(head), saved(1), saved(0));
/* p2c: tex1.p: Warning: Too many arguments for foovpack [299] */
    popnest();
    tailappend(newnoad());
    type(tail) = vcenternoad;
    mathtype(nucleus(tail)) = subbox;
    info(nucleus(tail)) = p;
    break;
    /*1173:*/

  case mathchoicegroup:   /*:1173*/
    buildchoices();
    break;
    /*1186:*/

  case mathgroup:
    unsave();
    saveptr--;
    mathtype(saved(0)) = submlist;
    p = finmlist(0);
    info(saved(0)) = p;
    if (p != 0) {
      if (link(p) == 0) {
	if (type(p) == ordnoad) {
	  if (mathtype(subscr(p)) == empty) {
	    if (mathtype(supscr(p)) == empty) {
	      mem[saved(0) - memmin].hh = mem[nucleus(p) - memmin].hh;
	      freenode(p, noadsize);
	    }
	  }
	} else if (type(p) == accentnoad) {
	  if (saved(0) == nucleus(tail)) {
	    if (type(tail) == ordnoad) {   /*1187:*/
	      q = head;
	      while (link(q) != tail)
		q = link(q);
	      link(q) = p;
	      freenode(tail, noadsize);
	      tail = p;
	    }
	    /*:1187*/
	  }
	}
      }
    }
    break;
    /*:1186*/

  default:
    confusion(S(1012));
    break;
  }

  /*
1027:*/
}
