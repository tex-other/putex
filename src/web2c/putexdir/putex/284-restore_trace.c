Static void restoretrace(halfword p, strnumber s)
{
  begindiagnostic();
  printchar('{');
  print(s);
  printchar(' ');
  showeqtb(p);
  printchar('}');
  enddiagnostic(false);
}

