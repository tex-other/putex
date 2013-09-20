Static void printsubsidiarydata(halfword p, ASCIIcode c)
{
  if (get_cur_length() >= depththreshold) {
    if (mathtype(p) != empty)
      print(S(334));
    return;
  }
  appendchar(c);
  tempptr = p;
  switch (mathtype(p)) {

  case mathchar:
    println();
    printcurrentstring();
    printfamandchar(p);
    break;

  case subbox:
    showinfo();
    break;

  case submlist:
    if (info(p) == 0) {
      println();
      printcurrentstring();
      print(S(335));
    } else
      showinfo();
    break;
  }
  flushchar();
}
