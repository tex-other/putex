/* The next subroutine will descend to another level of recursion when a
subsidiary mlist needs to be displayed. The parameter |c| indicates what
character is to become part of the recursion history. An empty mlist is
distinguished from a field with |math_type(p)=empty|, because these are
not equivalent (as explained above). */
void print_subsidiary_data(pointer p, ASCII_code c)
/* display a noad field */
{
  if (get_cur_length()>=depth_threshold) {
    if (math_type(p)!=empty) print(" []");
  } else {
    append_char(c); /* include |c| in the recursion history */
    temp_ptr=p; /* prepare for |show_info| if recursion is needed */
    switch (math_type(p)) {
      case math_char: print_ln(); print_current_string(); print_fam_and_char(p); break;
      case sub_box: show_info(); break; /* recursive call */
      case sub_mlist:
        if (info(p)==0) {
          print_ln(); print_current_string(); print("{}");
        } else show_info(); break; /* recursive call */
      default: ; /* |empty| */
    }
  }
  flush_char(); /* remove |c| from the recursion history */
}
