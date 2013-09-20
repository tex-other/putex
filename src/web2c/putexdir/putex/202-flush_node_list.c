Static void flushnodelist(halfword p)
{
  pointer q;

  while (p != 0) {
    q = link(p);
    if (ischarnode(p)) {
      freeavail(p);
    } else {
      switch (type(p)) {

      case hlistnode:
      case vlistnode:
      case unsetnode:
	flushnodelist(listptr(p));
	freenode(p, boxnodesize);
	goto _Ldone;
	break;

      case rulenode:
	freenode(p, rulenodesize);
	goto _Ldone;
	break;

      case insnode:
	flushnodelist(insptr(p));
	deleteglueref(splittopptr(p));
	freenode(p, insnodesize);
	goto _Ldone;
	break;

      case whatsitnode:   /*1358:*/
	switch (subtype(p)) {

	case opennode:
	  freenode(p, opennodesize);
	  break;

	case writenode:
	case specialnode:
	  deletetokenref(writetokens(p));
	  freenode(p, writenodesize);
	  goto _Ldone;
	  break;

	case closenode:
	case languagenode:
	  freenode(p, smallnodesize);
	  break;

	default:
	  confusion(S(427));
	  break;
	}
	goto _Ldone;
	break;
	/*:1358*/

      case gluenode:
	karmafastdeleteglueref(glueptr(p));
	if (leaderptr(p) != 0)
	  flushnodelist(leaderptr(p));
	break;

      case kernnode:
      case mathnode:
      case penaltynode:
	/* blank case */
	break;

      case ligaturenode:
	flushnodelist(ligptr(p));
	break;

      case marknode:
	deletetokenref(markptr(p));
	break;

      case discnode:
	flushnodelist(prebreak(p));
	flushnodelist(postbreak(p));
	break;

      case adjustnode:   /*698:*/
	flushnodelist(adjustptr(p));
	break;

      case stylenode:
	freenode(p, stylenodesize);
	goto _Ldone;
	break;

      case choicenode:
	flushnodelist(displaymlist(p));
	flushnodelist(textmlist(p));
	flushnodelist(scriptmlist(p));
	flushnodelist(scriptscriptmlist(p));
	freenode(p, stylenodesize);
	goto _Ldone;
	break;

      case ordnoad:
      case opnoad:
      case binnoad:
      case relnoad:
      case opennoad:
      case closenoad:
      case punctnoad:
      case innernoad:
      case radicalnoad:
      case overnoad:
      case undernoad:
      case vcenternoad:
      case accentnoad:
	if (mathtype(nucleus(p)) >= subbox)
	  flushnodelist(info(nucleus(p)));
	if (mathtype(supscr(p)) >= subbox)
	  flushnodelist(info(supscr(p)));
	if (mathtype(subscr(p)) >= subbox)
	  flushnodelist(info(subscr(p)));
	if (type(p) == radicalnoad)
	  freenode(p, radicalnoadsize);
	else if (type(p) == accentnoad)
	  freenode(p, accentnoadsize);
	else
	  freenode(p, noadsize);
	goto _Ldone;
	break;

      case leftnoad:
      case rightnoad:
	freenode(p, noadsize);
	goto _Ldone;
	break;

      case fractionnoad:
	flushnodelist(info(numerator(p)));
	flushnodelist(info(denominator(p)));
	freenode(p, fractionnoadsize);
	goto _Ldone;
	break;
	/*:698*/

      default:
	confusion(S(428));
	break;
      }
      freenode(p, smallnodesize);
_Ldone: ;
    }
    p = q;
  }
}
