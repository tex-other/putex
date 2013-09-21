/* Here is a subroutine that creates a whatsit node having a given |subtype|
and a given number of words. It initializes only the first word of the whatsit,
and appends it to the current list. */
static void new_whatsit(small_number s, small_number w)
{
  pointer p; /* the new node */
  p=get_node(w); type(p)=whatsit_node; subtype(p)=s;
  link(tail)=p; tail=p;
}
