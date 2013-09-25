/* Subroutine |save_for_after| puts a token on the stack for save-keeping. */
void save_for_after(pointer t)
{
  if (cur_level>level_one) {
    if (save_ptr>max_save_stack) {
      max_save_stack=save_ptr;
      if (max_save_stack>save_size-6) overflow("save size", save_size);
    }
    save_type(save_ptr)=insert_token; save_level(save_ptr)=level_zero;
    save_index(save_ptr)=t; incr(save_ptr);
  }
}

