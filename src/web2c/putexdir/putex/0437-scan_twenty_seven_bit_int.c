void scan_twenty_seven_bit_int(void)
{
  scan_int();
  if (cur_val<0 || cur_val>0777777777) {
    print_err("Bad delimiter code");
    help2("A numeric delimiter code must be between 0 and 2^{27}-1.",
      "I changed this one to zero."); int_error(cur_val); cur_val=0;
  }
}
