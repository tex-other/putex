Static void printword(memoryword w)
{
  printint(w.int_);
  printchar(' ');
  printscaled(w.sc);
  printchar(' ');
  printscaled((long)floor(unity * w.gr + 0.5));
  println();
  printint(w.hh.UU.lh);
  printchar('=');
  printint(w.hh.UU.U2.b0);
  printchar(':');
  printint(w.hh.UU.U2.b1);
  printchar(';');
  printint(w.hh.rh);
  printchar(' ');
  printint(w.qqqq.b0);
  printchar(':');
  printint(w.qqqq.b1);
  printchar(':');
  printint(w.qqqq.b2);
  printchar(':');
  printint(w.qqqq.b3);
}
