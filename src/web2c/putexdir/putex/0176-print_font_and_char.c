Static void print_font_and_char(long p)
{
  if (p>memend) {
    print_esc(S(308));
    return;
  }
  if ((font(p)>fontmax))
    print_char('*');
  else
	print_esc(fontidtext(font(p)));
  printchar(' ');
  print(character(p)-minquarterword);
}
