Static void endgraf(void)
{
  if (mode != hmode)
    return;
  if (head == tail)
    popnest();
  else
    linebreak(widowpenalty);
  normalparagraph();
  errorcount = 0;
}
