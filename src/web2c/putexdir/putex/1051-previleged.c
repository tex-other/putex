Static boolean privileged(void)
{
  if (mode > 0)
    return true;
  else {
    reportillegalcase();
    return false;
  }
}
