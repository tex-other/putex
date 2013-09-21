Static void popnest(void)
{
  freeavail(head);
  nestptr--;
  curlist = nest[nestptr];
}
