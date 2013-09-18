Static void fetch(halfword a)
{
  curc = character(a);
  curf = famfnt(fam(a) + cursize);
  if (curf == nullfont) {   /*723:*/
    printnl(S(292));
    print(S(385));
    printsize(cursize);
    printchar(' ');
    printint(fam(a));
    print(S(715));
    print(curc - minquarterword);
    printchar(')');
    help4(S(716),S(717),S(718),S(719));
    error();
    curi = nullcharacter;
    mathtype(a) = empty;
    return;
  }  /*:723*/
  if (curc - minquarterword >= fontbc[curf ] &&
      curc - minquarterword <= fontec[curf ])
    curi = charinfo(curf, curc);
  else
    curi = nullcharacter;
  if (!charexists(curi)) {
    charwarning(curf, curc - minquarterword);
    mathtype(a) = empty;
  }
}
