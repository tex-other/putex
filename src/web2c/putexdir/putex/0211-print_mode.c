static void print_mode(integer m) /* prints the mode represented by |m| */
{
  if (m>0) {
    switch (m/(max_command+1)) {
    case 0: print("vertical");
      break;
    case 1: print("horizontal");
      break;
    case 2: print("display math");
      break;
    }
  } else if (m==0) print("no");
  else {
    switch (-m/(maxcommand+1)) {
    case 0: print("internal vertical");
      break;
    case 1: print("restricted horizontal");
      break;
    case 2: print("math");
      break;
    }
  }
  print(" mode");
}
