Static void alignpeek(void)
{
_Lrestart:
  alignstate = 1000000L;
  /*406:*/
  do {
    getxtoken();
    /*:406*/
  } while (curcmd == spacer);
  if (curcmd == noalign) {
    scanleftbrace();
    newsavelevel(noaligngroup);
    if (mode == -vmode)
      normalparagraph();
    return;
  }
  if (curcmd == rightbrace) {
    finalign();
    return;
  }
  if (curcmd == carret && curchr == crcrcode)
    goto _Lrestart;
  initrow();
  initcol();
}
