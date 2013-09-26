/* Another handy subroutine computes the height plus depth of
a given character: */
scaled heigth_plus_depth(internalfontnumber f, quarterword c)
{
  four_quarters q;
  eight_bits hd; /* |height_depth| byte */

  q=char_info(f,c); hd=height_depth(q);
  return (char_height(f,hd)+char_depth(f,hd));
}
