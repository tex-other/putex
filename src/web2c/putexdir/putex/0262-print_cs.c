/* Single-character control sequences do not need to be looked up in a hash
table, since we can use the character code itself as a direct address.
The procedure |print_cs| prints the name of a control sequence, given
a pointer to its address in |eqtb|. A space is printed after the name
unless it is a single nonletter or an active character. This procedure
might be invoked with invalid data, so it is ``extra robust.'' The
individual characters must be printed one at a time using |print|, since
they may be unprintable. */
void printcs(integer p) /* prints a purported control sequence */
{
  if (p<hash_base) { /* single character */
    if (p>=single_base) {
      if (p==null_cs) {
        print_esc("csname"); print_esc("endcsname");
        return;
      } else {
        print_esc(p-single_base);
       if (catcode(p-single_base)==letter) print_char(' ');
       return;
      }
    }else if (p<active_base)
      print_esc("IMPOSSIBLE.");
    else
      print(p-active_base)
    return;
  }else if (p>=undefined_control_sequence) {
    print_esc("IMPOSSIBLE.");
    return;
  }else if ((text(p)<0) || (text(p)>=str_ptr))
    print_esc("NONEXISTENT.");
  else {
    print_esc(text(p));
    print_char(' ');
  }
}
