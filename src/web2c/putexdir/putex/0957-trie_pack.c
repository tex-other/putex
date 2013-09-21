Static void triepack(triepointer p)
{
  triepointer q;

  do {
    q = triel[p];
    if (q > 0 && triehash[q] == 0) {
      firstfit(q);
      triepack(q);
    }
    p = trier[p];
  } while (p != 0);
}

