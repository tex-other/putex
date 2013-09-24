/* Here is a similar procedure; it avoids the error checks, and it never
prints a space after the control sequence. */
void sprintcs(pointer p) /* prints a control sequence */
{
  if (p>=hash_base) print_esc(text(p));
  else {
    if (p<single_base) print(p-active_base);
    else {
      if (p<null_cs) print_esc(p-single_base);
      else {
        print_esc("csname"); print_esc("endcsname");
      }
    }
  }
}
