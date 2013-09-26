void open_or_close_in(void)
{
  unsigned char c; /* 1 for \.{\\openin}, 0 for \.{\\closein} */
  unsigned char n; /* stream number */

  c=cur_chr; scan_four_bit_int(); n=cur_val;
  if (read_open[n]!=closed) {
    a_close(&readfile[n]);
    read_open[n]=closed;
  }
  if (c!=0) {
    scan_optional_equals(); scan_file_name();
    if (cur_ext=="") cur_ext=".tex";
    pack_file_name(cur_name,"",cur_ext);
  if (a_open_in(&read_file[n])) read_open[n]=just_open;
}

