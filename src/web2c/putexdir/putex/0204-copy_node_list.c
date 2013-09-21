Static halfword copynodelist(halfword p)
{
  pointer h, q, r=0 /* XXXX */;
  char words;

  h = getavail();
  q = h;
  while (p != 0) {  /*205:*/
    words = 1;
    if (ischarnode(p)) {
      r = getavail();
#ifdef BIG_CHARNODE
	words = charnodesize;
#endif
    } else {  /*206:*/
      switch (type(p)) {   /*:206*/

      case hlistnode:
      case vlistnode:
      case unsetnode:
	r = getnode(boxnodesize);
	mem[r - memmin + 6] = mem[p - memmin + 6];
	mem[r - memmin + 5] = mem[p - memmin + 5];
	listptr(r) = copynodelist(listptr(p));
	words = 5;
	break;

      case rulenode:
	r = getnode(rulenodesize);
	words = rulenodesize;
	break;

      case insnode:
	r = getnode(insnodesize);
	mem[r - memmin + 4] = mem[p - memmin + 4];
	addglueref(splittopptr(p));
	insptr(r) = copynodelist(insptr(p));
	words = insnodesize - 1;
	break;

      case whatsitnode:   /*1357:*/
	switch (subtype(p)) {   /*:1357*/

	case opennode:
	  r = getnode(opennodesize);
	  words = opennodesize;
	  break;

	case writenode:
	case specialnode:
	  r = getnode(writenodesize);
	  addtokenref(writetokens(p));
	  words = writenodesize;
	  break;

	case closenode:
	case languagenode:
	  r = getnode(smallnodesize);
	  words = smallnodesize;
	  break;

	default:
	  confusion(S(429));
	  break;
	}
	break;

      case gluenode:
	r = getnode(smallnodesize);
	addglueref(glueptr(p));
	glueptr(r) = glueptr(p);
	leaderptr(r) = copynodelist(leaderptr(p));
	break;

      case kernnode:
      case mathnode:
      case penaltynode:
	r = getnode(smallnodesize);
	words = smallnodesize;
	break;

      case ligaturenode:
	r = getnode(smallnodesize);
	mem[ligchar(r) - memmin] = mem[ligchar(p) - memmin];
	ligptr(r) = copynodelist(ligptr(p));
	break;

      case discnode:
	r = getnode(smallnodesize);
	prebreak(r) = copynodelist(prebreak(p));
	postbreak(r) = copynodelist(postbreak(p));
	break;

      case marknode:
	r = getnode(smallnodesize);
	addtokenref(markptr(p));
	words = smallnodesize;
	break;

      case adjustnode:
	r = getnode(smallnodesize);
	adjustptr(r) = copynodelist(adjustptr(p));
	break;

      default:
	confusion(S(430));
	break;
      }
    }
    while (words > 0) {   /*:205*/
      words--;
      mem[r + words - memmin] = mem[p + words - memmin];
    }
    link(q) = r;
    q = r;
    p = link(p);
  }
  link(q) = 0;
  q = link(h);
  freeavail(h);
  return q;
}
