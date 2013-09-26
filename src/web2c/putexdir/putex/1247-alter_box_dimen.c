void alter_box_dimen(void)
{
  small_number c; /* |width_offset| or |height_offset| or |depth_offset| */
  eight_bits b; /* box number */
  c=cur_chr; scan_eight_bit_int(); b=cur_val; scan_optional_equals();
  scan_normal_dimen();
  if (box(b)!=0) mem[box(b)+c].sc=cur_val;
}
