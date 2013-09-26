/* Here is a procedure that might be called `Get the next non-blank non-relax
non-call non-assignment token'. */
void do_assignments(void)
{
  while (true) {
    do {
      get_x_token();
    } while (cur_cmd==spacer || cur_cmd==relax);
    if (cur_cmd<=max_non_prefixed_command) goto PUexit;
    set_box_allowed=false; prefixed_command(); set_box_allowed=true;
  }
PUexit: ;
}
