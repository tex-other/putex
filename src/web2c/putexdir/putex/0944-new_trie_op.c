Static quarterword newtrieop(smallnumber d, smallnumber n, quarterword v)
{
  quarterword Result;
  short h;
  quarterword u;
  short l;

  h = abs(n + d * 313 + v * 361 + curlang * 1009) % (trieopsize + trieopsize) -
      trieopsize;
  while (true) {
    l = trieophash[h + trieopsize];
    if (l == 0) {
      if (trieopptr == trieopsize)
	overflow(S(767), trieopsize);
      u = trieused[curlang];
      if (u == maxquarterword)
	overflow(S(768), maxquarterword - minquarterword);
      trieopptr++;
      u++;
      trieused[curlang] = u;
      hyfdistance[trieopptr - 1] = d;
      hyfnum[trieopptr - 1] = n;
      hyfnext[trieopptr - 1] = v;
      trieoplang[trieopptr - 1] = curlang;
      trieophash[h + trieopsize] = trieopptr;
      trieopval[trieopptr - 1] = u;
      Result = u;
      goto _Lexit;
    }
    if (hyfdistance[l - 1] == d && hyfnum[l - 1] == n &&
	hyfnext[l - 1] == v && trieoplang[l - 1] == curlang) {
      Result = trieopval[l - 1];
      goto _Lexit;
    }
    if (h > -trieopsize)
      h--;
    else
      h = trieopsize;
  }
_Lexit:
  return Result;
}
