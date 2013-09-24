/* The |show_node_list| routine requires some auxiliary subroutines: one to
print a font-and-character combination, one to print a token list without
its reference count, and one to print a rule dimension. */
void print_font_and_char(integer p)
{
  if (p>mem_end) {
    print_esc("CLOBBERED.");
    return;
  }
  if ((font(p)<font_base)||(font(p)>font_max))
    print_char('*');
  else
	print_esc(fontidtext(font(p)));
  printchar(' '); print_ASCII(qo(character(p)));
}
