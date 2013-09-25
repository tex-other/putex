void ins_error(void)
{
  OK_to_interrupt=false; back_input(); token_type=inserted;
  OK_to_interrupt=true; error();
}
