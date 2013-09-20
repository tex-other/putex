Static void newwritewhatsit(smallnumber w)
{
  newwhatsit(curchr, w);
  if (w != writenodesize)
    scanfourbitint();
  else {
    scanint();
    if (curval < 0)
      curval = 17;
    else if (curval > 15)
      curval = 16;
  }
  writestream(tail) = curval;
}
