/* Here is a function that returns a pointer to a character node for a
given character in a given font. If that character doesn't exist,
|null| is returned instead. */
pointer new_character(internal_font_number f, eight_bits c)
{
  halfword Result;
  pointer p; /* newly allocated node */

  if (font_bc[f]<=c) {
    if (font_ec[f]>=c) {
      if (char_exists(charinfo(f, qi(c)))) {
        p=get_avail(); font(p)=f; character(p)=qi(c);
        return p;
      }
    }
  }
  char_warning(f,c);
}
