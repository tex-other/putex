Static void initrow(void)
{
  pushnest();
  mode = -hmode - vmode - mode;
  if (mode == -hmode)
    spacefactor = 0;
  else
    prevdepth = 0;
  tailappend(newglue(glueptr(preamble)));
  subtype(tail) = tabskipcode + 1;
  curalign = link(preamble);
  curtail = curhead;
  initspan(curalign);
}
