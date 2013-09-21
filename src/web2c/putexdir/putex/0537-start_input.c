Static void startinput(void)
{
  scanfilename();
  if (curext == S(385))
    curext = S(669);
  packfilename(curname,curarea,curext);
  while (true) {
    beginfilereading();
    if (aopenin(&curfile))
      goto _Ldone;
    if (curarea == S(385)) {
      packfilename(curname, S(677), curext);
      if (aopenin(&curfile))
	goto _Ldone;
    }
    endfilereading();
    promptfilename(S(665), S(669));
  }
_Ldone:
  name = amakenamestring(&curfile);
  if (jobname == 0) {
    jobname = curname;
    openlogfile();
  }
  if (termoffset + flength(name) > maxprintline - 2) {
    println();
  } else if (termoffset > 0 || fileoffset > 0)
    printchar(' ');
  printchar('(');
  openparens++;
  slowprint(name);
  fflush(termout);
  state = newline;
#if 0
  if (name == strptr - 1) {   /*538:*/
    flushstring();
    name = curname;
  }
#else 
   name = curname;
#endif
  line = 1;
  inputln(&curfile, false);
  firmuptheline();
  if (endlinecharinactive) {
    limit--;
  } else
    buffer[limit] = endlinechar;
  first = limit + 1;
  loc = start;   /*:538*/
}
