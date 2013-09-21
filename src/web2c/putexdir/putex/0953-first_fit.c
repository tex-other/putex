Static void firstfit(triepointer p)
{
  triepointer h, z, q;
  ASCIIcode c;
  triepointer l, r;
  short ll;

  c = triec[p];
  z = triemin[c];
  while (true) {
    h = z - c;   /*954:*/
    if (triemax < h + 256) {   /*:954*/
      if (triesize <= h + 256)
	overflow(S(769), triesize);
      do {
	triemax++;
	P_clrbits_B(trietaken, triemax - 1, 0, 3);
	trielink(triemax) = triemax + 1;
	trieback(triemax) = triemax - 1;
      } while (triemax != h + 256);
    }
    if (P_getbits_UB(trietaken, h - 1, 0, 3))   /*955:*/
      goto _Lnotfound;
    q = trier[p];
    while (q > 0) {
      if (trielink(h + triec[q]) == 0)
	goto _Lnotfound;
      q = trier[q];
    }
    goto _Lfound;   /*:955*/
_Lnotfound:
    z = trielink(z);
  }
_Lfound:   /*956:*/
  P_putbits_UB(trietaken, h - 1, 1, 0, 3);
  triehash[p] = h;
  q = p;
  do {
    z = h + triec[q];
    l = trieback(z);
    r = trielink(z);
    trieback(r) = l;
    trielink(l) = r;
    trielink(z) = 0;
    if (l < 256) {
      if (z < 256)
	ll = z;
      else
	ll = 256;
      do {
	triemin[l] = r;
	l++;
      } while (l != ll);
    }
    q = trier[q];   /*:956*/
  } while (q != 0);
}
