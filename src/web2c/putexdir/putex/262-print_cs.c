Static void printcs(long p)
{
  if (p < hashbase) {
    if (p >= singlebase) {
      if (p == nullcs) {
	printesc(S(262));
	printesc(S(263));
	return;
      }
      printesc(p - singlebase);
      if (catcode(p - singlebase) == letter)
	printchar(' ');
      return;
    }
    if (p < activebase)
      printesc(S(264));
    else
      print(p - activebase);
    return;
  }
  if (p >= undefinedcontrolsequence) {
    printesc(S(264));
    return;
  }
  if ( !str_valid(text(p)))
    printesc(S(265));
  else {
    printesc(text(p));
    printchar(' ');
  }
}
