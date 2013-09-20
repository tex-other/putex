Static void endtokenlist(void)
{
  if (tokentype >= backedup) {
    if (tokentype <= inserted)
      flushlist(start);
    else {
      deletetokenref(start);
      if (tokentype == macro) {
	while (paramptr > paramstart) {
	  paramptr--;
	  flushlist(paramstack[paramptr]);
	}
      }
    }
  } else if (tokentype == utemplate) {
    if (alignstate > 500000L)
      alignstate = 0;
    else
      fatalerror(S(509));
  }
  popinput();
  checkinterrupt();
}

