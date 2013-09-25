/* While we're at it, we might as well deal with similar routines that
will be needed later. */
void scan_four_bit_int(void)
{
  scan_int();
  if (cur_val<0 || cur_val>15) {
    print_err("Bad number");
    help2("Since I expected to read a number between 0 and 15,",
      "I changed this one to zero."); int_error(cur_val); cur_val=0;
  }
}

