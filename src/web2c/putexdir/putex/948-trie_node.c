Static triepointer trienode(triepointer p)
{
  triepointer Result, h, q;

  h = abs(triec[p] + trieo[p] * 1009 + triel[p] * 2718 + trier[p] * 3142) %
      triesize;
  while (true) {
    q = triehash[h];
    if (q == 0) {
      triehash[h] = p;
      Result = p;
      goto _Lexit;
    }
    if (triec[q] == triec[p] && trieo[q] == trieo[p] &&
	triel[q] == triel[p] && trier[q] == trier[p]) {
      Result = q;
      goto _Lexit;
    }
    if (h > 0)
      h--;
    else
      h = triesize;
  }
_Lexit:
  return Result;
}
