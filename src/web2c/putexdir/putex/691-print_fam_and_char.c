Static void print_fam_and_char(halfword p)
{
  printesc(S(333));
  printint(fam(p));
  printchar(' ');
  print(character(p) - minquarterword);
}
