Static void scanoptionalequals(void)
{   /*406:*/
  do {
    getxtoken();   /*:406*/
  } while (curcmd == spacer);
  if (curtok != othertoken + '=')
    backinput();
}
