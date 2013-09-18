Static void passtext(void)
{
  long l;
  smallnumber savescannerstatus;

  savescannerstatus = scannerstatus;
  scannerstatus = skipping;
  l = 0;
  skipline = line;
  while (true) {
    getnext();
    if (curcmd == fiorelse) {
      if (l == 0)
	goto _Ldone;
      if (curchr == ficode)
	l--;
    } else if (curcmd == iftest)
      l++;
  }
_Ldone:
  scannerstatus = savescannerstatus;
}
