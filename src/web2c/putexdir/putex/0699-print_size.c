void printsize(long s)
{
  if (s == 0) {
    printesc(S(266));
    return;
  }
  if (s == scriptsize)
    printesc(S(267));
  else
    printesc(S(268));
} 
