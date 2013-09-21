Static boolean scankeyword(strnumber s)
{
  boolean Result;
#if 1
  pointer p = getavail();
  pointer my_backup_head = p;
#else
  pointer my_backup_head = backuphead;
  pointer p = backuphead;
#endif
#if 0
  poolpointer k;
#else 
  int k;
  int k_e;
#endif
  link(p) = 0;
#if 0
  k = strstart[s];
  while (k < str_end(s) ) {
    getxtoken();
    if ((curcs == 0) & ((curchr == strpool[k]) |
			(curchr == strpool[k] - 'a' + 'A'))) {
#else
  k=0;
  k_e=flength(s);
  while(k<k_e) {
    int str_c = str_getc(s,k);
    getxtoken();
    if ((curcs == 0) & ((curchr == str_c) |
                        (curchr == str_c - 'a' + 'A'))) {
#endif
      storenewtoken(p,curtok);
      k++;
      continue;
    } else {
      if (curcmd == spacer && p == my_backup_head)
	continue;
      backinput();
      if (p != my_backup_head) {
	backlist(link(my_backup_head));
      }
      Result = false;
      goto _Lexit;
    }
  }
  flushlist(link(my_backup_head));
  Result = true;
_Lexit:

  freeavail(my_backup_head); 

  return Result;
}
