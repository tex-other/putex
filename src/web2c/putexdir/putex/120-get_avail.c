Static pointer getavail(void)
{
  pointer p;

  p = avail;
  if (p != 0)
    avail = link(avail);
  else 
  if (memend + charnodesize <= memmax) {
	p = memend+1;
	memend += charnodesize;
  } else {
	p = himemmin-1;
	himemmin -= charnodesize;
    if (himemmin <= lomemmax) {
      runaway();
      overflow(S(317), memmax - memmin + 1);
    }
  }
  set_as_char_node(p);
  link(p) = 0;
  /*_STAT*/
  dynused += charnodesize;
  /*_ENDSTAT*/
  return p;
}
