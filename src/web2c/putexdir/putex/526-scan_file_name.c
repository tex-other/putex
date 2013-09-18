Static void scanfilename(void)
{
  nameinprogress = true;
  beginname();
  /*406:*/
  do {
    getxtoken();
    /*:406*/
  } while (curcmd == spacer);
  while (true) {
    if (curcmd > otherchar || curchr > 255) {
      backinput();
      goto _Ldone;
    }
    if (!morename(curchr))
      goto _Ldone;
    getxtoken();
  }
_Ldone:
  endname();
  nameinprogress = false;
}
