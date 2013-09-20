Static void gettoken(void)
{
  nonewcontrolsequence = false;
  getnext();
  nonewcontrolsequence = true;
/*
  pack_tok(curtok,curcs,curcmd,curchr);
*/
  curtok=pack_tok(curcs,curcmd,curchr);
}

