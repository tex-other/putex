/* Conversely, when some variable-size node |p| of size |s| is no longer needed,
the operation |free_node(p,s)| will make its words available, by inserting
|p| as a new empty node just before where |rover| now points.
@^inner loop@> */
static void free_node(pointer p, halfword s) /* variable-size node liberation */
{
  pointer q; /* |llink(rover)| */
  node_size(p)=s; link(p)=empty_flag;
  q=llink(rover); llink(p)=q; rlink(p)=rover; /* set both links */
  llink(rover)=p; rlink(q)=p; /* insert |p| into the ring */
  var_used-=s; /* maintain statistics */
}
