Static void ensurevbox(eightbits n)
{
  pointer p;

  p = box(n);
  if (p == 0)
    return;
  if (type(p) != hlistnode)
    return;
  printnl(S(292));
  print(S(806));
  help3(S(807),S(808),S(809));
  boxerror(n);
}
