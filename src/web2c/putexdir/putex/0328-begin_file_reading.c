/* The |begin_file_reading| procedure starts a new level of input for lines
of characters to be read from a file, or as an insertion from the
terminal. It does not take care of opening the file, nor does it set |loc|
or |limit| or |line|. */
void begin_file_reading(void)
{
  if (in_open==max_in_open) overflow("text input levels",max_in_open);
  if (first==buf_size) overflow("buffer size",buf_size);
  incr(in_open);
  /* enter a new input level, save the old */
  if (input_ptr>max_in_stack) {
    max_in_stack=input_ptr;
    if (input_ptr==stack_size) overflow("input stack size",stack_size);
  }
  input_stack[input_ptr]=cur_input; /* stack the record */
  incr(input_ptr);
  index=in_open;
  line_stack[index]=line; start=first; state=mid_line;
  name=0; /* |terminal_input| is now |true| */
}
