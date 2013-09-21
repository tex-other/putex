Static void showbox(halfword p)
{  /*236:*/
  depththreshold = showboxdepth;
  breadthmax = showboxbreadth;   /*:236*/
  if (breadthmax <= 0)
    breadthmax = 5;
#if 0
  if (poolptr + depththreshold >= poolsize)
    depththreshold = poolsize - poolptr - 1;
#else
	depththreshold = str_adjust_to_room(depththreshold);
#endif
  shownodelist(p);
  println();
}
