void printnl(strnumber s)
{
  if ( (termoffset > 0 && (selector & 1)) ||
      (fileoffset > 0 && selector >= logonly))
    println();
  print(s);
}
