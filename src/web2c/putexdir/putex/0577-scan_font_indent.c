/* Before we forget about the format of these tables, let's deal with two
of \TeX's basic scanning routines related to font information. */
void scan_font_ident(void)
{
  internal_font_number f;
  halfword m;

  do {
    get_x_token();
  } while (cur_cmd==spacer);
  if (cur_cmd==def_font) f=cur_font;
  else if (cur_cmd==set_font) f=cur_chr;
  else if (cur_cmd==def_family) {
    m=cur_chr; scan_four_bit_int(); f=equiv(m+cur_val);
  } else {
    print_err("Missing font identifier");
    help2("I was looking for a control sequence whose",
      "current meaning has been defined by \font.");
    back_error(); f=null_font;
  }
  cur_val=f;
}
