Static void scandelimiter(halfword p, boolean r)
{
  if (r)
    scantwentysevenbitint();
  else {   /*404:*/
    do {
      getxtoken();   /*:404*/
    } while (curcmd == spacer || curcmd == relax);
    switch (curcmd) {

    case letter:
    case otherchar:
      curval = delcode(curchr);
      break;

    case delimnum:
      scantwentysevenbitint();
      break;

    default:
      curval = -1;
      break;
    }
  }
  if (curval < 0) {   /*1161:*/
    printnl(S(292));
    print(S(905));
    help6(S(906),S(907),S(908),S(909),S(910),S(911));
    backerror();
    curval = 0;
  }
  /*:1161*/
  smallfam(p) = (curval / 1048576L) & 15;
  smallchar(p) = (curval / 4096) & 255;
  largefam(p) = (curval / 256) & 15;
  largechar(p) = curval & 255;
}
