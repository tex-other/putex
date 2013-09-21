/* An entire string is output by calling |print|. Note that if we are outputting
the single standard ASCII character \.c, we could call |print("c")|, since
|"c"=99| is the number of a single-character string, as explained above. But
|print_char("c")| is quicker, so \TeX\ goes directly to the |print_char|
routine when it knows that this is safe. (The present implementation
assumes that it is always safe to print a visible ASCII character.)
@^system dependencies@> */
void print(integer s) /* prints string |s| */
{
  long nl;
  if (s<256) {
    if (s>=0) {
      if (selector>pseudo) {
	    print_char(s); goto PUexit; /* internal strings are not expanded */
      }
      if (s==newlinechar) {
        if (selector<pseudo) {
          println(); goto PUexit;
        }
      }
      nl=new_line_char; new_line_char = -1;
      str_print(s);
      newlinechar=nl;
      goto PUexit;
    }
    s = S(261);
  }
  str_print(s);
PUexit: ;
}
