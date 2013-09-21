Static triepointer compresstrie(triepointer p)
{
  if (p == 0)
    return 0;
  else {
    triel[p] = compresstrie(triel[p]);
    trier[p] = compresstrie(trier[p]);
    return (trienode(p));
  }
}
