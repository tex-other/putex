Static void extrarightbrace(void)
{
  printnl(S(292));
  print(S(843));
  switch (curgroup) {

  case semisimplegroup:
    printesc(S(836));
    break;

  case mathshiftgroup:
    printchar('$');
    break;

  case mathleftgroup:
    printesc(S(419));
    break;
  }
  help5(S(844),S(845),S(846),S(847),S(848));
  error();
  alignstate++;
}
