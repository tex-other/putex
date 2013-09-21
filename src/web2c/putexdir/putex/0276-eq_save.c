Static void eqsave(halfword p, quarterword l)
{
  if (saveptr > maxsavestack) {
    maxsavestack = saveptr;
    if (maxsavestack > savesize - 6)
      overflow(S(476), savesize);
  }
  if (l == levelzero)
    savetype(saveptr) = restorezero;
  else {
    savestack[saveptr] = eqtb[p - activebase];
    saveptr++;
    savetype(saveptr) = restoreoldvalue;
  }
  savelevel(saveptr) = l;
  saveindex(saveptr) = p;
  saveptr++;
}
