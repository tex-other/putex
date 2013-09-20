Static void appenddiscretionary(void)
{
  long c;

  tailappend(newdisc());
  if (curchr == 1) {
    c = get_hyphenchar(curfont);
    if (c >= 0) {
      if (c < 256)
	prebreak(tail) = newcharacter(curfont, c);
    }
    return;
  }
  saveptr++;
  saved(-1) = 0;
  newsavelevel(discgroup);
  scanleftbrace();
  pushnest();
  mode = -hmode;
  spacefactor = 1000;
}

