Static void initspan(halfword p)
{
  pushnest();
  if (mode == -hmode)
    spacefactor = 1000;
  else {
    prevdepth = ignoredepth;
    normalparagraph();
  }
  curspan = p;
}
