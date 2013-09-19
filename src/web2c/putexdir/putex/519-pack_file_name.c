void packfilename(strnumber n, strnumber a, strnumber e)
{
  long k;

  k = 0;
  filename_k = 0;
  str_map(a,appendtoname);
  str_map(n,appendtoname);
  str_map(e,appendtoname);
  k = filename_k;
  if (k <= filenamesize)
    namelength = k;
  else
    namelength = filenamesize;
  for (k = namelength; k < filenamesize; k++)
    nameoffile[k] = ' ';
}
