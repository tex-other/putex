Static void initcol(void)
{
  extrainfo(curalign) = curcmd;
  if (curcmd == omit)
    alignstate = 0;
  else {
    backinput();
    begintokenlist(upart(curalign), utemplate);
  }
}
