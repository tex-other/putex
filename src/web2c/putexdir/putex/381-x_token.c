Static void xtoken(void)
{
  while (curcmd > maxcommand) {
    expand();
    getnext();
  }
/*
  pack_tok(curtok,curcs,curcmd,curchr);
*/
  curtok = pack_tok(curcs,curcmd,curchr);
}

