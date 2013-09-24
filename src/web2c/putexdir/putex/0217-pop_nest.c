/* Conversely, when \TeX\ is finished on the current level, the former
state is restored by calling |pop_nest|. This routine will never be
called at the lowest semantic level, nor will it be called unless |head|
is a node that should be returned to free memory. */
void pop_nest(void) /* leave a semantic level, re-enter the old */
{
  free_avail(head); decr(nestptr); cur_list=nest[nest_ptr];
}
