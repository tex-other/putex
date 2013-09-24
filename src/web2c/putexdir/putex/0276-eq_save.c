/* To save a value of |eqtb[p]| that was established at level |l|, we
can use the following subroutine. */
void eq_save(pointer p, quarterword l) /* saves |eqtb[p]| */
{
  if (save_ptr>max_save_stack) {
    max_save_stack=save_ptr;
    if (max_save_stack>save_size-6) overflow("save size", save_size);
  }
  if (l==level_zero) save_type(save_ptr)=restore_zero;
  else {
    save_stack[save_ptr]=eqtb[p]; incr(saveptr);
    save_type(save_ptr)=restore_old_value;
  }
  save_level(save_ptr)=l; save_index(save_ptr)=p; incr(saveptr);
}
