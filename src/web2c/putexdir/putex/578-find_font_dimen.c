Static void findfontdimen(boolean writing)
{
  internalfontnumber f;
  long n;

  scanint();
  n = curval;
  scanfontident();
  f = curval;
  if (n <= 0)
    curval = fmemptr;
  else {
    if (writing && n <= spaceshrinkcode && n >= spacecode &&
	fontglue[f ] != 0) {
      deleteglueref(fontglue[f ]);
      fontglue[f ] = 0;
    }
    if (n > fontparams[f ]) {
      if (f < fontptr)
	curval = fmemptr;
      else  /*580:*/
      {   /*:580*/
	do {
	  if (fmemptr == fontmemsize)
	    overflow(S(587), fontmemsize);
	  fontinfo[fmemptr].sc = 0;
	  fmemptr++;
	  fontparams[f ]++;
	} while (n != fontparams[f ]);
	curval = fmemptr - 1;
      }
    } else
      curval = n + parambase[f ];
  }  /*579:*/
  if (curval != fmemptr)   /*:579*/
    return;
  printnl(S(292));
  print(S(588));
  printesc(fontidtext(f));
  print(S(589));
  printint(fontparams[f ]);
  print(S(590));
  help2(S(591),S(592));
  error();
}
