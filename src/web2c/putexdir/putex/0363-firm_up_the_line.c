Static void firmuptheline(void)
{
  short k;

  limit = last;
  if (pausing <= 0)
    return;
  if (interaction <= nonstopmode)
    return;
  println();
  if (start < limit) {
    for (k = start; k < limit; k++)
      print(buffer[k]);
  }
  first = limit;
  print(S(532));
  terminput();
  if (last <= first)
    return;
  for (k = first; k < last; k++)
    buffer[k + start - first] = buffer[k];
  limit = start + last - first;
}
