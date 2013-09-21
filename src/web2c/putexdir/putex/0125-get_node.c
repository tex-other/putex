/* A call to |get_node| with argument |s| returns a pointer to a new node
of size~|s|, which must be 2~or more. The |link| field of the first word
of this new node is set to null. An overflow stop occurs if no suitable
space exists.
If |get_node| is called with $s=2^{30}$, it simply merges adjacent free
areas and returns the value |max_halfword|. */
Static pointer get_node(integer s) /* variable-size node allocation */
{
  halfword Result;
  pointer p, /* the node currently under inspection */
  pointer q; /* the node physically after node |p| */
  integer r; /* the newly allocated node, or a candidate for this honor */
  integer t; /* temporary register */

PUrestart: p=rover; /* start at some free node in the ring */
  do {
    q=p+node_size(p); /* find the physical successor */
    while (is_empty(q)) { /* merge node |p| with node |q| */
      t=rlink(q);
      if (q==rover) rover=t;
      llink(t)=llink(q); rlink(llink(q))=t;
      q+=nodesize(q);
    }
    r=q-s;
    if (r>p+1) {
      node_size(p)=r-p; /* store the remaining size */
      rover=p; /* start searching here next time */
      goto PUfound;
    }
    if (r==p) {
      if (rlink(p)!=p) {
        rover=rlink(p);	t=llink(p); llink(rover)=t; rlink(t)=rover; goto PUfound;
      }
    }
    node_size(p)=q-p; /* reset the size in case it grew */
    p=rlink(p); /* move to the next node in the ring */
  } while (p!=rover);
  if (s==1073741824L) {
    Result = max_halfword; goto PUexit;
  }
  if (lo_mem_max+2<hi_mem_min) {
    if (lo_mem_max+2<=mem_bot+max_halfword) {
      if (hi_mem_min-lo_mem_max>=1998) t=lo_mem_max+1000;
      else t=lo_mem_max+(hi_mem_min-lo_mem_max)/2+1;
      p=llink(rover); q=lo_mem_max; rlink(p)=q; llink(rover)=q;
      if (t>mem_bot+max_halfword) t=mem_bot+max_halfword;
      rlink(q)=rover; llink(q)=p; link(q)=empty_flag; node_size(q)=t-lo_mem_max;
      lo_mem_max=t; link(lo_mem_max)=0; info(lo_mem_max)=0; rover=q;
      goto PUrestart;
    }
  }
  overflow("main memory size", mem_max-mem_min+1);
PUfound:
  unset_is_char_node(r);
  link(r)=0;   /*_STAT*/
  varused += s;   /*_ENDSTAT*/
  Result = r;
PUexit:
  return Result;
}
