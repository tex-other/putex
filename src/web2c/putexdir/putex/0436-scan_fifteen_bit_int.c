void scan_fifteen_bit_int(void)
{
  scan_int();
  if (cur_val<0 || cur_val>077777) {
    print_err("Bad mathchar");
    help2("A mathchar number must be between 0 and 32767.",
      "I changed this one to zero."); int_error(cur_val); cur_val=0;
  }
}
