void scan_char_num(void)
{
  scan_int();
  if (cur_val<0 || cur_val>255) {
    print_err("Bad character code");
    help2("A character number must be between 0 and 255.",
      "I changed this one to zero."); int_error(cur_val); cur_val=0;
  }
}
