void alter_page_so_far(void)
{
  unsigned char c; /* index into |page_so_far| */
  c=cur_chr; scan_optional_equals(); scan_normal_dimen();
  page_so_far[c]=cur_val;
}
