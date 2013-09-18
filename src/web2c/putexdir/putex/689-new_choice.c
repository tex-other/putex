Static halfword newchoice(void)
{
  pointer p;

  p = getnode(stylenodesize);
  type(p) = choicenode;
  subtype(p) = 0;
  displaymlist(p) = 0;
  textmlist(p) = 0;
  scriptmlist(p) = 0;
  scriptscriptmlist(p) = 0;
  return p;
}
