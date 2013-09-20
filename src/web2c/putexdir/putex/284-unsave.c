Static void unsave(void)
{
  pointer p;
  quarterword l=0 /* XXXX */;
  halfword t;

  if (curlevel <= levelone) {
    confusion(S(478));
    return;
  }
  curlevel--;   /*282:*/
  while (true) {
    saveptr--;
    if (savetype(saveptr) == levelboundary)
      goto _Ldone;
    p = saveindex(saveptr);
    if (savetype(saveptr) == inserttoken) {   /*326:*/
      t = curtok;
      curtok = p;
      backinput();
      curtok = t;
      continue;
    }  /*:326*/
    if (savetype(saveptr) == restoreoldvalue) {   /*283:*/
      l = savelevel(saveptr);
      saveptr--;
    } else
      savestack[saveptr] = eqtb[undefinedcontrolsequence - activebase];
    if (p < intbase) {
      if (eqlevel(p) == levelone) {
	eqdestroy(savestack[saveptr]);   /*_STAT*/
	if (tracingrestores > 0)   /*_ENDSTAT*/
	  restoretrace(p, S(479));
      } else {
	eqdestroy(eqtb[p - activebase]);
	eqtb[p - activebase] = savestack[saveptr];   /*_STAT*/
	if (tracingrestores > 0)   /*_ENDSTAT*/
	  restoretrace(p, S(480));
      }
      continue;
    }
    if (xeqlevel[p - intbase] != levelone) {
      eqtb[p - activebase] = savestack[saveptr];
      xeqlevel[p - intbase] = l;   /*_STAT*/
      if (tracingrestores > 0)   /*_ENDSTAT*/
	restoretrace(p, S(480));
    } else   /*:283*/
    {  /*_STAT*/
      if (tracingrestores > 0)   /*_ENDSTAT*/
	restoretrace(p, S(479));
    }
  }
_Ldone:
  curgroup = savelevel(saveptr);
  curboundary = saveindex(saveptr);   /*:282*/
}
