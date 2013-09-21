#if 1
#if 0
/* Despite comment from Knuth does not work */
Static void xtoken(void);

Static void getxtoken(void)
{
  getnext();
  xtoken();
}
#endif

Static void getxtoken(void)
{
_Lrestart:
  getnext();
  if (curcmd <= maxcommand)
    goto _Ldone;
  if (curcmd >= call) {
    if (curcmd >= endtemplate) {
      curcs = frozenendv;
      curcmd = endv;
      goto _Ldone;
    }
    macrocall();
  } else
    expand();
  goto _Lrestart;
_Ldone:
/*
  pack_tok(curtok,curcs,curcmd,curchr);
*/
  curtok=pack_tok(curcs,curcmd,curchr);
}
#else 

Static void getxtoken(void)
{
  getnext();
  while (curcmd > maxcommand) {
    expand();
    getnext();
  }
  if (curcs == 0)
    curtok = curcmd * dwa_do_8 + curchr;
  else
    curtok = cstokenflag + curcs;
}

#endif
