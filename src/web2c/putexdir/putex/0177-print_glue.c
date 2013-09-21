/* Then there is a subroutine that prints glue stretch and shrink, possibly
followed by the name of finite units: */
static void print_glue(scaled d, integer order, str_number s)
{ /* prints a glue component */
  print_scaled(d);
  if ((unsigned long)order>filll) {
    print("foul"); return;
  }
  if (order<=normal) {
    if (s!=0)
      print(s);
    return;
  }
  print("fil");
  while (order>fil) {
    print_char('l'); order--;
  }
}
