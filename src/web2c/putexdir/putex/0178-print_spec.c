/* The next subroutine prints a whole glue specification. */
static void print_spec(integer p, str_number s) /* prints a glue specification */
{
  if (p<mem_min||p>=lo_mem_max) print_char('*');
  else {
    print_scaled(width(p));
    if (s!=0) print(s);
    if (stretch(p)!=0) {
      print(" plus "); print_glue(stretch(p),stretch_order(p),s);
    }
    if (shrink(p)!=0) {
      print(" minus "); print_glue(shrink(p),shrink_order(p),s);
    }
  }
}

