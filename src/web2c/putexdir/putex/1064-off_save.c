Static void offsave(void)
{
  pointer p;

  if (curgroup == bottomlevel) {   /*1066:*/
    printnl(S(292));
    print(S(558));
    printcmdchr(curcmd, curchr);
    help1(S(835));
    error();
    return;
  }  /*:1066*/
  backinput();
  p = getavail();
  link(temphead) = p;
  printnl(S(292));
  print(S(554));   /*1065:*/
  switch (curgroup) {   /*:1065*/

  case semisimplegroup:
    info(p) = cstokenflag + frozenendgroup;
    printesc(S(836));
    break;

  case mathshiftgroup:
    info(p) = mathshifttoken + '$';
    printchar('$');
    break;

  case mathleftgroup:
    info(p) = cstokenflag + frozenright;
    link(p) = getavail();
    p = link(p);
    info(p) = othertoken + '.';
    printesc(S(837));
    break;

  default:
    info(p) = rightbracetoken + '}';
    printchar('}');
    break;
  }
  print(S(555));
  inslist(link(temphead));
  help5(S(838),S(839),S(840),S(841),S(842));
  error();
}
