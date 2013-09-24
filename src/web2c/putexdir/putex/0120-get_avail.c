/* If the available-space list is empty, i.e., if |avail=null|,
we try first to increase |mem_end|. If that cannot be done, i.e., if
|mem_end=mem_max|, we try to decrease |hi_mem_min|. If that cannot be
done, i.e., if |hi_mem_min=lo_mem_max+1|, we have to quit. */
pointer get_avail(void) /* single-word node allocation */
{
  pointer p; /* the new node being got */
  p=avail; /* get top location in the |avail| stack */
  if (p!=0) avail=link(avail); /* and pop it off */
  else if (mem_end+char_node_size<=mem_max) {
	p=mem_end+1; mem_end+=char_node_size;
  } else {
	p=hi_mem_min-1;	hi_mem_min-=char_node_size;
    if (hi_mem_min<=lo_mem_max) {
      runaway(); /* if memory is exhausted, display possible runaway text */
      overflow("main memory size", mem_max-mem_min+1);
      /* quit; all one-word nodes are busy */
    }
  }
  set_as_char_node(p);
  link(p)=0; /* provide an oft-desired initialization of the new node */
  dynused += charnodesize;
  return p;
}
