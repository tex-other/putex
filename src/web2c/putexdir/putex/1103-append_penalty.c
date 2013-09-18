Static void appendpenalty(void)
{
  scanint();
  tailappend(newpenalty(curval));
  if (mode == vmode)
    buildpage();
}
