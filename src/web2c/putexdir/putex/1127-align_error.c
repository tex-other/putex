Static void alignerror(void)
{
  if (labs(alignstate) > 2) {   /*1128:*/
    printnl(S(292));
    print(S(885));
    printcmdchr(curcmd, curchr);
    if (curtok == tabtoken + '&') {
      help6(S(886),S(887),S(888),S(889),S(890),S(891));
    } else {
      help5(S(886),S(892),S(889),S(890),S(891));
    }
    error();
    return;
  }
  /*:1128*/
  backinput();
  if (alignstate < 0) {
    printnl(S(292));
    print(S(566));
    alignstate++;
    curtok = leftbracetoken + '{';
  } else {
    printnl(S(292));
    print(S(893));
    alignstate--;
    curtok = rightbracetoken + '}';
  }
  help3(S(894),S(895),S(896));
  inserror();
}
