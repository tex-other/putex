Static void saveforafter(halfword t)
{
  if (curlevel <= levelone)
    return;
  if (saveptr > maxsavestack) {
    maxsavestack = saveptr;
    if (maxsavestack > savesize - 6)
      overflow(S(476), savesize);
  }
  savetype(saveptr) = inserttoken;
  savelevel(saveptr) = levelzero;
  saveindex(saveptr) = t;
  saveptr++;
}

