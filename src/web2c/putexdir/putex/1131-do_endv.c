Static void doendv(void)
{
  if (curgroup != aligngroup) {
    offsave();
    return;
  }
  endgraf();
  if (fincol())
    finrow();
}
