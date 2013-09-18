Static void scanmath(halfword p)
{
  long c;

_Lrestart:   /*404:*/
  do {
    getxtoken();   /*:404*/
  } while (curcmd == spacer || curcmd == relax);
_Lreswitch:
  switch (curcmd) {

  case letter:
  case otherchar:
  case chargiven:
    c = mathcode(curchr);
    if (c == 32768L) {   /*1152:*/
      curcs = curchr + activebase;
      curcmd = eqtype(curcs);
      curchr = equiv(curcs);
      xtoken();
      backinput();   /*:1152*/
      goto _Lrestart;
    }
    break;

  case charnum:
    scancharnum();
    curchr = curval;
    curcmd = chargiven;
    goto _Lreswitch;
    break;

  case mathcharnum:
    scanfifteenbitint();
    c = curval;
    break;

  case mathgiven:
    c = curchr;
    break;

  case delimnum:   /*1153:*/
    scantwentysevenbitint();
    c = curval / 4096;
    break;

  default:
    backinput();
    scanleftbrace();
    saved(0) = p;
    saveptr++;
    pushmath(mathgroup);
    goto _Lexit;   /*:1153*/
    break;
  }
  mathtype(p) = mathchar;
  character(p) = c & 255;
  if (c >= varcode && faminrange) {
    fam(p) = curfam;
  } else
    fam(p) = (c / 256) & 15;
_Lexit: ;
}
