static pointer new_choice(void) /* create a choice node */
{
  pointer p; /* the new node */
  p=get_node(style_node_size); type(p)=choice_node;
  subtype(p)=0; /* the |subtype| is not used */
  display_mlist(p)=0; text_mlist(p)=0; script_mlist(p)=0;
  script_script_mlist(p)=0;
  return p;
}
