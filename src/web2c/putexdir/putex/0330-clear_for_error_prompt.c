/* In order to keep the stack from overflowing during a long sequence of
inserted `\.{\\show}' commands, the following routine removes completed
error-inserted lines from memory.
 */
void clear_for_error_prompt(void)
{
  while (state!=token_list && terminal_input &&
    input_ptr>0 && loc>limit) end_file_reading();
  print_ln(); clear_terminal();
}

