Static boolean morename(ASCIIcode c)
{
  if (c == ' ')
    return false;
  else {
    str_room(1);
    if (c == '.' && extdelimiter == 0) {
	extdelimiter = makestring();
    }
    appendchar(c);
    return true;
  }
}
