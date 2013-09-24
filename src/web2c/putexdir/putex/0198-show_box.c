/* The recursive machinery is started by calling |show_box|. */
void showbox(halfword p)
{
  depth_threshold=show_box_depth;
  breadth_max=show_box_breadth;
  if (breadth_max<=0) breadth_max = 5;
#if 0
  if (poolptr + depththreshold >= poolsize)
    depththreshold = poolsize - poolptr - 1;
#else
	depth_threshold = str_adjust_to_room(depth_threshold);
#endif
  show_node_list(p);
  print_ln();
}
