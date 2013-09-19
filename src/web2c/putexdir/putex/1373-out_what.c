Static void outwhat(halfword p)
{
  /* smallnumber */ int j; /* INT */

  switch (subtype(p)) {

  case opennode:
  case writenode:
  case closenode:   /*1374:*/
    if (!doingleaders) {   /*:1374*/
      j = writestream(p);
      if (subtype(p) == writenode)
	writeout(p);
      else {
	if (writeopen[j])
	  aclose(&writefile[j]);
	if (subtype(p) == closenode)
	  writeopen[j] = false;
	else if (j < 16) {
	  curname = openname(p);
	  curarea = openarea(p);
	  curext = openext(p);
	  if (curext == S(385))
	    curext = S(669);
	packfilename(curname,curarea,curext);
	  while (!aopenout(&writefile[j]))
	    promptfilename(S(683), S(669));
	  writeopen[j] = true;
	}
      }
    }
    break;

  case specialnode:
    specialout(p);
    break;

  case languagenode:
    /* blank case */
    break;

  default:
    confusion(S(684));
    break;
  }
}
