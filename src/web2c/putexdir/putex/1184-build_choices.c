Static void buildchoices(void)
{
  pointer p;

  unsave();
  p = finmlist(0);
  switch (saved(-1)) {

  case 0:
    displaymlist(tail) = p;
    break;

  case 1:
    textmlist(tail) = p;
    break;

  case 2:
    scriptmlist(tail) = p;
    break;

  case 3:
    scriptscriptmlist(tail) = p;
    saveptr--;
    goto _Lexit;
    break;
  }
  (saved(-1))++;
  pushmath(mathchoicegroup);
  scanleftbrace();
_Lexit: ;
}
