void print_style(integer c)
{
  switch (c/2) {
    case 0: print_esc("displaystyle"); break; /* |display_style=0| */
    case 1: print_esc("textstyle"); break; /* |text_style=2| */
    case 2: print_esc("scriptstyle"); break; /* |script_style=4| */
    case 3: print_esc("scriptscriptstyle"); break; /* |script_script_style=6| */
    default print("Unknown style!"); break;
  }
}
