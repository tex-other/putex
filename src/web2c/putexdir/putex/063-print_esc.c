void printesc(strnumber s)
{  /*243:*/
  long c;

  c = escapechar;   /*:243*/
  if (c >= 0) {
    if (c < 256)
      print(c);
  }
  slowprint(s);
}
