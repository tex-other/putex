/* Here is a function that returns a pointer to a rule node having a given
thickness |t|. The rule will extend horizontally to the boundary of the vlist
that eventually contains it. */
static pointer fraction_rule(scaled t) /* construct the bar for a fraction */
{
  pointer p; /* the new node */
  p=new_rule(); height(p)=t; depth(p)=0;
  return p;
}
