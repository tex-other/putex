Static void short_display(long p)
{
  long n;
  while (p > memmin) {
    if (is_char_node(p)) {
      if (p <= memend) {
	if (font(p) != font_in_short_display) {
	  if ((font(p) < 0 ) || (font(p) > fontmax))
	    print_char('*');
	  else
		printesc(fontidtext(font(p)));
	  printchar(' ');
	  font_in_short_display = font(p);
	}
	print(character(p) - minquarterword);
      }
    } else {
      switch (type(p)) {

      case hlistnode:
      case vlistnode:
      case insnode:
      case whatsitnode:
      case marknode:
      case adjustnode:
      case unsetnode:
	print(S(328));
	break;

      case rulenode:
	printchar('|');
	break;

      case gluenode:
	if (glueptr(p) != zeroglue)
	  printchar(' ');
	break;

      case mathnode:
	printchar('$');
	break;

      case ligaturenode:
	shortdisplay(ligptr(p));
	break;

      case discnode:
	shortdisplay(prebreak(p));
	shortdisplay(postbreak(p));
	n = replacecount(p);
	while (n > 0) {
	  if (link(p) != 0)
	    p = link(p);
	  n--;
	}
	break;
      }
    }
    p = link(p);
  }
}
