/* A |math_node|, which occurs only in horizontal lists, appears before and
after mathematical formulas. The |subtype| field is |before| before the
formula and |after| after it. There is a |width| field, which represents
the amount of surrounding space inserted by \.{\\mathsurround}. */
static pointer new_math(scaled w, small_number s)
{
  pointer p; /* the new node */
  p=get_node(small_node_size); type(p) = math_node;
  subtype(p)=s; width(p)=w;
  return p;
}
