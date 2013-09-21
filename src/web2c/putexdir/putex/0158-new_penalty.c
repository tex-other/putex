/* A |penalty_node| specifies the penalty associated with line or page
breaking, in its |penalty| field. This field is a fullword integer, but
the full range of integer values is not used: Any penalty |>=10000| is
treated as infinity, and no break will be allowed for such high values.
Similarly, any penalty |<=-10000| is treated as negative infinity, and a
break will be forced. */
static pointer new_penalty(integer m)
{
  pointer p; /* the new node */
  p=get_node(small_node_size); type(p)=penalty_node;
  subtype(p)=0; /* the |subtype| is not used */
  penalty(p)=m;
  return p;
}
