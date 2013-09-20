Static void appenditaliccorrection(void)
{
  pointer p;
  internalfontnumber f;
  int c;

  if (tail != head) {
    if (ischarnode(tail)) {
      p = tail;
	f = font(p);
	c = character(p);
    }
    else if (type(tail) == ligaturenode) {
      p = ligchar(tail);
	f = font_ligchar(tail);
	c = character_ligchar(tail);
    } else
      goto _Lexit;
/*    f = font(p); 
    tailappend(newkern(charitalic(f, charinfo(f, character(p)))));
*/
	tailappend(newkern(charitalic(f, charinfo(f,c))));
    subtype(tail) = explicit;
  }
_Lexit: ;
}
