/* The procedure |print_esc| prints a string that is preceded by
the user's escape character (which is usually a backslash). */
void print_esc(str_number s) /* prints escape character, then |s| */
{  
  integer c; /* the escape character code */
  c=escape_char;
  if (c>=0) {
    if (c<256) print(c);
  }
  slow_print(s);
}
