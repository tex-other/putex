/* When \TeX's work on one level is interrupted, the state is saved by
calling |push_nest|. This routine changes |head| and |tail| so that
a new (empty) list is begun; it does not change |mode| or |aux|. */
void push_nest(void) /* enter a new semantic level, save the old */
{
  if (nest_ptr>max_nest_stack) {
    max_nest_stack=nest_ptr;
    if (nest_ptr==nest_size) overflow("semantic nest size", nestsize);
  }
  nest[nest_ptr]=cur_list; /* stack the record */
  incr(nestptr); head=get_avail(); tail=head; prevgraf=0; mode_line=line;
}

