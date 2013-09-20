Static void pauseforinstructions(void)
{
  if (!OKtointerrupt)
    return;
  interaction = errorstopmode;
  if (selector == logonly || selector == noprint)
    selector++;
  printnl(S(292));
  print(S(304));
  help3(S(305),S(306),S(307));
  deletionsallowed = false;
  error();
  deletionsallowed = true;
  interrupt = 0;
}
