Static void newsavelevel(groupcode c)
{
  if (saveptr > maxsavestack) {
    maxsavestack = saveptr;
    if (maxsavestack > savesize - 6)
      overflow(S(476), savesize);
  }
  savetype(saveptr) = levelboundary;
  savelevel(saveptr) = curgroup;
  saveindex(saveptr) = curboundary;
  if (curlevel == maxquarterword)
    overflow(S(477), maxquarterword - minquarterword);
  curboundary = saveptr;
  curlevel++;
  saveptr++;
  curgroup = c;
}

