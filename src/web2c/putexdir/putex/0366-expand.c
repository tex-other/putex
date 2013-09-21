Static void expand(void)
{
  halfword t;
  pointer p, r, backupbackup;
  short j;
  long cvbackup;
  smallnumber cvlbackup, radixbackup, cobackup, savescannerstatus;

  cvbackup = curval;
  cvlbackup = curvallevel;
  radixbackup = radix;
  cobackup = curorder;

  backupbackup = link(backuphead);

  if (curcmd < call) {   /*367:*/
    if (tracingcommands > 1)
      showcurcmdchr();
    switch (curcmd) {

    case topbotmark:   /*386:*/
      if (curmark[curchr - topmarkcode] != 0)
	begintokenlist(curmark[curchr - topmarkcode], marktext);
      break;
      /*:386*/

    case expandafter:   /*368:*/
      gettoken();
      t = curtok;
      gettoken();
      if (curcmd > maxcommand)
	expand();
      else
	backinput();
      curtok = t;
      backinput();
      break;
      /*:368*/

    case noexpand:   /*369:*/
      savescannerstatus = scannerstatus;
      scannerstatus = normal;
      gettoken();
      scannerstatus = savescannerstatus;
      t = curtok;
      backinput();
      if (t >= cstokenflag) {
	p = getavail();
	info(p) = cstokenflag + frozendontexpand;
	link(p) = loc;
	start = p;
	loc = p;
      }
      break;
      /*:369*/

    case csname:   /*372:*/
      r = getavail();
      p = r;
      do {
	getxtoken();
	if (curcs == 0) {
	  storenewtoken(p,curtok);
	}
      } while (curcs == 0);
      if (curcmd != endcsname) {   /*373:*/
	printnl(S(292));
	print(S(554));
	printesc(S(263));
	print(S(555));
	help2(S(556),S(557));
	backerror();
      }
      /*:373*/
      /*374:*/
      j = first;
      p = link(r);
      while (p != 0) {
	if (j >= maxbufstack) {
	  maxbufstack = j + 1;
	  if (maxbufstack == bufsize)
	    overflow(S(511), bufsize);
	}
	buffer[j] = (info(p)) & (dwa_do_8-1);
	j++;
	p = link(p);
      }
      if (j > first + 1) {
/*	nonewcontrolsequence = false; 
*/
	curcs = idlookup_p(buffer+first, j - first,false);
/*
	nonewcontrolsequence = true;
*/
      } else if (j == first)
	curcs = nullcs;
      else
	curcs = singlebase + buffer[first];   /*:374*/
      flushlist(r);
      if (eqtype(curcs) == undefinedcs)
	eqdefine(curcs, relax, 256);
      curtok = curcs + cstokenflag;
      backinput();
      break;
      /*:372*/

    case convert:
      convtoks();
      break;

    case the:
      insthetoks();
      break;

    case iftest:
      conditional();
      break;

    case fiorelse:   /*510:*/
      if (curchr > iflimit) {
	if (iflimit == ifcode)
	  insertrelax();
	else {
	  printnl(S(292));
	  print(S(558));
	  printcmdchr(fiorelse, curchr);
	  help1(S(559));
	  error();
	}
      } else {   /*:510*/
	while (curchr != ficode)   /*496:*/
	  passtext();
	p = condptr;
	ifline = iflinefield(p);
	curif = subtype(p);
	iflimit = type(p);
	condptr = link(p);
	freenode(p, ifnodesize);   /*:496*/
      }
      break;

    case input:   /*378:*/
      if (curchr > 0)
	forceeof = true;
      else if (nameinprogress)
	insertrelax();
      else
	startinput();
      break;
      /*370:*/

    default:
      printnl(S(292));
      print(S(560));
      help5(S(561),S(562),S(563),S(564),S(565));
      error();   /*:370*/
      break;
    }
  }  /*:367*/
  else if (curcmd < endtemplate)
    macrocall();
  else {
    curtok = cstokenflag + frozenendv;
    backinput();
  }
  curval = cvbackup;
  curvallevel = cvlbackup;
  radix = radixbackup;
  curorder = cobackup;

  link(backuphead) = backupbackup;

  /*:378*/
  /*375:*/
  /*:375*/
}

