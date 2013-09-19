Static void promptfilename(strnumber s, strnumber e)
{
  short k;

  if (s == S(665)) {
    printnl(S(292));
    print(S(666));
  } else {
    printnl(S(292));
    print(S(667));
  }
  printfilename(curname, curarea, curext);
  print(S(668));
  if (e == S(669))
    showcontext();
  printnl(S(670));
  print(s);
  if (interaction < scrollmode)
    fatalerror(S(671));
  print(S(488));
  terminput();   /*531:*/
  beginname();
  k = first;
  while (buffer[k] == ' ' && k < last)
    k++;
  while (true) {
    if (k == last)
      goto _Ldone;
    if (!morename(buffer[k]))
      goto _Ldone;
    k++;
  }
_Ldone:
  endname();   /*:531*/
  if (curext == S(385))
    curext = e;
  packfilename(curname,curarea,curext);
}

