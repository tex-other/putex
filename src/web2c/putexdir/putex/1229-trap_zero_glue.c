Static void trapzeroglue(void)
{
  if (!((width(curval) == 0) & (stretch(curval) == 0) &
	(shrink(curval) == 0)))
    return;
  addglueref(zeroglue);
  deleteglueref(curval);
  curval = zeroglue;
}
