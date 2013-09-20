Static void pushnest(void)
{
  if (nestptr > maxneststack) {
    maxneststack = nestptr;
    if (nestptr == nestsize)
      overflow(S(438), nestsize);
  }
  nest[nestptr] = curlist;
  nestptr++;
  head = getavail();
  tail = head;
  prevgraf = 0;
  modeline = line;
}

