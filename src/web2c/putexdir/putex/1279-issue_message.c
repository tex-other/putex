Static void issuemessage(void)
{
  char oldsetting;
  char c;
  strnumber s;

  c = curchr;
  link(garbage) = scantoks(false, true);
  oldsetting = selector;
  selector = newstring;
  tokenshow(defref);
  selector = oldsetting;
  flushlist(defref);
  str_room(1);
  s = makestring();
  if (c == 0) {   /*1280:*/
    if (termoffset + flength(s) > maxprintline - 2) {
      println();
    } else if (termoffset > 0 || fileoffset > 0)
      printchar(' ');
    slowprint(s);
    fflush(termout);
  } else  /*1283:*/
  {   /*:1283*/
    printnl(S(292));
    print(S(385));
    slowprint(s);
    if (errhelp != 0)
      useerrhelp = true;
    else if (longhelpseen) {
      help1(S(974));
    } else {
      if (interaction < errorstopmode)
	longhelpseen = true;
      help4(S(975),S(976),S(977),S(978));
    }
    error();
    useerrhelp = false;
  }
  /*:1280*/
  flushstring();
}
