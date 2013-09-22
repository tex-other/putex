static pointer new_style(small_number s) /* create a style node */
{
  pointer p; /* the new node */
  p=get_node(style_node_size); type(p)=style_node;
  subtype(p)=s; width(p)=0; depth(p)=0; /* the |width| and |depth| are not used */
  return p;
} 
