Static void begininsertoradjust(void)
{
  if (curcmd == vadjust)
    curval = 255;
  else {
    scaneightbitint();
    if (curval == 255) {
      printnl(S(292));
      print(S(867));
      printesc(S(374));
      printint(255);
      help1(S(868));
      error();
      curval = 0;
    }
  }
  saved(0) = curval;
  saveptr++;
  newsavelevel(insertgroup);
  scanleftbrace();
  normalparagraph();
  pushnest();
  mode = -vmode;
  prevdepth = ignoredepth;
}
