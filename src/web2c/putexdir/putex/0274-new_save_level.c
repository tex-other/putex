void new_save_level(group_code c) /* begin a new level of grouping */
{
  if (save_ptr>max_save_stack) {
    max_save_stack=save_ptr;
    if (max_save_stack>save_size-6) overflow("save size", save_size);
  }
  save_type(save_ptr)=level_boundary; save_level(save_ptr)=cur_group;
  save_index(save_ptr)=cur_boundary;
  if (cur_level==max_quarterword)
    overflow("grouping levels", max_quarterword-min_quarterword);
  cur_boundary=save_ptr; incr(cur_level); incr(save_ptr); cur_group=c;
}

