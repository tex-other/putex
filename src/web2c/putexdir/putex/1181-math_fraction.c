Static void mathfraction(void)
{
  smallnumber c;

  c = curchr;
  if (incompleatnoad != 0) {   /*1183:*/
    if (c >= delimitedcode) {
      scandelimiter(garbage, false);
      scandelimiter(garbage, false);
    }
    if (c % delimitedcode == abovecode) {
      scannormaldimen();
    }
    printnl(S(292));
    print(S(921));
    help3(S(922),S(923),S(924));
    error();
    return;
  }  /*:1183*/
  incompleatnoad = getnode(fractionnoadsize);
  type(incompleatnoad) = fractionnoad;
  subtype(incompleatnoad) = normal;
  mathtype(numerator(incompleatnoad)) = submlist;
  info(numerator(incompleatnoad)) = link(head);
  mem[denominator(incompleatnoad) - memmin].hh = emptyfield;
  mem[leftdelimiter(incompleatnoad) - memmin].qqqq = nulldelimiter;
  mem[rightdelimiter(incompleatnoad) - memmin].qqqq = nulldelimiter;
  link(head) = 0;
  tail = head;   /*1182:*/
  if (c >= delimitedcode) {
    scandelimiter(leftdelimiter(incompleatnoad), false);
    scandelimiter(rightdelimiter(incompleatnoad), false);
  }
  switch (c % delimitedcode) {   /*:1182*/

  case abovecode:
    scannormaldimen();
    thickness(incompleatnoad) = curval;
    break;

  case overcode:
    thickness(incompleatnoad) = defaultcode;
    break;

  case atopcode:
    thickness(incompleatnoad) = 0;
    break;
  }
} 
