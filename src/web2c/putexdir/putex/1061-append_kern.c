Static void appendkern(void)
{
  quarterword s;

  s = curchr;
  scandimen(s == muglue, false, false);
  tailappend(newkern(curval));
  subtype(tail) = s;
}
