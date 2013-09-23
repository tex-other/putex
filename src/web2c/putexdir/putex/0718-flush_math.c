/* Sometimes it is necessary to destroy an mlist. The following
subroutine empties the current list, assuming that |abs(mode)=mmode|. */
void flush_math(void)
{
  flush_node_list(link(head)); flush_node_list(incompleat_noad);
  link(head)=0; tail=head; incompleat_noad=0;
}
