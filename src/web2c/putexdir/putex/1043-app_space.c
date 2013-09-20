Static void appspace(void)
{
  pointer q;

  if (spacefactor >= 2000 && xspaceskip != zeroglue)
    q = newparamglue(xspaceskipcode);
  else {
    if (spaceskip != zeroglue)
      mainp = spaceskip;
    else {   /*1042:*/
      mainp = fontglue[curfont ];
      if (mainp == 0) {
	fontindex mmaink;
	mainp = newspec(zeroglue);
	mmaink = parambase[curfont ] + spacecode;
#if 1
	maink = mmaink;
#endif
	width(mainp) = fontinfo[maink].sc;
	stretch(mainp) = fontinfo[mmaink + 1].sc;
	shrink(mainp) = fontinfo[mmaink + 2].sc;
	fontglue[curfont ] = mainp;
      }
    }
    mainp = newspec(mainp);   /*1044:*/
    if (spacefactor >= 2000)
      width(mainp) += extraspace(curfont);
    stretch(mainp) = xnoverd(stretch(mainp), spacefactor, 1000);
    shrink(mainp) = xnoverd(shrink(mainp), 1000, spacefactor);
	/*:1044*/
    q = newglue(mainp);
    gluerefcount(mainp) = 0;
  }
  link(tail) = q;
  tail = q;

  /*:1042*/
}
