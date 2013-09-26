/* The parameters to |hpack| and |vpack| correspond to \TeX's primitives
like `\.{\\hbox} \.{to} \.{300pt}', `\.{\\hbox} \.{spread} \.{10pt}'; note
that `\.{\\hbox}' with no dimension following it is equivalent to
`\.{\\hbox} \.{spread} \.{0pt}'.  The |scan_spec| subroutine scans such
constructions in the user's input, including the mandatory left brace that
follows them, and it puts the specification onto |save_stack| so that the
desired box can later be obtained by executing the following code:
$$\vbox{\halign{#\hfil\cr
|save_ptr:=save_ptr-2;|\cr
|hpack(p,saved(1),saved(0)).|\cr}}$$
Special care is necessary to ensure that the special |save_stack| codes
are placed just below the new group code, because scanning can change
|save_stack| when \.{\\csname} appears. */
void scan_spec(group_code c, boolean three_codes)
/* scans a box specification and left brace */
{
  integer s=0 /* temporarily saved value */
  unsigned char spec_code;
  if (three_codes)  s=saved(0);
  if (scan_keyword("to")) spec_code=exactly;
  else if (scan_keyword("spread")) spec_code=additional;
  else {
    spec_code=additional; cur_val=0;
    goto PUfound;
  }
  scan_normal_dimen();
PUfound:
  if (three_codes) {
    saved(0)=s; incr(saveptr);
  }
  saved(0)=spec_code; saved(1)=cur_val; save_ptr+=2;
  new_save_level(c); scan_left_brace();
}
