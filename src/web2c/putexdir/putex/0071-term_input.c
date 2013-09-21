Static void terminput(void)
{
  short k;

  fflush(termout);
  if (!inputln(&termin, true))
    fatalerror(S(302));
  termoffset = 0;
  selector--;
  if (last != first) {
    for (k = first; k < last; k++)
      print(buffer[k]);
  }
  println();
  selector++;
}
