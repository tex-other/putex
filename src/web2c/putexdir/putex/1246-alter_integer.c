void alter_integer(void)
{
  unsigned char c; /* 0 for \.{\\deadcycles}, 1 for \.{\\insertpenalties} */
  c=cur_chr; scan_optional_equals(); scan_int();
  if (c==0) dead_cycles=cur_val;
  else insert_penalties=cur_val;
}
