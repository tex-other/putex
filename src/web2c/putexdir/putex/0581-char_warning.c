Static void charwarning(internalfontnumber f, eightbits c)
{
  if (tracinglostchars <= 0)
    return;
  begindiagnostic();
  printnl(S(678));
  print(c);
  print(S(679));
  slowprint(get_fontname(f));
  printchar('!');
  enddiagnostic(false);
}
