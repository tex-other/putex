Static void showwhatever(void)
{
  pointer p;

  switch (curchr) {

  case showlists:
    begindiagnostic();
    showactivities();
    break;

  case showboxcode:   /*1296:*/
    scaneightbitint();
    begindiagnostic();
    printnl(S(979));
    printint(curval);
    printchar('=');
    if (box(curval) == 0)
      print(S(465));
    else
      showbox(box(curval));
    break;
    /*:1296*/

  case showcode:   /*1294:*/
    gettoken();
    printnl(S(980));
    if (curcs != 0) {
      sprintcs(curcs);
      printchar('=');
    }
    printmeaning(curchr, curcmd);
    goto _Lcommonending;
    break;
    /*:1294*/
    /*1297:*/

  default:
    p = thetoks();
    printnl(S(980));
    tokenshow(temphead);
    flushlist(link(temphead));
    goto _Lcommonending;   /*:1297*/
    break;
  }/*1298:*/
  enddiagnostic(true);
  printnl(S(292));
  print(S(981));
  if (selector == termandlog) {
    if (tracingonline <= 0) {   /*:1298*/
      selector = termonly;
      print(S(982));
      selector = termandlog;
    }
  }
_Lcommonending:
  if (interaction < errorstopmode) {
    helpptr = 0;
    errorcount--;
  } else if (tracingonline > 0) {
    help3(S(983),S(984),S(985));
  } else {
    help5(S(983),S(984),S(985),S(986),S(987));
  }
  error();
}
