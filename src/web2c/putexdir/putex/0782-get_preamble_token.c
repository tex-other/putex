Static void getpreambletoken(void)
{
_Lrestart:
  gettoken();
  while (curchr == spancode && curcmd == tabmark) {
    gettoken();
    if (curcmd > maxcommand) {
      expand();
      gettoken();
    }
  }
  if (curcmd == endv)
    fatalerror(S(509));
  if (curcmd != assignglue || curchr != gluebase + tabskipcode)
    return;
  scanoptionalequals();
  scanglue(glueval);
  if (globaldefs > 0)
    geqdefine(gluebase + tabskipcode, glueref, curval);
  else
    eqdefine(gluebase + tabskipcode, glueref, curval);
  goto _Lrestart;
}
