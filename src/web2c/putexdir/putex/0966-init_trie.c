Static void inittrie(void)
{   /*952:*/
  triepointer p;
  long j, k, t;
  triepointer r, s;
  twohalves h;

  /*945:*/
  opstart[0] = -minquarterword;
  for (j = 1; j <= 255; j++)
    opstart[j] = opstart[j - 1] + trieused[j - 1] - minquarterword;
  for (j = 1; j <= trieopptr; j++)
    trieophash[j + trieopsize] = opstart[trieoplang[j - 1]] + trieopval[j - 1];
  for (j = 1; j <= trieopptr; j++) {
    while (trieophash[j + trieopsize] > j) {   /*:945*/
      k = trieophash[j + trieopsize];
      t = hyfdistance[k - 1];
      hyfdistance[k - 1] = hyfdistance[j - 1];
      hyfdistance[j - 1] = t;
      t = hyfnum[k - 1];
      hyfnum[k - 1] = hyfnum[j - 1];
      hyfnum[j - 1] = t;
      t = hyfnext[k - 1];
      hyfnext[k - 1] = hyfnext[j - 1];
      hyfnext[j - 1] = t;
      trieophash[j + trieopsize] = trieophash[k + trieopsize];
      trieophash[k + trieopsize] = k;
    }
  }
  for (p = 0; p <= triesize; p++)
    triehash[p] = 0;
  trieroot = compresstrie(trieroot);
  for (p = 0; p <= trieptr; p++)
    triehash[p] = 0;
  for (p = 0; p <= 255; p++)
    triemin[p] = p + 1;
  trielink(0) = 1;
  triemax = 0;   /*:952*/
  if (trieroot != 0) {
    firstfit(trieroot);
    triepack(trieroot);
  }
  /*958:*/
  h.rh = 0;
  h.UU.U2.b0 = minquarterword;
  h.UU.U2.b1 = minquarterword;
  if (trieroot == 0) {
    for (r = 0; r <= 256; r++)
      trie[r] = h;
    triemax = 256;
  } else {
    triefix(trieroot);
    r = 0;
    do {
      s = trielink(r);
      trie[r] = h;
      r = s;
    } while (r <= triemax);
  }
  triechar(0) = '?';   /*:958*/
  trienotready = false;
}
