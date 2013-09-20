Static void triefix(triepointer p)
{
  triepointer q;
  ASCIIcode c;
  triepointer z;

  z = triehash[p];
  do {
    q = triel[p];
    c = triec[p];
    trielink(z + c) = triehash[q];
    triechar(z + c) = c;
    trieop(z + c) = trieo[p];
    if (q > 0)
      triefix(q);
    p = trier[p];
  } while (p != 0);
}
