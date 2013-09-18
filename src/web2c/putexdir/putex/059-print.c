void print(strnumber s)
{
  long nl;

   if (s < 256) {
    if (s >= 0) {
      if (selector > pseudo) {
	printchar(s);
	goto _Lexit;
      }
      if (s == newlinechar) {   /*244:*/
	if (selector < pseudo) {
	  println();
	  goto _Lexit;
	}
      }
      /*:244*/
      nl = newlinechar;
      newlinechar = -1;
        str_print(s);
      newlinechar = nl;
      goto _Lexit;
    }
    s = S(261);
  }
	str_print(s);
_Lexit: ;
}
