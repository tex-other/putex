Static void fixlanguage(void)
{
  ASCIIcode l;

  if (language <= 0)
    l = 0;
  else if (language > 255)
    l = 0;
  else
    l = language;
  if (l == clang)
    return;
  newwhatsit(languagenode, smallnodesize);
  whatlang(tail) = l;
  clang = l;
  whatlhm(tail) = normmin(lefthyphenmin);
  whatrhm(tail) = normmin(righthyphenmin);
}
